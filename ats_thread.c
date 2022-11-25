#include <winbase.h>

extern void atomic_store(int* out, int n) {
  InterlockedExchange(out, n);
}

extern int atomic_load(const int* value) {
  return InterlockedOr((int*)value, 0); // shady stuff!
}

extern void atomic_inc(int* value) {
  InterlockedIncrement(value);
}

extern void atomic_dec(int* value) {
  InterlockedDecrement(value);
}

extern int atomic_add(int* out, int n) {
  return InterlockedExchangeAdd(out, n);
}

extern int atomic_sub(int* out, int n) {
  return InterlockedExchangeAdd(out, -n);
}

extern int atomic_or(int* out, int n) {
  return InterlockedOr(out, n);
}

extern int atomic_and(int* out, int n) {
  return InterlockedAnd(out, n);
}

extern int atomic_xor(int* out, int n) {
  return InterlockedXor(out, n);
}

// ----------------------------------------- threads ------------------------------------------ //

extern struct thread thread_create(thread_proc_t* proc, void* args) {
  struct thread thread = {0};
  thread.id = CreateThread(NULL, 0, proc, args, 0, NULL);
  return thread;
}

extern int thread_join(struct thread* thread) {
  WaitForSingleObject(thread->id, INFINITE);
  DWORD exit_code = 0;
  GetExitCodeThread(thread->id, &exit_code);
  return exit_code;
}

extern void thread_destroy(struct thread* thread) {
  CloseHandle(thread->id);
}

extern void thread_yield(void) {
  SwitchToThread();
}

extern void thread_sleep(unsigned milliseconds) {
  Sleep(milliseconds);
}

extern void thread_exit(int exit_code) {
  ExitThread(exit_code);
}

//----------------------------------------- task queue -------------------------------------------- //

extern int task_queue_thread(void* data) {
  struct task_queue* queue = data;

  while (true) {
    if (queue->begin == queue->end) {
      thread_yield();
    } else {
      struct task* task = queue->array + queue->begin;
      task->proc(task->data);

      atomic_inc(&queue->begin);
      atomic_and(&queue->begin, TASK_QUEUE_MOD);
    }
  }

  return 0;
}

extern void task_queue_init(struct task_queue* queue) {
  memset(queue, 0, sizeof *queue);
  queue->thread = thread_create(task_queue_thread, queue);
}

extern void task_queue_push(struct task_queue* queue, task_proc_t* proc, void* data) {
  queue->array[queue->end] = (struct task) {
    .proc = proc,
    .data = data,
  };

  atomic_inc(&queue->end);
  atomic_and(&queue->end, TASK_QUEUE_MOD);
}

extern void task_queue_wait(struct task_queue* queue) {
  while (queue->begin != queue->end) {
    thread_yield();
  }
}

//----------------------------------------- task queue -------------------------------------------- //

extern void task_system_init(struct task_system* ts, u32 thread_count) {
  if (!thread_count) {
    struct _SYSTEM_INFO info = {0};
    GetSystemInfo(&info);
    ts->thread_count = ClampMax(info.dwNumberOfProcessors, TASK_SYSTEM_MAX_THREADS);
  } else {
    ts->thread_count = thread_count;
  }

  for (int i = 0; i < ts->thread_count; ++i) {
    task_queue_init(&ts->queue[i]);
  }
}

extern void task_system_push(struct task_system* ts, task_proc_t* proc, void* data) {
  task_queue_push(&ts->queue[ts->next], proc, data);
  ts->next = (ts->next + 1) % ts->thread_count;
}

extern void task_system_wait(struct task_system* ts) {
  for (int i = 0; i < ts->thread_count; ++i) {
    task_queue_wait(&ts->queue[i]);
  }
}

