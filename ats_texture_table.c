
// ------------------------------------ texture table ------------------------------------- //

typedef struct
{
   b32      user_provided;
   image    img;
   char     name[256];
} tex_image;

static tex_table  texture_table;
static usize      tex_image_count;
static tex_image  tex_image_array[1024];

extern tex_table* tex_get_table(void)
{
   return &texture_table;
}

extern void tex_add_image(const char* name, image img)
{
   tex_image data = ATS_INIT;

   data.user_provided = true;
   data.img = img;

   strcpy(data.name, name);

   tex_image_array[tex_image_count++] = data;
}

extern image tex_get_image(void)
{
   return texture_table.img;
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
   tex_image* a = (tex_image*)va;
   tex_image* b = (tex_image*)vb;

   int dw = b->img.width  - a->img.width;
   int dh = a->img.height - a->img.height;

   return b->img.width - a->img.width;
}

extern b32 rect_contains_image(tex_rect rect, image image)
{
   u32 rect_width  = rect.max_x - rect.min_x;
   u32 rect_height = rect.max_y - rect.min_y;

   return image.width <= rect_width && image.height <= rect_height;
}

extern void tex_load_from_dir(const char* dir_path)
{
   for_iter(file_iter, it, file_iter_create(dir_path, "*.png")) {
      tex_image data = ATS_INIT;

      data.img = file_load_image(it.current);
      cstr_copy_without_extension(data.name, it.data.cFileName);
      tex_image_array[tex_image_count++] = data;
   }
}

extern void tex_begin(int width, int height)
{
   tex_image_count = 0;

   texture_table = make(tex_table) {
      width,
      height, 
      mem_array(u32, (usize)(width * height)),
   };

   texture_table.array[0].in_use = true;

   for (u32 i = 0; i < width * height; ++i) {
      texture_table.img.pixels[i] = 0xff000000;
   }
}

static usize    tex_stack_top; 
static tex_rect tex_stack_buf[4096];

static tex_rect tex_get_fit(image img)
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

extern void tex_end(void)
{
   tex_stack_top = 0;
   tex_stack_buf[tex_stack_top++] = make(tex_rect) {
      0,
      0,
      (u16)(texture_table.img.width - 1),
      (u16)(texture_table.img.height - 1),
   };

   qsort(tex_image_array, tex_image_count, sizeof (tex_image), tex_cmp_image);

   for (usize i = 0; i < tex_image_count; ++i) {
      tex_image* data = &tex_image_array[i];
      tex_rect rect = tex_get_fit(data->img);

      u16 size_x   = data->img.width + 2;
      u16 size_y   = data->img.height + 2;

      u16 offset_x = rect.min_x;
      u16 offset_y = rect.min_y;

      _tex_add_entry(data->name, make(tex_rect) {
         offset_x + 1u,
         offset_y + 1u,
         offset_x + size_x - 1u,
         offset_y + size_y - 1u,
      });

      for (i32 y = 0; y < data->img.height; ++y) {
         for (i32 x = 0; x < data->img.width; ++x) {
            u32 pixel = image_get(&data->img, x, y);
            image_set(&texture_table.img, x + offset_x + 1, y + offset_y + 1, pixel);
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

      if (!data->user_provided) {
         file_free_image(&data->img);
      }
   }

   tex_image_count = 0;
   tex_stack_top = 0;
}

