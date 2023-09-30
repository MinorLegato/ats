#pragma once

void atomic_store(int* out, int n);

int atomic_load(const int* value);

void atomic_inc(int* value);
void atomic_dec(int* value);

int atomic_add(int* out, int n);
int atomic_sub(int* out, int n);

int atomic_or(int* out, int n);
int atomic_and(int* out, int n);
int atomic_xor(int* out, int n);

// ----------------------------------------- threads ------------------------------------------ //

typedef struct {
  void* id;
} thread;

typedef int thread_proc(void* args);

thread thread_create(thread_proc* proc, void* args);

int  thread_join(thread* td);
void thread_destroy(thread* td);
void thread_yield(void);
void thread_sleep(u32 milliseconds);
void thread_exit(int exit_code);

//----------------------------------------- task queue -------------------------------------------- //

typedef void task_proc_t(void* data);

typedef struct {
  task_proc_t* proc;
  void* data;
} task_data;

#define TASK_QUEUE_MAX (64)
#define TASK_QUEUE_MOD (63)

typedef struct {
  thread    thread;

  int       begin;
  int       end;
  task_data array[TASK_QUEUE_MAX];
} task_queue;

int task_queue_thread(void* data);
void task_queue_init(task_queue* queue);
void task_queue_push(task_queue* queue, task_proc_t* proc, void* data);
void task_queue_wait(task_queue* queue);

//----------------------------------------- task queue -------------------------------------------- //

#define TASK_SYSTEM_MAX_THREADS (32)

typedef struct {
  int         thread_count;
  int         next;
  task_queue  queue[TASK_SYSTEM_MAX_THREADS];
} task_system;

void task_system_init(task_system* ts, u32 thread_count);
void task_system_push(task_system* ts, task_proc_t* proc, void* data);
void task_system_wait(task_system* ts);

