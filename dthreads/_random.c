// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: _random.c
//    Date: 2024-08-15
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

#include "_headers/random.h"

void dthread_rng_init(void)
{
    dthread_debug("dthread_rng_init");

#if defined(_WIN32) || defined(_WIN64)
    InitializeCriticalSection(&dthread_rng_mutex);
#else
    pthread_mutex_init(&dthread_rng_mutex, NULL);
#endif
}

void dthread_rng_cleanup(void)
{
    dthread_debug("dthread_rng_cleanup");

#if defined(_WIN32) || defined(_WIN64)
    DeleteCriticalSection(&dthread_rng_mutex);
#else
    pthread_mutex_destroy(&dthread_rng_mutex);
#endif
}

int dthread_rng_random(void)
{
    dthread_debug("dthread_rng_random");

    int random_number;

// Lock the mutex
#if defined(_WIN32) || defined(_WIN64)
    EnterCriticalSection(&dthread_rng_mutex);
#else
    pthread_mutex_lock(&dthread_rng_mutex);
#endif

    // Generate random number
    random_number = rand();

// Unlock the mutex
#if defined(_WIN32) || defined(_WIN64)
    LeaveCriticalSection(&dthread_rng_mutex);
#else
    pthread_mutex_unlock(&dthread_rng_mutex);
#endif

    return random_number;
}
