// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: posix.h
//    Date: 2024-08-12
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: POSIX header file for dthreads library, this is not to be used in your
// *               library directly.
// ***************************************************************************************

#ifndef DTHREAD_POSIX_H_

#define DTHREAD_POSIX_H_

#include "api.h"

#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>

#if defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 700)
#define DTHREAD_MUTEX_ROBUST_AND_COND_CLOCK_AVAILABLE
#endif

typedef pthread_t _DThreadHandle;

typedef struct DThreadAttr
{
    size_t stacksize;
    void* stackaddr;
    int detachstate;
    size_t guardsize;
    int inheritsched;
    int schedpolicy;
    int scope;
    size_t stack;

} DThreadAttr;

typedef struct DThreadMutex
{
    pthread_mutex_t handle;
} DThreadMutex;

typedef struct DThreadMutexAttr
{
    int pshared;
    int type;
#ifdef DTHREAD_MUTEX_ROBUST_AND_COND_CLOCK_AVAILABLE
    int robust;
#endif
    int protocol;
    int prioceiling;
} DThreadMutexAttr;

typedef struct DThreadCond
{
    pthread_cond_t handle;
} DThreadCond;

typedef struct DThreadCondAttr
{
    int pshared;
#ifdef DTHREAD_MUTEX_ROBUST_AND_COND_CLOCK_AVAILABLE
    int clock;
#endif
} DThreadCondAttr;

typedef struct DThreadRWLock
{
    pthread_rwlock_t handle;
} DThreadRWLock;

typedef struct DThreadBarrier
{
#ifdef __APPLE__
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int trip_count;
#else
    pthread_barrier_t handle;
#endif
    int num_threads;
} DThreadBarrier;

typedef struct DThreadSemaphore
{
#ifdef __APPLE__
    sem_t* handle;
#else
    sem_t handle;
#endif
} DThreadSemaphore;

#endif // DTHREAD_POSIX_H_
