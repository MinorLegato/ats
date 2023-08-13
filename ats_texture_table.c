
// ------------------------------------ texture table ------------------------------------- //

typedef struct
{
   b32      in_use;
   u32      hash;
   tex_rect rect;
   char     name[64];
} tex_entry;

typedef struct
{
   u16   width;
   u16   height;
   u32*  pixels;

   tex_entry array[TEXTURE_TABLE_SIZE];
} tex_table;

typedef struct
{
   b32 user_provided;

   u16 width;
   u16 height;
   const u32* pixels;

   char name[256];
} tex_image;

static tex_table  texture_table;
static usize      tex_image_count;
static tex_image  tex_image_array[1024];

extern tex_table* tex_get_table(void)
{
   return &texture_table;
}

extern const u32* tex_get_pixels(void)
{
   return texture_table.pixels;
}

extern u16 tex_get_width(void)
{
   return texture_table.width;
}

extern u16 tex_get_height(void)
{
   return texture_table.height;
}

extern tex_rect tex_get_rect(tex_id id)
{
   return texture_table.array[id.index].rect;
}

extern tex_id tex_get_id(const char* name)
{
   u32 hash  = hash_str(name);
   u16 index = hash % TEXTURE_TABLE_SIZE;

   while (texture_table.array[index].in_use) {
      if ((texture_table.array[index].hash == hash) && (strcmp(texture_table.array[index].name, name) == 0)) {
         tex_id id = { index };
         return id;
      }
      index = (index + 1) % TEXTURE_TABLE_SIZE;
   }

   assert(false);
   return make(tex_id) ATS_INIT;
}

extern tex_rect tex_get(const char* name)
{
   return tex_get_rect(tex_get_id(name));
}

static void cstr_copy_without_extension(char* out, const char* str)
{
   while (*str && *str != '.') {
      *(out++) = *(str++);
   }
   *out = '\0';
}

static void cstr_concat(char* out, const char* a, const char* b)
{
   while (*a) *out++ = *a++;
   while (*b) *out++ = *b++;
   *(out) = '\0';
}

static int tex_cmp_image(const void* va, const void* vb)
{
   const tex_image* a = (const tex_image*)va;
   const tex_image* b = (const tex_image*)vb;

   return b->width - a->width;
}

extern b32 rect_contains_image(tex_rect rect, const tex_image* image)
{
   u16 rect_width  = rect.max_x - rect.min_x;
   u16 rect_height = rect.max_y - rect.min_y;

   return image->width <= rect_width && image->height <= rect_height;
}

extern void tex_add_image(const char* name, const u32* pixels, u16 width, u16 height)
{
   tex_image image = ATS_INIT;

   image.user_provided = true;
   image.width = width;
   image.height = height;
   image.pixels = pixels;

   strcpy(image.name, name);

   tex_image_array[tex_image_count++] = image;
}

extern void tex_load_dir(const char* dir_path)
{
   for_iter(file_iter, it, file_iter_create(dir_path, "*.png")) {
      tex_image image = ATS_INIT;

      image.pixels = file_load_image(it.current, &image.width, &image.height);

      cstr_copy_without_extension(image.name, it.data.cFileName);
      tex_image_array[tex_image_count++] = image;
   }
}

extern void tex_begin(u16 width, u16 height)
{
   tex_image_count = 0;

   texture_table = make(tex_table) {
      width,
      height, 
      mem_array(u32, width * height),
   };

   texture_table.array[0].in_use = true;
}

static usize    tex_stack_top; 
static tex_rect tex_stack_buf[4096];

static tex_rect tex_get_fit(const tex_image* img)
{
   u32 j = 0;
   for (j = 0; j < tex_stack_top; ++j) {
      if (rect_contains_image(tex_stack_buf[j], img)) {
         break;
      }
   }
   tex_rect rect = tex_stack_buf[j];
   tex_stack_buf[j] = tex_stack_buf[--tex_stack_top];
   return rect;
}

static void _tex_add_entry(const char* name, tex_rect rect)
{
   u32 hash  = hash_str(name);
   u16 index = hash % TEXTURE_TABLE_SIZE;

   while (texture_table.array[index].in_use) {
      if ((texture_table.array[index].hash == hash) && (strcmp(texture_table.array[index].name, name) == 0))
         assert(true);

      index = (index + 1) % TEXTURE_TABLE_SIZE;
   }

   tex_entry* entry = &texture_table.array[index];

   entry->in_use = true;
   entry->rect = rect;
   entry->hash = hash;

   strcpy_s(entry->name, 64, name);
}

extern void tex_end(void)
{
   tex_stack_top = 0;
   tex_stack_buf[tex_stack_top++] = make(tex_rect) {
      0,
      0,
      (u16)(texture_table.width - 1),
      (u16)(texture_table.height - 1),
   };

   qsort(tex_image_array, tex_image_count, sizeof (tex_image), tex_cmp_image);

   for (usize i = 0; i < tex_image_count; ++i) {
      tex_image* image = &tex_image_array[i];
      tex_rect   rect = tex_get_fit(image);

      u16 size_x   = image->width + 2;
      u16 size_y   = image->height + 2;

      u16 offset_x = rect.min_x;
      u16 offset_y = rect.min_y;

      _tex_add_entry(image->name, make(tex_rect) {
         offset_x + 1u,
         offset_y + 1u,
         offset_x + size_x - 1u,
         offset_y + size_y - 1u,
      });

      for (i32 y = 0; y < image->height; ++y) {
         for (i32 x = 0; x < image->width; ++x) {
            u32 img_index = y * image->width + x;
            u32 tex_index = (y + offset_y + 1) * texture_table.width + (x + offset_x + 1);

            u32 pixel = image->pixels[img_index];
            texture_table.pixels[tex_index] = pixel;
         }
      }

      tex_rect a = {
         (u16)(rect.min_x),          (u16)(rect.min_y + size_y),
         (u16)(rect.min_x + size_x), (u16)(rect.max_y),
      };

      tex_rect b = {
         (u16)(rect.min_x + size_x), (u16)(rect.min_y),
         (u16)(rect.max_x),          (u16)(rect.max_y),
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

