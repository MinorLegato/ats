// =============================================== ROUTINE ========================================== //
// copied and modefied from: https://gist.github.com/NoelFB/7a5fa66fc29dd7ed1c11042c30f1b00e

// Example:
// // Assuming you have a `routine` variable stored somewhere
// rt_begin(routine);
// {
//     // stuff that happens frame 1
// }
// rt_wait(1.0f);
// {
//     // after 1.0s, this block runs
// }
// rt_for(0.25f);
// {
//     // this block repeats for 0.25s
// }
// rt_step();
// {
//     // the following frame, this block is run
// }
// rt_label(0);
// {
//     if (something)
//         rt_repeat();
//
//     // not run if rt_repeat() was called
//     something_else();
// }
// rt_step();
// {
//     if (another) rt_goto(0); // jumps to label 0
//     if (another2) rt_restart();  // jumps to rt_begin
//     // otherwise the next block will be run next frame
// }
// rt_while(condition_is_true);
// {
//     // this is repeated until condition_is_true is false
// }
// rt_end();

#define RT_LABEL_OFFSET 1147483647
#define rt_hash(tag) (RT_LABEL_OFFSET + (tag))

typedef struct {
  void* at;
  f32 wait_for;
  f32 repeat_for;
} rt_state;

#define rt_begin(rt, delta_time) \
  if ((rt).wait_for > 0) { (rt).wait_for -= (delta_time); goto _rt_end; } \
  rt_state* _rt = &(rt); \
  f32 _rt_dt = (delta_time); \
  b32 _rt_mn = 1; \
  if (_rt->at) goto *_rt->at; \
  _rt->at = &&_rt_start; \
  _rt_start:;

#define rt_step() \
  if (_rt_mn) _rt->at = &&macro_var(_rt_); \
  goto _rt_end; \
  macro_var(_rt_):;

#define rt_label(name) \
  if (_rt_mn) _rt->at = &&name; \
  goto _rt_end; \
  name:;

#define rt_for(time) \
  rt_step(); \
  _rt->repeat_for += _rt_dt; \
  _rt_mn = _rt->repeat_for >= (time); \
  if (_rt_mn) _rt->repeat_for = 0;

#define rt_while(condition) \
  rt_step(); \
  _rt_mn = !(condition); \
  if (!_rt_mn)

#define rt_until(condition) \
  rt_step(); \
  _rt_mn = (condition); \
  if (_rt_mn)

#define rt_wait(time) \
  if (_rt_mn) { _rt->wait_for = (time); _rt->at = &&macro_var(_rt_); } \
  goto _rt_end; \
  macro_var(_rt_):;

#define rt_end() \
  if (_rt_mn) _rt->at = &&_rt_end; \
  goto _rt_end; \
  _rt_end:;


// Flow Statements:
// These can be used anywhere between rt_begin and rt_end,
// including if statements, while loops, etc.

// Repeats the block that this is contained within
// Skips the remainder of the block
#define rt_repeat() \
  goto _rt_end;

// Goes to a given block labeled with `rt_label`
#define rt_goto(name) \
  do { \
    _rt->at = &&name; \
    goto _rt_end; \
  } while (0);

// Restarts the entire Coroutine;
// Jumps back to `rt_begin` on the next frame
#define rt_restart() \
  do { \
    _rt->at = 0; \
    _rt->wait_for = 0; \
    _rt->repeat_for = 0; \
    goto _rt_end; \
  } while(0)

#if 0
#define rt_begin(rt, delta_time) \
  if (rt.wait_for > 0) { \
    rt.wait_for -= (delta_time); \
  } else { \
    rt_state* __rt = &(rt); \
    f32 __dt = (delta_time); \
    int __mn = 1; \
    switch (__rt->at) { \
      case 0: { \

#define rt_step() \
      } if (__mn) __rt->at = __LINE__; \
      break; \
      case __LINE__: { \

#define rt_label(name) \
      } if (__mn) __rt->at = rt_hash(name); \
      break; \
      case rt_hash(name): { \

#define rt_for(time) \
        rt_step(); \
        if (__rt->repeat_for < time) {  \
          __rt->repeat_for += __dt; \
          __mn = __rt->repeat_for >= time; \
          if (__mn) __rt->repeat_for = 0; \
        } \

#define rt_while(condition) \
      } if (__mn) __rt->at = __LINE__; \
      break; \
      case __LINE__: if (condition) { \
        __mn = 0 \

#define rt_until(condition) \
      } if (__mn) __rt->at = ((condition) ? __LINE__ : -__LINE__); \
      break; \
      case -__LINE__: \
                      if (condition) __rt->at = __LINE__; \
      break; \
      case __LINE__: { \

#define rt_wait(time) \
      } if (__mn) { \
        __rt->wait_for = time; \
        __rt->at = __LINE__; \
      } \
      break; \
      case __LINE__: { \

#define rt_end() \
      } if (__mn) __rt->at = -1; \
      break; \
    } \
  } \
  goto rt_end_of_routine;  \
  rt_end_of_routine: \

// Flow Statements:
// These can be used anywhere between rt_begin and rt_end,
// including if statements, while loops, etc.

// Repeats the block that this is contained within
// Skips the remainder of the block
#define rt_repeat() \
  goto rt_end_of_routine \

// Goes to a given block labeled with `rt_label`
#define rt_goto(name) \
  do { \
    __rt->at = rt_hash(name); \
    goto rt_end_of_routine; \
  } while(0) \

// Restarts the entire Coroutine;
// Jumps back to `rt_begin` on the next frame
#define rt_restart() \
  do { \
    __rt->at = 0; \
    __rt->wait_for = 0; \
    __rt->repeat_for = 0; \
    goto rt_end_of_routine; \
  } while(0) \

#endif
