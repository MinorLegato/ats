#pragma once

static void atomic_store(int* out, int n);

static int atomic_load(const int* value);

static void atomic_inc(int* value);
static void atomic_dec(int* value);

static int atomic_add(int* out, int n);
static int atomic_sub(int* out, int n);

static int atomic_or(int* out, int n);
static int atomic_and(int* out, int n);
static int atomic_xor(int* out, int n);

// ----------------------------------------- threads ------------------------------------------ //

typedef struct {
  void* id;
} thread;

typedef int thread_proc(void* args);

static thread thread_create(thread_proc* proc, void* args);

static int  thread_join(thread* td);
static void thread_destroy(thread* td);
static void thread_yield(void);
static void thread_sleep(u32 milliseconds);
static void thread_exit(int exit_code);

//----------------------------------------- task queue -------------------------------------------- //

typedef void task_proc_t(void* data);

typedef struct {
  task_proc_t* proc;
  void* data;
} task_data;

#define TASK_QUEUE_MAX (64)
#define TASK_QUEUE_MOD (63)

typedef struct {
  thread thread;

  int begin;
  int end;
  task_data array[TASK_QUEUE_MAX];
} task_queue;

static int task_queue_thread(void* data);
static void task_queue_init(task_queue* queue);
static void task_queue_push(task_queue* queue, task_proc_t* proc, void* data);
static void task_queue_wait(task_queue* queue);

//----------------------------------------- task queue -------------------------------------------- //

#define TASK_SYSTEM_MAX_THREADS (32)

typedef struct {
  u32 thread_count;
  int next;
  task_queue queue[TASK_SYSTEM_MAX_THREADS];
} task_system;

static void task_system_init(task_system* ts, u32 thread_count);
static void task_system_push(task_system* ts, task_proc_t* proc, void* data);
static void task_system_wait(task_system* ts);

// ===================================== INTERNAL ===================================== //

#include <winbase.h>

static void atomic_store(int* out, int n)
{
  InterlockedExchange(out, n);
}

static int atomic_load(const int* value)
{
  return InterlockedOr((int*)value, 0); // shady stuff!
}

static void atomic_inc(int* value)
{
  InterlockedIncrement(value);
}

static void atomic_dec(int* value)
{
  InterlockedDecrement(value);
}

static int atomic_add(int* out, int n)
{
  return InterlockedExchangeAdd(out, n);
}

static int atomic_sub(int* out, int n)
{
  return InterlockedExchangeAdd(out, -n);
}

static int atomic_or(int* out, int n)
{
  return InterlockedOr(out, n);
}

static int atomic_and(int* out, int n)
{
  return InterlockedAnd(out, n);
}

static int atomic_xor(int* out, int n)
{
  return InterlockedXor(out, n);
}

// ----------------------------------------- threads ------------------------------------------ //

static thread thread_create(thread_proc* proc, void* args)
{
  thread td = {0};
  td.id = CreateThread(NULL, 0, proc, args, 0, NULL);
  return td;
}

static int thread_join(thread* td)
{
  WaitForSingleObject(td->id, INFINITE);
  DWORD exit_code = 0;
  GetExitCodeThread(td->id, &exit_code);
  return exit_code;
}

static void thread_destroy(thread* td)
{
  CloseHandle(td->id);
}

static void thread_yield(void)
{
  SwitchToThread();
}

static void thread_sleep(u32 milliseconds)
{
  Sleep(milliseconds);
}

static void thread_exit(int exit_code)
{
  ExitThread(exit_code);
}

//----------------------------------------- task queue -------------------------------------------- //

static int task_queue_thread(void* data)
{
  task_queue* queue = data;

  while (true) {
    if (queue->begin == queue->end) {
      thread_yield();
    } else {
      task_data* task = queue->array + queue->begin;
      task->proc(task->data);

      atomic_inc(&queue->begin);
      atomic_and(&queue->begin, TASK_QUEUE_MOD);
    }
  }

  return 0;
}

static void task_queue_init(task_queue* queue)
{
  memset(queue, 0, sizeof *queue);
  queue->thread = thread_create(task_queue_thread, queue);
}

static void task_queue_push(task_queue* queue, task_proc_t* proc, void* data)
{
  queue->array[queue->end] = (task_data) {
    .proc = proc,
    .data = data,
  };

  atomic_inc(&queue->end);
  atomic_and(&queue->end, TASK_QUEUE_MOD);
}

static void task_queue_wait(task_queue* queue)
{
  while (queue->begin != queue->end) {
    thread_yield();
  }
}

//----------------------------------------- task queue -------------------------------------------- //

static void task_system_init(task_system* ts, u32 thread_count)
{
  if (!thread_count) {
    struct _SYSTEM_INFO info = {0};
    GetSystemInfo(&info);
    ts->next = 0;
    ts->thread_count = clamp_max(info.dwNumberOfProcessors, TASK_SYSTEM_MAX_THREADS);
  } else {
    ts->next = 0;
    ts->thread_count = thread_count;
  }

  for (int i = 0; i < ts->thread_count; ++i) {
    task_queue_init(&ts->queue[i]);
  }
}

static void task_system_push(task_system* ts, task_proc_t* proc, void* data)
{
  task_queue_push(&ts->queue[ts->next], proc, data);
  ts->next = (ts->next + 1) % ts->thread_count;
}

static void task_system_wait(task_system* ts)
{
  for (int i = 0; i < ts->thread_count; ++i) {
    task_queue_wait(&ts->queue[i]);
  }
}

