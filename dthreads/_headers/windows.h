// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: windows.h
//    Date: 2024-08-12
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Windows header file for dthreads library, this is not to be used in your
// *               library directly.
// ***************************************************************************************

#ifndef DTHREAD_WINDOWS_H_

#define DTHREAD_WINDOWS_H_

#include "api.h"

#include <stddef.h>
#include <synchapi.h>
#include <windows.h>

typedef HANDLE _DThreadHandle;

typedef struct DThreadAttr
{
    size_t stacksize;
    int dwCreationFlags;
} DThreadAttr;

typedef struct DThreadMutex
{
    CRITICAL_SECTION handle;
} DThreadMutex;

typedef struct DThreadMutexAttr
{
    void* nothing;
} DThreadMutexAttr;

typedef struct DThreadCond
{
    CONDITION_VARIABLE handle;
} DThreadCond;

typedef struct DThreadCondAttr
{
    void* nothing;
} DThreadCondAttr;

typedef struct DThreadRWLock
{
    int type;
    PSRWLOCK handle;
} DThreadRWLock;

typedef struct DThreadBarrier
{
    CRITICAL_SECTION cs;
    CONDITION_VARIABLE cv;
    int count;
    int waiting;
    int num_threads;
} DThreadBarrier;

typedef struct DThreadSemaphore
{
    HANDLE handle;
} DThreadSemaphore;

#endif // DTHREAD_WINDOWS_H_
