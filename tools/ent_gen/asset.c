#include <string.h>

#define ASSET_PATH_MAX_DEPTH (8)

typedef u8 asset_tag_t;
enum {
  ASSET_TAG_ROOT,
  ASSET_TAG_FILE,
  ASSET_TAG_DIR,
};

typedef u8 asset_depth_t;
enum {
  ASSET_DEPTH_NONE,
  ASSET_DEPTH_ENTITY,
  ASSET_DEPTH_ANIMATION,
  ASSET_DEPTH_FRAME,
};

typedef struct asset_node {
  struct asset_node* root;
  struct asset_node* array;

  asset_tag_t tag;
  asset_depth_t depth;

  char name[PATH_MAX];
  char path[PATH_MAX];
} asset_node_t;

static u32 asset_path_count;
static const char* asset_path_array[ASSET_PATH_MAX_DEPTH];

static char* asset_get_path(char* path, const char* file_name) {
  for (u32 i = 0; i < asset_path_count; ++i) {
    path += sprintf(path, "%s/", asset_path_array[i]);
  }
  path += sprintf(path, "%s", file_name);
  return path;
}

static DIR* dir_open(const char* path) {
  asset_path_array[asset_path_count++] = path;
  char full_path[PATH_MAX];
  char* it = full_path;
  for (u32 i = 0; i < asset_path_count; ++i) {
    it += sprintf(it, "%s/", asset_path_array[i]);
  }
  return opendir(full_path);
}

static void dir_close(DIR* dir) {
  closedir(dir);
  asset_path_count--;
}

static asset_node_t* asset_add(asset_node_t* asset, asset_tag_t tag, const char* file_name) {
  asset_node_t* node = dyn_new(asset->array);
  node->root = asset;
  node->tag = tag;
  node->depth = asset->depth + 1;
  strcpy_s(node->name, PATH_MAX, file_name);
  asset_get_path(node->path, file_name);
  if (node->tag == ASSET_TAG_FILE) {
    char* dot = strchr(node->name, '.');
    *dot = '\0';
  }
  return node;
}

static int asset_node_cmp(const void* va, const void* vb) {
  const asset_node_t* a = va;
  const asset_node_t* b = vb;
  return strcmp(a->name, b->name);
}

static void asset_sort_tree(asset_node_t* node) {
  if (!node) return;
  dyn_sort(node->array, asset_node_cmp);
  for (int i = 0; i < dyn_len(node->array); ++i) {
    asset_sort_tree(&node->array[i]);
  }
}

static asset_node_t asset_load_directory(const char* path) {
  asset_node_t root = { .tag = ASSET_TAG_ROOT };
  DIR* ent_dir = dir_open(path);
  // entity level:
  for (struct dirent* ent = readdir(ent_dir); ent; ent = readdir(ent_dir)) {
    if (ent->d_name[0] == '.') continue;
    switch (ent->d_type) {
      case DT_REG: {
        asset_add(&root, ASSET_TAG_FILE, ent->d_name);
      } break;
      case DT_DIR: {
        asset_node_t* anim_node = asset_add(&root, ASSET_TAG_DIR, ent->d_name);
        // animation level:
        DIR* anim_dir = dir_open(ent->d_name);
        for (struct dirent* anim = readdir(anim_dir); anim; anim = readdir(anim_dir)) {
          if (anim->d_name[0] == '.') continue;
          switch (anim->d_type) {
            case DT_REG: {
            asset_add(anim_node, ASSET_TAG_FILE, anim->d_name);
            } break;
            case DT_DIR: {
              asset_node_t* frame_node = asset_add(anim_node, ASSET_TAG_DIR, anim->d_name);
              // frame level:
              DIR* frame_dir = dir_open(anim->d_name);
              for (struct dirent* frame = readdir(frame_dir); frame; frame = readdir(frame_dir)) {
                if (frame->d_name[0] == '.') continue;
                switch (frame->d_type) {
                  case DT_REG: {
                    asset_add(frame_node, ASSET_TAG_FILE, frame->d_name);
                  } break;
                }
              }
              dir_close(frame_dir);
            } break;
          }
        }
        dir_close(anim_dir);
      } break;
    }
  }
  dir_close(ent_dir);
  asset_sort_tree(&root);
  return root;
}

static void asset_print_tree(asset_node_t* node) {
  if (!node) return;
  if (node->tag != ASSET_TAG_ROOT) {
    printf("%s :: %s\n", node->name, node->path);
  }
  for (int i = 0; i < dyn_len(node->array); ++i) {
    asset_print_tree(&node->array[i]);
  }
}

