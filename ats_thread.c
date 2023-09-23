#include <winbase.h>

void
atomic_store(int* out, int n) {
  InterlockedExchange(out, n);
}

int
atomic_load(const int* value) {
  return InterlockedOr((int*)value, 0); // shady stuff!
}

void
atomic_inc(int* value) {
  InterlockedIncrement(value);
}

void
atomic_dec(int* value) {
  InterlockedDecrement(value);
}

int
atomic_add(int* out, int n) {
  return InterlockedExchangeAdd(out, n);
}

int
atomic_sub(int* out, int n) {
  return InterlockedExchangeAdd(out, -n);
}

int
atomic_or(int* out, int n) {
  return InterlockedOr(out, n);
}

int
atomic_and(int* out, int n) {
  return InterlockedAnd(out, n);
}

int
atomic_xor(int* out, int n) {
  return InterlockedXor(out, n);
}

// ----------------------------------------- threads ------------------------------------------ //

struct thread
thread_create(thread_proc_t* proc, void* args) {
  struct thread thread = {0};
  thread.id = CreateThread(NULL, 0, proc, args, 0, NULL);
  return thread;
}

int
thread_join(struct thread* thread) {
  WaitForSingleObject(thread->id, INFINITE);
  DWORD exit_code = 0;
  GetExitCodeThread(thread->id, &exit_code);
  return exit_code;
}

void
thread_destroy(struct thread* thread) {
  CloseHandle(thread->id);
}

void
thread_yield(void) {
  SwitchToThread();
}

void
thread_sleep(unsigned milliseconds) {
  Sleep(milliseconds);
}

void
thread_exit(int exit_code) {
  ExitThread(exit_code);
}

//----------------------------------------- task queue -------------------------------------------- //

int
task_queue_thread(void* data) {
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

void
task_queue_init(struct task_queue* queue) {
  memset(queue, 0, sizeof *queue);
  queue->thread = thread_create(task_queue_thread, queue);
}

void
task_queue_push(struct task_queue* queue, task_proc_t* proc, void* data) {
  queue->array[queue->end] = (struct task) {
    .proc = proc,
    .data = data,
  };

  atomic_inc(&queue->end);
  atomic_and(&queue->end, TASK_QUEUE_MOD);
}

void
task_queue_wait(struct task_queue* queue) {
  while (queue->begin != queue->end) {
    thread_yield();
  }
}

//----------------------------------------- task queue -------------------------------------------- //

void
task_system_init(struct task_system* ts, unsigned thread_count) {
  if (!thread_count) {
    struct _SYSTEM_INFO info = {0};
    GetSystemInfo(&info);
    ts->thread_count = min(info.dwNumberOfProcessors, TASK_SYSTEM_MAX_THREADS);
  } else {
    ts->thread_count = thread_count;
  }

  for (int i = 0; i < ts->thread_count; ++i) {
    task_queue_init(&ts->queue[i]);
  }
}

void
task_system_push(struct task_system* ts, task_proc_t* proc, void* data) {
  task_queue_push(&ts->queue[ts->next], proc, data);
  ts->next = (ts->next + 1) % ts->thread_count;
}

void
task_system_wait(struct task_system* ts) {
  for (int i = 0; i < ts->thread_count; ++i) {
    task_queue_wait(&ts->queue[i]);
  }
}

