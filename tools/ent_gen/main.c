#include "../../ats_fs.h"
#include "../../ats_array.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../../ext/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../ext/stb_image_write.h"

#include "common.c"
#include "asset.c"
#include "emit.c"
#include "rect.c"

#define SPRITE_SHEET_SIZE (2048)
#define FRAME_BORDER_SIZE (2)

typedef struct {
  const char* name;

  int x;
  int y;
  int w;
  int h;
  u32* pixels;

  int min_x;
  int min_y;
  int max_x;
  int max_y;
} frame_t;

typedef struct {
  const char* name;
  frame_t* frames;
} animation_t;

typedef struct {
  const char* name;
  animation_t* animations;
} entity_t;

static entity_t* entity_array;

static const char** entity_tags;
static const char** animation_tags;
static const char** frame_tags;

static frame_t** frame_array;

static u32 sprite_sheet[SPRITE_SHEET_SIZE][SPRITE_SHEET_SIZE];

static u32 frame_get(frame_t* s, int x, int y) {
  return s->pixels[y * s->w + x];
}

static void frame_set_location(frame_t* frame) {
  u32 largest = 0;
  rect_t r = rect_array[0];
  int sw = frame->w + (FRAME_BORDER_SIZE << 1);
  int sh = frame->h + (FRAME_BORDER_SIZE << 1);
  for (u32 i = 1; i < dyn_len(rect_array); ++i) {
    rect_t a = rect_array[i];
    if ((rect_w(r) >= sw) && (rect_h(r) >= sh) && (rect_area(a) > rect_area(r))) {
      largest = i;
      r = a;
    }
  }
  dyn_rem(rect_array, largest);
  frame->x = r.min_x + FRAME_BORDER_SIZE;
  frame->y = r.min_y + FRAME_BORDER_SIZE;
  // push remaining area:
  {
    push_rect((rect_t) {
      .min_x = r.min_x + sw,
      .min_y = r.min_y,
      .max_x = r.max_x,
      .max_y = r.max_y,
    });
    push_rect((rect_t) {
      .min_x = r.min_x,
      .min_y = r.min_y + sh,
      .max_x = r.min_x + sw,
      .max_y = r.max_y,
    });
  }
}

static void parse_assets(asset_node_t* node) {
  static entity_t* entity;
  static animation_t* animation;

  if (!node) return;

  if (node->depth == ASSET_DEPTH_ENTITY) {
    animation = 0;
    entity = dyn_new(entity_array);
    entity->name = node->name;

    dyn_add(entity_tags, node->name);
  }

  if (node->depth == ASSET_DEPTH_ANIMATION && (node->tag != ASSET_TAG_FILE)) {
    animation = dyn_new(entity->animations);
    animation->name = node->name;
  }

  if (node->tag == ASSET_TAG_FILE) {
    if (!animation) {
      animation = dyn_new(entity->animations);
      animation->name = "idle";
    }
    
    // handle frame:
    {
      frame_t* frame = dyn_new(animation->frames);
      frame->name = node->name;

      int comp = 0;
      frame->pixels = (u32*)stbi_load(node->path, &frame->w, &frame->h, &comp, 4);

      {
        frame->min_x = frame->w;
        frame->min_y = frame->h;
        frame->max_x = 0;
        frame->max_y = 0;

        for (int y = 0; y < frame->h; ++y) {
          for (int x = 0; x < frame->w; ++x) {
            u32 c = frame_get(frame, x, y);
            if (c & 0xff000000) {
              if (x < frame->min_x) frame->min_x = x;
              if (y < frame->min_y) frame->min_y = y;
              if (x > frame->max_x) frame->max_x = x;
              if (y > frame->max_y) frame->max_y = y;
            }
          }
        }
      }

      dyn_add(frame_tags, node->name);
      dyn_add(frame_array, frame);
    }
    
    // handle animation:
    {
      b8 add_animation = 1;
      for (u32 i = 0; i < dyn_len(animation_tags); ++i) {
        if (strcmp(animation_tags[i], animation->name) == 0) {
          add_animation = 0;
          break;
        }
      }

      if (add_animation) {
        dyn_add(animation_tags, animation->name);
      }
    }
  }

  for (u32 i = 0; i < dyn_len(node->array); ++i) {
    parse_assets(&node->array[i]);
  }
}

static int frame_cmp(const void* va, const void* vb) {
  const frame_t* a = *((const frame_t**)va);
  const frame_t* b = *((const frame_t**)vb);
  return (b->w * b->h) - (a->w * a->h);
}

int main(void) {
  asset_node_t asset_root = asset_load_directory("assets");
  asset_node_t* node = &asset_root;
  parse_assets(node);

  // build texture:
  {
    dyn_sort(frame_array, frame_cmp);
    push_rect((rect_t) {
      .min_x = 0,
      .min_y = 0,
      .max_x = SPRITE_SHEET_SIZE,
      .max_y = SPRITE_SHEET_SIZE,
    });
    for (u32 i = 0; i < dyn_len(frame_array); ++i) {
      frame_t* f = frame_array[i];
      frame_set_location(f);
      // add to spritesheet:
      for (int y = 0; y < f->h; ++y) {
        for (int x = 0; x < f->w; ++x) {
          int px = f->x + x;
          int py = f->y + y;
          sprite_sheet[py][px] = frame_get(f, x, y);
        }
      }
      // expand pixels at borders:
      {
        // top/bot:
        for (i32 x = 0; x < f->w; ++x) {
          int px = f->x + x;
          {
            int py = f->y - 1; 
            u32 pixel = frame_get(f, x, 0);
            if (pixel & 0xff000000) sprite_sheet[py][px] = pixel;
          }
          {
            int py = f->y + f->h; 
            u32 pixel = frame_get(f, x, f->h - 1);
            if (pixel & 0xff000000) sprite_sheet[py][px] = pixel;
          }
        }
        // left/right
        for (i32 y = 0; y < f->h; ++y) {
          int py = f->y + y;
          {
            int px = f->x - 1; 
            u32 pixel = frame_get(f, 0, y);
            if (pixel & 0xff000000) sprite_sheet[py][px] = pixel;
          }
          {
            int px = f->x + f->w; 
            u32 pixel = frame_get(f, f->w - 1, y);
            if (pixel & 0xff000000) sprite_sheet[py][px] = pixel;
          }
        }
        // corners:
        { u32 pixel = frame_get(f, 0,        0);        if (pixel & 0xff000000) sprite_sheet[f->y - 1]   [f->x - 1]    = pixel; }
        { u32 pixel = frame_get(f, f->w - 1, 0);        if (pixel & 0xff000000) sprite_sheet[f->y - 1]   [f->x + f->w] = pixel; }
        { u32 pixel = frame_get(f, f->w - 1, f->h - 1); if (pixel & 0xff000000) sprite_sheet[f->y + f->h][f->x + f->w] = pixel; }
        { u32 pixel = frame_get(f, 0,        f->h - 1); if (pixel & 0xff000000) sprite_sheet[f->y + f->h][f->x - 1]    = pixel; }
      }
    }

    stbi_write_png("test.png", SPRITE_SHEET_SIZE, SPRITE_SHEET_SIZE, 4, sprite_sheet, 0);
  }

  emit_format("#pragma once\n\n");

  // emit entity tags:
  {
    emit_format("typedef u16 entity_tag_t;\n");
    emit_format("enum : u16 {\n");
    emit_format("  ET_NONE,\n");
    for (int i = 0; i < dyn_len(entity_tags); ++i) {
      emit_format("  ET_%s,\n", scream(entity_tags[i]));
    }
    emit_format("  ET_COUNT,\n");
    emit_format("};\n\n");
  }

  // emit animation tags:
  {
    emit_format("typedef u16 animation_tag_t;\n");
    emit_format("enum : u16 {\n");
    for (int i = 0; i < dyn_len(animation_tags); ++i) {
      emit_format("  AT_%s,\n", scream(animation_tags[i]));
    }
    emit_format("  AT_COUNT,\n");
    emit_format("};\n\n");
  }

  // emit animation tags:
  {
    emit_format("typedef u16 frame_tag_t;\n");
    emit_format("enum : u16 {\n");
    emit_format("  FT_NONE,\n");
    for (int i = 0; i < dyn_len(frame_tags); ++i) {
      emit_format("  FT_%s,\n", scream(frame_tags[i]));
    }
    emit_format("  FT_COUNT,\n");
    emit_format("};\n\n");
  }

  // emit entity struct:
  {
    emit_format("typedef struct {\n");
    emit_format("  char name[256];\n");
    emit_format("  frame_tag_t animations[AT_COUNT];\n");
    emit_format("} entity_info_t;\n\n");
  }

  // emit entity table:
  {
    emit_format("static entity_info_t entity_info_table[ET_COUNT] = {\n");
    for (u32 i = 0; i < dyn_len(entity_array); ++i) {
      entity_t* e = entity_array + i;

      emit_format("  [ET_%s] = {\n", scream(e->name));
      emit_format("    .name = \"%s\",\n", e->name);
      emit_format("    .animations = {\n");
      for (u32 j = 0; j < dyn_len(e->animations); ++j) {
        animation_t* a = e->animations + j;
        char animation_buffer[NAME_MAX];
        char frame_buffer[NAME_MAX];
        strcpy_s(animation_buffer, NAME_MAX, scream(a->name));
        strcpy_s(frame_buffer, NAME_MAX, scream(a->frames[0].name));
        emit_format("      [AT_%s] = FT_%s,\n", animation_buffer, frame_buffer);
      }
      emit_format("    },\n");
      emit_format("  },\n");
    }
    emit_format("};\n\n");
  }

  // emit frame_rect_t
  {
    emit_format("typedef struct {\n");
    emit_format("  i16 min_x;\n");
    emit_format("  i16 min_y;\n");
    emit_format("  i16 max_x;\n");
    emit_format("  i16 max_y;\n");
    emit_format("} frame_rect_t;\n\n");
  }
  
  // emit frame struct
  {
    emit_format("typedef struct {\n");
    emit_format("  entity_tag_t entity;\n");
    emit_format("  animation_tag_t animation;\n");
    emit_format("  frame_tag_t next;\n");
    emit_format("  frame_rect_t rect;\n");
    emit_format("  frame_rect_t pixel_rect;\n");
    emit_format("} frame_info_t;\n\n");
  }

  // emit frame table:
  {
    char next_buffer[NAME_MAX] = {};
    emit_format("static frame_info_t frame_info_table[FT_COUNT] = {\n");

    for (u32 i = 0; i < dyn_len(entity_array); ++i) {
      entity_t* e = entity_array + i;

      for (u32 j = 0; j < dyn_len(e->animations); ++j) {
        animation_t* a = e->animations + j;

        for (u32 k = 0; k < dyn_len(a->frames); ++k) {
          frame_t* f = a->frames + k;
          emit_format("  [FT_%s] = {\n", scream(f->name));
          emit_format("    .next        = FT_%s,\n", scream((a->frames + ((k + 1) % dyn_len(a->frames)))->name));
          emit_format("    .entity      = ET_%s,\n", scream(e->name));
          emit_format("    .animation   = AT_%s,\n", scream(a->name));
          emit_format("    .rect = {\n");
          emit_format("      .min_x       = %d,\n", f->x);
          emit_format("      .min_y       = %d,\n", f->y);
          emit_format("      .max_x       = %d,\n", f->x + f->w);
          emit_format("      .max_y       = %d,\n", f->y + f->h);
          emit_format("    },\n");
          emit_format("    .pixel_rect = {\n");
          emit_format("      .min_x       = %d,\n", f->x + f->min_x);
          emit_format("      .min_y       = %d,\n", f->y + f->min_y);
          emit_format("      .max_x       = %d,\n", f->x + f->max_x);
          emit_format("      .max_y       = %d,\n", f->y + f->max_y);
          emit_format("    },\n");
          emit_format("  },\n");
        }
      }
    }

    emit_format("};\n\n");
  }

  emit_file_to_disk("generated_types.h");
}

