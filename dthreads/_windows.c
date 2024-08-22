// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: _windows.c
//    Date: 2024-08-12
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: DO NOT LINK TO THIS FILE DIRECTLY REFER TO README
// ***************************************************************************************

#include "_headers/common.h"
#include "dthread.h"

typedef struct
{
    void* result;
} DThreadResult;

DWORD WINAPI _dthread_winapi_function_wrapper(void* data)
{
    DThread* thread = (DThread*)data;
    thread->_result = thread->_func(thread->_data);

    return 0;
}

int dthread_create(DThread* thread, DThreadAttr* attr)
{
    dthread_debug("dthread_create");

    assert(thread && "`thread` cannot be NULL in dthread_create");

    if (attr)
        thread->handle = CreateThread(NULL, attr->stacksize ? attr->stacksize : 0, _dthread_winapi_function_wrapper, thread, attr->dwCreationFlags ? (DWORD)attr->dwCreationFlags : 0, NULL);
    else
        thread->handle = CreateThread(NULL, 0, _dthread_winapi_function_wrapper, thread, 0, NULL);

    return thread->handle == NULL;
}

int dthread_detach(DThread* thread)
{
    dthread_debug("dthread_detach");

    return !CloseHandle(thread->handle);
}

int dthread_join(DThread* thread)
{
    dthread_debug("dthread_join");

    DWORD wait_result = WaitForSingleObject(thread->handle, INFINITE);
    if (wait_result != WAIT_OBJECT_0)
    {
        dthread_debug_args("dthread_join: WaitForSingleObject failed, result: %lu", wait_result);
        return 1;
    }

    // 👉 NOTE by @dezashibi
    // Remember that the result is already stored in thread->_result by the thread function
    // You can access it after this function returns

    return !CloseHandle(thread->handle);
}

int dthread_equal(DThread* thread1, DThread* thread2)
{
    dthread_debug("dthread_equal");

    return thread1->handle == thread2->handle;
}

_DThreadHandle dthread_self(void)
{
    dthread_debug("dthread_self");

    return GetCurrentThread();
}

dthread_ulong_t dthread_id(DThread* thread)
{
    dthread_debug("dthread_id");

    return GetThreadId(thread->handle);
}

void dthread_exit(void* code)
{
    dthread_debug("dthread_exit");

#if (defined(__WATCOMC__) || defined(_MSC_VER) || defined(__DMC__))
    ExitThread((DWORD)code);
#else
    ExitThread((DWORD)(uintptr_t)code);
#endif
}

int dthread_cancel(DThread* thread)
{
    dthread_debug("dthread_cancel");

    return !TerminateThread(thread->handle, 0);
}

int dthread_mutex_init(DThreadMutex* mutex, DThreadMutexAttr* attr)
{
    dthread_debug("dthread_mutex_init");

    (void)attr;

    InitializeCriticalSection(&mutex->handle);

    return 0;
}

int dthread_mutex_lock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_lock");

    EnterCriticalSection(&mutex->handle);

    return 0;
}

int dthread_mutex_trylock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_trylock");

    return !TryEnterCriticalSection(&mutex->handle);
}

int dthread_mutex_unlock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_unlock");

    LeaveCriticalSection(&mutex->handle);

    return 0;
}

int dthread_mutex_destroy(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_destroy");

    DeleteCriticalSection(&mutex->handle);

    return 0;
}

int dthread_cond_init(DThreadCond* cond, DThreadCondAttr* attr)
{
    dthread_debug("dthread_cond_init");

    (void)attr;

    InitializeConditionVariable(&cond->handle);

    return 0;
}

int dthread_cond_signal(DThreadCond* cond)
{
    dthread_debug("dthread_cond_signal");

    WakeConditionVariable(&cond->handle);

    return 0;
}

int dthread_cond_broadcast(DThreadCond* cond)
{
    dthread_debug("dthread_cond_broadcast");

    WakeAllConditionVariable(&cond->handle);

    return 0;
}

int dthread_cond_destroy(DThreadCond* cond)
{
    dthread_debug("dthread_cond_destroy");

    (void)cond;

    return 0;
}

int dthread_cond_wait(DThreadCond* cond, DThreadMutex* mutex)
{
    dthread_debug("dthread_cond_wait");

    return !SleepConditionVariableCS(&cond->handle, &mutex->handle, INFINITE);
}

int dthread_rwlock_init(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_init");

    rwlock->handle = malloc(sizeof(SRWLOCK));

    if (!rwlock->handle)
        return 1;

    InitializeSRWLock(rwlock->handle);

    return 0;
}

int dthread_rwlock_rdlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_rdlock");

    AcquireSRWLockShared(rwlock->handle);

    rwlock->type = 1;

    return 0;
}

int dthread_rwlock_unlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_unlock");

    switch (rwlock->type)
    {
    case 1:
    {
        ReleaseSRWLockShared(rwlock->handle);

        break;
    }

    case 2:
    {
        ReleaseSRWLockExclusive(rwlock->handle);

        break;
    }
    }

    rwlock->type = 0;

    return 0;
}

int dthread_rwlock_wrlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_wrlock");

    AcquireSRWLockExclusive(rwlock->handle);

    rwlock->type = 2;

    return 0;
}

int dthread_rwlock_destroy(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_destroy");

    free(rwlock->handle);
    rwlock->handle = NULL;
    rwlock->type = 0;

    return 0;
}

void dthread_barrier_init(DThreadBarrier* barrier, int num_threads)
{
    dthread_debug("dthread_barrier_init");

    InitializeCriticalSection(&barrier->cs);
    InitializeConditionVariable(&barrier->cv);
    barrier->count = num_threads;
    barrier->waiting = 0;
    barrier->num_threads = num_threads;
}

void dthread_barrier_wait(DThreadBarrier* barrier)
{
    dthread_debug("dthread_barrier_wait");

    EnterCriticalSection(&barrier->cs);
    barrier->waiting++;

    if (barrier->waiting < barrier->count)
    {
        SleepConditionVariableCS(&barrier->cv, &barrier->cs, INFINITE);
    }
    else
    {
        barrier->waiting = 0;
        WakeAllConditionVariable(&barrier->cv);
    }

    LeaveCriticalSection(&barrier->cs);
}

void dthread_barrier_destroy(DThreadBarrier* barrier)
{
    dthread_debug("dthread_barrier_destroy");

    DeleteCriticalSection(&barrier->cs);
}

int dthread_semaphore_init(DThreadSemaphore* semaphore, dthread_uint_t initial_value)
{
    dthread_debug("dthread_semaphore_init");

    semaphore->handle = CreateSemaphore(NULL, initial_value, LONG_MAX, NULL);
    return semaphore->handle ? 0 : -1;
}

int dthread_semaphore_wait(DThreadSemaphore* semaphore)
{
    dthread_debug("dthread_semaphore_wait");

    return WaitForSingleObject(semaphore->handle, INFINITE) == WAIT_OBJECT_0 ? 0 : -1;
}

int dthread_semaphore_post(DThreadSemaphore* semaphore)
{
    dthread_debug("dthread_semaphore_post");

    return ReleaseSemaphore(semaphore->handle, 1, NULL) ? 0 : -1;
}

int dthread_semaphore_destroy(DThreadSemaphore* semaphore)
{
    dthread_debug("dthread_semaphore_destroy");

    return CloseHandle(semaphore->handle) ? 0 : -1;
}
