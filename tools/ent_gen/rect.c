
typedef struct {
  int min_x;
  int min_y;
  int max_x;
  int max_y;
} rect_t;

#define rect_x(r) (r).min_x
#define rect_y(r) (r).min_y
#define rect_w(r) ((r).max_x - (r).min_x)
#define rect_h(r) ((r).max_y - (r).min_y)
#define rect_area(r) (rect_w(r) * rect_h(r))

static rect_t* rect_array = 0;

static void push_rect(rect_t rect) {
  if (rect.min_x >= rect.max_x) return;
  if (rect.min_y >= rect.max_y) return;
  dyn_add(rect_array, rect);
}

