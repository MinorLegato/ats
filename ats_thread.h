#pragma once

// ----------------------------------------- threads ------------------------------------------ //

typedef struct {
  void* id;
} thread;

typedef unsigned long thread_proc(void* args);

//----------------------------------------- task queue -------------------------------------------- //

typedef unsigned long task_proc_t(void* data);

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

//----------------------------------------- task queue -------------------------------------------- //

#define TASK_SYSTEM_MAX_THREADS (32)

typedef struct {
  u32 thread_count;
  int next;
  task_queue queue[TASK_SYSTEM_MAX_THREADS];
} task_system;

// ===================================== INTERNAL ===================================== //

#include <winbase.h>

static void atomic_store(long* out, int n)
{
  InterlockedExchange(out, n);
}

static int atomic_load(const long* value)
{
  return InterlockedOr((long*)value, 0); // shady stuff!
}

static void atomic_inc(long* value)
{
  InterlockedIncrement(value);
}

static void atomic_dec(long* value)
{
  InterlockedDecrement(value);
}

static int atomic_add(long* out, long n)
{
  return InterlockedExchangeAdd(out, n);
}

static int atomic_sub(long* out, long n)
{
  return InterlockedExchangeAdd(out, -n);
}

static int atomic_or(long* out, long n)
{
  return InterlockedOr(out, n);
}

static int atomic_and(long* out, long n)
{
  return InterlockedAnd(out, n);
}

static int atomic_xor(long* out, long n)
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

static unsigned long task_queue_thread(void* data)
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

