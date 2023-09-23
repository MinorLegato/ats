
// ------------------------------------ texture table ------------------------------------- //

typedef struct {
  int       in_use;
  unsigned  hash;
  tex_rect  rect;
  char      name[64];
} tex_entry;

typedef struct {
  unsigned short  width;
  unsigned short  height;
  unsigned*       pixels;
  tex_entry       array[TEXTURE_TABLE_SIZE];
} tex_table;

typedef struct {
  int user_provided;

  unsigned short  width;
  unsigned short  height;
  const unsigned* pixels;

  char name[256];
} tex_image;

static tex_table  texture_table;
static unsigned   tex_image_count;
static tex_image  tex_image_array[1024];

tex_table*
tex_get_table(void) {
  return &texture_table;
}

const unsigned*
tex_get_pixels(void) {
  return texture_table.pixels;
}

unsigned short
tex_get_width(void) {
  return texture_table.width;
}

unsigned short
tex_get_height(void) {
  return texture_table.height;
}

tex_rect
tex_get_rect(tex_id id) {
  return texture_table.array[id.index].rect;
}

tex_id
tex_get_id(const char* name) {
  unsigned hash = hash_str(name);
  unsigned short index = hash % TEXTURE_TABLE_SIZE;

  while (texture_table.array[index].in_use) {
    if ((texture_table.array[index].hash == hash) && (strcmp(texture_table.array[index].name, name) == 0)) {
      tex_id id = { index };
      return id;
    }
    index = (index + 1) % TEXTURE_TABLE_SIZE;
  }

  assert(0);
  return (tex_id) {0};
}

tex_rect
tex_get(const char* name) {
  return tex_get_rect(tex_get_id(name));
}

static void
cstr_copy_without_extension(char* out, const char* str) {
  while (*str && *str != '.') {
    *(out++) = *(str++);
  }
  *out = '\0';
}

static void
cstr_concat(char* out, const char* a, const char* b) {
  while (*a) *out++ = *a++;
  while (*b) *out++ = *b++;
  *(out) = '\0';
}

static int
tex_cmp_image(const void* va, const void* vb) {
  const tex_image* a = (const tex_image*)va;
  const tex_image* b = (const tex_image*)vb;

  return b->width - a->width;
}

void
tex_add_image(const char* name, const unsigned* pixels, unsigned short width, unsigned short height) {
  tex_image image = {0};

  image.user_provided = 1;
  image.width         = width;
  image.height        = height;
  image.pixels        = pixels;

  strcpy(image.name, name);

  tex_image_array[tex_image_count++] = image;
}

void
tex_load_dir(const char* dir_path) {
  for (file_iter it = file_iter_create(dir_path, "*.png");
       file_iter_is_valid(&it);
       file_iter_advance(&it)
  ) {
    tex_image image = {0};

    image.pixels = file_load_image(it.current, &image.width, &image.height);

    cstr_copy_without_extension(image.name, it.data.cFileName);
    tex_image_array[tex_image_count++] = image;
  }
}

void
tex_begin(unsigned short width, unsigned short height) {
  tex_image_count = 0;

  texture_table = (tex_table) {
    width,
    height, 
    mem_array(unsigned, width * height),
  };

  texture_table.array[0].in_use = 1;
}

static unsigned tex_stack_top; 
static tex_rect tex_stack_buf[4096];

int
rect_contains_image(tex_rect rect, unsigned short width, unsigned short height) {
  unsigned short rect_width  = rect.max_x - rect.min_x;
  unsigned short rect_height = rect.max_y - rect.min_y;
  return width <= rect_width && height <= rect_height;
}

static tex_rect
tex_get_fit(unsigned short width, unsigned short height) {
  unsigned j = 0;
  for (j = 0; j < tex_stack_top; ++j) {
    if (rect_contains_image(tex_stack_buf[j], width, height)) {
      break;
    }
  }
  tex_rect rect = tex_stack_buf[j];
  tex_stack_buf[j] = tex_stack_buf[--tex_stack_top];
  return rect;
}

static void
_tex_add_entry(const char* name, tex_rect rect) {
  unsigned hash  = hash_str(name);
  unsigned short index = hash % TEXTURE_TABLE_SIZE;

  while (texture_table.array[index].in_use) {
    if ((texture_table.array[index].hash == hash) && (strcmp(texture_table.array[index].name, name) == 0))
      assert(0);
    index = (index + 1) % TEXTURE_TABLE_SIZE;
  }

  tex_entry* entry = &texture_table.array[index];

  entry->in_use = 1;
  entry->rect   = rect;
  entry->hash   = hash;

  strcpy_s(entry->name, 64, name);
}


static unsigned
_tex_get_pixel(const tex_image* image, unsigned short x, unsigned short y) {
  return image->pixels[y * image->width + x];
}

static void
_tex_set_pixel(unsigned short x, unsigned short y, unsigned color) {
  texture_table.pixels[y * texture_table.width + x] = color;
}


void
tex_end(void) {
  tex_stack_top = 0;
  tex_stack_buf[tex_stack_top++] = (tex_rect) {
    0,
    0,
    texture_table.width,
    texture_table.height,
  };

  qsort(tex_image_array, tex_image_count, sizeof (tex_image), tex_cmp_image);

  for (unsigned i = 0; i < tex_image_count; ++i) {
    tex_image* image = &tex_image_array[i];
    tex_rect   rect = tex_get_fit(image->width + 2, image->height + 2);

    unsigned short offset_x = rect.min_x + 1;
    unsigned short offset_y = rect.min_y + 1;
    unsigned short size_x   = image->width + 2;
    unsigned short size_y   = image->height + 2;

    _tex_add_entry(image->name, (tex_rect) {
      offset_x,
      offset_y,
      (unsigned short)(offset_x + image->width),
      (unsigned short)(offset_y + image->height),
    });

    for (unsigned short y = 0; y < image->height; ++y) {
      for (unsigned short x = 0; x < image->width; ++x) {
        _tex_set_pixel(offset_x + x, offset_y + y, _tex_get_pixel(image, x, y));

        if (x == 0)                 _tex_set_pixel(offset_x + x - 1, offset_y + y, _tex_get_pixel(image, x, y));
        if (x == image->width - 1)  _tex_set_pixel(offset_x + x + 1, offset_y + y, _tex_get_pixel(image, x, y));
        if (y == 0)                 _tex_set_pixel(offset_x + x, offset_y + y - 1, _tex_get_pixel(image, x, y));
        if (y == image->height - 1) _tex_set_pixel(offset_x + x, offset_y + y + 1, _tex_get_pixel(image, x, y));
      }
    }

    tex_rect a = {
      (unsigned short)(rect.min_x + size_x),
      rect.min_y,
      rect.max_x,
      rect.max_y,
    };

    tex_rect b = {
      rect.min_x,
      (unsigned short)(rect.min_y + size_y),
      (unsigned short)(rect.min_x + size_x),
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

