#pragma once

#include "ats_base.h"

typedef struct image {
  i32 width;
  i32 height;
  u32* pixels;
} image;

static u32 image_get(const image* img, i32 x, i32 y) {
  assert(x >= 0 && x < img->width);
  assert(y >= 0 && y < img->height);

  return img->pixels[y * img->width + x];
}

static void image_set(image* img, i32 x, i32 y, u32 pixel) {
  assert(x >= 0 && x < img->width);
  assert(y >= 0 && y < img->height);
  img->pixels[y * img->width + x] = pixel;
}

