#pragma once

#define swap(type, a, b)      do { type tmp = (a); (a) = (b); (b) = tmp; } while(0)

#define countof(array)        (sizeof (array) / sizeof (array)[0])

#define is_power_of_two(x)    (((x) != 0) && ((x) & ((x) - 1)) == 0)

#define align_down(n, a)      ((n) & ~((a) - 1))
#define align_up(n, a)        align_down((n) + (a) - 1, (a))
#define align_down_ptr(p, a)  ((void*)align_down((uintptr_t)(p), (a)))
#define align_up_ptr(p, a)    ((void*)align_down((uintptr_t)(p), (a)))

#define clamp_min(n, min)     ((n) < (min)? (min) : (n))
#define clamp_max(n, max)     ((n) > (max)? (max) : (n))
#define clamp(n, min, max)    clamp_max(clamp_min(n, min), max)
#define clamp01(n)            clamp(n, 0, 1)
#undef min
#define min(a, b)             ((a) < (b)? (a) : (b))
#undef max
#define max(a, b)             ((a) > (b)? (a) : (b))
#define lerp(a, b, t)         ((a) + (t) * ((b) - (a)))

#define join_helper(a, b)     a##b
#define join_token(a, b)      join_helper(a, b)
#define macro_var(a)          join_token(a, __LINE__)

#define scope_guard(start, end) for (int macro_var(i) = ((start), 0); !macro_var(i); (macro_var(i)++, (end)))

#define def(_val, _def) ((_val) == 0? (_def) : (_val))

#define for_range(index, start, below) \
  for (int index = (start); index < (below); ++index)

#define for_array(index, array) \
  for (int index = 0; index < countof(array); ++index)

#define repeat(count) \
  for (int macro_var(index) = 0; macro_var(index) < (count); ++macro_var(index))

#define for_r2(rect, ix, iy) \
  for (int iy = rect.min.y; iy <= rect.max.y; ++iy) \
  for (int ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_r3(rect, ix, iy, iz) \
  for (int iz = rect.min.z; iz <= rect.max.z; ++iz) \
  for (int iy = rect.min.y; iy <= rect.max.y; ++iy) \
  for (int ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_iter(iter_type, iter_name, ...) \
  for (struct iter_type iter_name = (__VA_ARGS__); \
       iter_type##_is_valid(&iter_name); \
       iter_type##_advance(&iter_name))

#define for_each(type, var, ...) \
  for (struct type* var = (struct type*)0xdeadbeefull; var != NULL; var = NULL) \
  for (struct type##_iter macro_var(it) = (__VA_ARGS__); \
       (var = macro_var(it).current, type##_iter_is_valid(&macro_var(it))); \
       type##_iter_advance(&macro_var(it)))

#define match(...) \
  for (u32 _match_hash = hash_str(__VA_ARGS__), _with_hash = 0, _match_found = 0; _match_hash; _match_hash = 0)

#define with(...) \
  if (!_match_found && (_match_found = ((_with_hash = hash_str(__VA_ARGS__)) == _match_hash)))

