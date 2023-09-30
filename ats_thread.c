#include <winbase.h>

void atomic_store(int* out, int n) {
  InterlockedExchange(out, n);
}

int atomic_load(const int* value) {
  return InterlockedOr((int*)value, 0); // shady stuff!
}

void atomic_inc(int* value) {
  InterlockedIncrement(value);
}

void atomic_dec(int* value) {
  InterlockedDecrement(value);
}

int atomic_add(int* out, int n) {
  return InterlockedExchangeAdd(out, n);
}

int atomic_sub(int* out, int n) {
  return InterlockedExchangeAdd(out, -n);
}

int atomic_or(int* out, int n) {
  return InterlockedOr(out, n);
}

int atomic_and(int* out, int n) {
  return InterlockedAnd(out, n);
}

int atomic_xor(int* out, int n) {
  return InterlockedXor(out, n);
}

// ----------------------------------------- threads ------------------------------------------ //

thread thread_create(thread_proc* proc, void* args) {
  thread td = {0};
  td.id = CreateThread(NULL, 0, proc, args, 0, NULL);
  return td;
}

int thread_join(thread* td) {
  WaitForSingleObject(td->id, INFINITE);
  DWORD exit_code = 0;
  GetExitCodeThread(td->id, &exit_code);
  return exit_code;
}

void thread_destroy(thread* td) {
  CloseHandle(td->id);
}

void thread_yield(void) {
  SwitchToThread();
}

void thread_sleep(u32 milliseconds) {
  Sleep(milliseconds);
}

void thread_exit(int exit_code) {
  ExitThread(exit_code);
}

//----------------------------------------- task queue -------------------------------------------- //

int task_queue_thread(void* data) {
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

void task_queue_init(task_queue* queue) {
  memset(queue, 0, sizeof *queue);
  queue->thread = thread_create(task_queue_thread, queue);
}

void task_queue_push(task_queue* queue, task_proc_t* proc, void* data) {
  queue->array[queue->end] = (task_data) {
    .proc = proc,
    .data = data,
  };

  atomic_inc(&queue->end);
  atomic_and(&queue->end, TASK_QUEUE_MOD);
}

void task_queue_wait(task_queue* queue) {
  while (queue->begin != queue->end) {
    thread_yield();
  }
}

//----------------------------------------- task queue -------------------------------------------- //

void task_system_init(task_system* ts, u32 thread_count) {
  if (!thread_count) {
    struct _SYSTEM_INFO info = {0};
    GetSystemInfo(&info);
    ts->thread_count = clamp_max(info.dwNumberOfProcessors, TASK_SYSTEM_MAX_THREADS);
  } else {
    ts->thread_count = thread_count;
  }

  for (int i = 0; i < ts->thread_count; ++i) {
    task_queue_init(&ts->queue[i]);
  }
}

void task_system_push(task_system* ts, task_proc_t* proc, void* data) {
  task_queue_push(&ts->queue[ts->next], proc, data);
  ts->next = (ts->next + 1) % ts->thread_count;
}

void task_system_wait(task_system* ts) {
  for (int i = 0; i < ts->thread_count; ++i) {
    task_queue_wait(&ts->queue[i]);
  }
}

