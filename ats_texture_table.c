
// ------------------------------------ texture table ------------------------------------- //

struct tex_entry {
  b32 in_use;
  u32 hash;

  struct tex_rect rect;
  char name[64];
};

struct tex_table {
  u16 width;
  u16 height;
  u32* pixels;

  struct tex_entry array[TEXTURE_TABLE_SIZE];
};

struct tex_image {
  b32 user_provided;

  u16 width;
  u16 height;
  const u32* pixels;

  char name[256];
};

static struct tex_table texture_table;
static usize tex_image_count;
static struct tex_image tex_image_array[1024];

struct tex_table* tex_get_table(void) {
  return &texture_table;
}

const u32* tex_get_pixels(void) {
  return texture_table.pixels;
}

u16 tex_get_width(void) {
  return texture_table.width;
}

u16 tex_get_height(void) {
  return texture_table.height;
}

struct tex_rect tex_get_rect(struct tex_id id) {
  return texture_table.array[id.index].rect;
}

struct tex_id tex_get_id(const char* name) {
  u32 hash  = hash_str(name);
  u16 index = hash % TEXTURE_TABLE_SIZE;

  while (texture_table.array[index].in_use) {
    if ((texture_table.array[index].hash == hash) && (strcmp(texture_table.array[index].name, name) == 0)) {
      struct tex_id id = { index };
      return id;
    }
    index = (index + 1) % TEXTURE_TABLE_SIZE;
  }

  assert(false);
  return (struct tex_id) {0};
}

struct tex_rect tex_get(const char* name) {
  return tex_get_rect(tex_get_id(name));
}

static void cstr_copy_without_extension(char* out, const char* str) {
  while (*str && *str != '.') {
    *(out++) = *(str++);
  }
  *out = '\0';
}

static void cstr_concat(char* out, const char* a, const char* b) {
  while (*a) *out++ = *a++;
  while (*b) *out++ = *b++;
  *(out) = '\0';
}

static int tex_cmp_image(const void* va, const void* vb) {
  const struct tex_image* a = (const struct tex_image*)va;
  const struct tex_image* b = (const struct tex_image*)vb;
  return b->width - a->width;
}

void tex_add_image(const char* name, const u32* pixels, u16 width, u16 height) {
  struct tex_image image = {0};

  image.user_provided = true;
  image.width = width;
  image.height = height;
  image.pixels = pixels;

  strcpy(image.name, name);

  tex_image_array[tex_image_count++] = image;
}

void tex_load_dir(const char* dir_path) {
  for_iter(file_iter, it, file_iter_create(dir_path, "*.png")) {
    struct tex_image image = {0};

    image.pixels = file_load_image(it.current, &image.width, &image.height);

    cstr_copy_without_extension(image.name, it.data.cFileName);
    tex_image_array[tex_image_count++] = image;
  }
}

void tex_begin(u16 width, u16 height) {
  tex_image_count = 0;

  texture_table = (struct tex_table) {
    width,
    height, 
    mem_array(u32, (usize)(width * height)),
  };

  texture_table.array[0].in_use = true;
}

static usize tex_stack_top; 
static struct tex_rect tex_stack_buf[4096];

b32 rect_contains_image(struct tex_rect rect, u16 width, u16 height) {
  u16 rect_width  = rect.max_x - rect.min_x;
  u16 rect_height = rect.max_y - rect.min_y;

  return width <= rect_width && height <= rect_height;
}

static struct tex_rect tex_get_fit(u16 width, u16 height) {
  u32 j = 0;
  for (j = 0; j < tex_stack_top; ++j) {
    if (rect_contains_image(tex_stack_buf[j], width, height)) {
      break;
    }
  }
  struct tex_rect rect = tex_stack_buf[j];
  tex_stack_buf[j] = tex_stack_buf[--tex_stack_top];
  return rect;
}

static void _tex_add_entry(const char* name, struct tex_rect rect) {
  u32 hash  = hash_str(name);
  u16 index = hash % TEXTURE_TABLE_SIZE;

  while (texture_table.array[index].in_use) {
    if ((texture_table.array[index].hash == hash) && (strcmp(texture_table.array[index].name, name) == 0))
      assert(true);

    index = (index + 1) % TEXTURE_TABLE_SIZE;
  }

  struct tex_entry* entry = &texture_table.array[index];

  entry->in_use = true;
  entry->rect = rect;
  entry->hash = hash;

  strcpy_s(entry->name, 64, name);
}


static inline u32 _tex_get_pixel(const struct tex_image* image, u16 x, u16 y) {
  return image->pixels[y * image->width + x];
}

static inline void _tex_set_pixel(u16 x, u16 y, u32 color) {
  texture_table.pixels[y * texture_table.width + x] = color;
}

void tex_end(void) {
  tex_stack_top = 0;
  tex_stack_buf[tex_stack_top++] = (struct tex_rect) {
    0,
    0,
    texture_table.width,
    texture_table.height,
  };

  qsort(tex_image_array, tex_image_count, sizeof (struct tex_image), tex_cmp_image);

  for (usize i = 0; i < tex_image_count; ++i) {
    struct tex_image* image = &tex_image_array[i];
    struct tex_rect   rect = tex_get_fit(image->width + 2, image->height + 2);

    u16 offset_x = rect.min_x + 1;
    u16 offset_y = rect.min_y + 1;
    u16 size_x   = image->width + 2;
    u16 size_y   = image->height + 2;

    _tex_add_entry(image->name, (struct tex_rect) {
      offset_x,
      offset_y,
      (u16)(offset_x + image->width),
      (u16)(offset_y + image->height),
    });

    for (u16 y = 0; y < image->height; ++y) {
      for (u16 x = 0; x < image->width; ++x) {
        _tex_set_pixel(offset_x + x, offset_y + y, _tex_get_pixel(image, x, y));

        if (x == 0)                 _tex_set_pixel(offset_x + x - 1, offset_y + y, _tex_get_pixel(image, x, y));
        if (x == image->width - 1)  _tex_set_pixel(offset_x + x + 1, offset_y + y, _tex_get_pixel(image, x, y));
        if (y == 0)                 _tex_set_pixel(offset_x + x, offset_y + y - 1, _tex_get_pixel(image, x, y));
        if (y == image->height - 1) _tex_set_pixel(offset_x + x, offset_y + y + 1, _tex_get_pixel(image, x, y));
      }
    }

    struct tex_rect a = {
      (u16)(rect.min_x + size_x),
      rect.min_y,
      rect.max_x,
      rect.max_y,
    };

    struct tex_rect b = {
      rect.min_x,
      (u16)(rect.min_y + size_y),
      (u16)(rect.min_x + size_x),
      rect.max_y,
    };

    if (a.min_x + size_x <= rect.max_x && a.min_y + size_y <= rect.max_y) {
      tex_stack_buf[tex_stack_top++] = a;
    }

    if (b.min_x + size_x <= rect.max_x && b.min_y + size_y <= rect.max_y) {
      tex_stack_buf[tex_stack_top++] = b;
    }

    if (!image->user_provided) {
      file_free_image(image->pixels);
    }
  }

  tex_image_count = 0;
  tex_stack_top = 0;
}

