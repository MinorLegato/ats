#pragma once

#define WIN32_LEAN_AND_MEAN
#define WIN32_MEAN_AND_LEAN
#include <windows.h>

typedef DWORD WINAPI (*Thread_Func)(LPVOID data);

#define THREAD_FUNC(func_name) DWORD WINAPI func_name(LPVOID data)

typedef struct Thread {
    DWORD       id;
    HANDLE      handle;
} Thread;

static Thread threadCreate(const Thread_Func thread_func, void* data) {
    Thread thread;

    thread.handle = CreateThread(NULL, 0, thread_func, data, 0, &thread.id);

    return thread;
}

static DWORD threadWait(Thread thread) {
    WaitForSingleObject(thread.handle, INFINITE);

    DWORD result = 0;

    GetExitCodeThread(thread.handle, &result);

    return result;
}


