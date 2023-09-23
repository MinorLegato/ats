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

struct thread {
  void* id;
};

typedef int thread_proc_t(void* args);

struct thread thread_create(thread_proc_t* proc, void* args);

int thread_join(struct thread* thread);
void thread_destroy(struct thread* thread);
void thread_yield(void);
void thread_sleep(unsigned milliseconds);
void thread_exit(int exit_code);

//----------------------------------------- task queue -------------------------------------------- //

typedef void task_proc_t(void* data);

struct task {
  task_proc_t* proc;
  void* data;
};

#define TASK_QUEUE_MAX (64)
#define TASK_QUEUE_MOD (63)

struct task_queue {
  struct thread thread;

  int begin;
  int end;
  struct task array[TASK_QUEUE_MAX];
};

int task_queue_thread(void* data);
void task_queue_init(struct task_queue* queue);
void task_queue_push(struct task_queue* queue, task_proc_t* proc, void* data);
void task_queue_wait(struct task_queue* queue);

//----------------------------------------- task queue -------------------------------------------- //

#define TASK_SYSTEM_MAX_THREADS (32)

struct task_system {
  int thread_count;
  int next;
  struct task_queue queue[TASK_SYSTEM_MAX_THREADS];
};

void task_system_init(struct task_system* ts, unsigned thread_count);
void task_system_push(struct task_system* ts, task_proc_t* proc, void* data);
void task_system_wait(struct task_system* ts);

