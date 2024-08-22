// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: random.h
//    Date: 2024-08-15
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Tread Safe Randomization header file for dthreads library, this is not
// *               to be used in your library directly.
// ***************************************************************************************

#ifndef DTHREAD_RANDOM_H_
#define DTHREAD_RANDOM_H_

#include "api.h"

#if defined(_WIN32) || defined(_WIN64)
#include <synchapi.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <stdlib.h>
#include <time.h>

/**
 * @brief Mutex for ensuring thread-safe access to the random number generator.
 *
 * This static mutex is used to synchronize access to the random number generator,
 * ensuring that only one thread can generate a random number at a time.
 * On Windows, a CRITICAL_SECTION is used, while on POSIX systems, a pthread mutex is used.
 */
#if defined(_WIN32) || defined(_WIN64)
static CRITICAL_SECTION dthread_rng_mutex;
#else
static pthread_mutex_t dthread_rng_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

/**
 * @brief Initializes the mutex for thread-safe random number generation.
 *
 * This function initializes the mutex used for ensuring that random number generation
 * is thread-safe. It must be called before any threads start generating random numbers.
 * On Windows, it initializes a CRITICAL_SECTION, and on POSIX systems, it initializes a pthread mutex.
 */
DTHREAD_API void dthread_rng_init(void);

/**
 * @brief Cleans up the mutex used for thread-safe random number generation.
 *
 * This function destroys the mutex used for ensuring thread-safe random number generation.
 * It should be called after all random number generation tasks are complete, typically
 * at the end of the program.
 */
DTHREAD_API void dthread_rng_cleanup(void);

/**
 * @brief Generates a thread-safe random number.
 *
 * This function generates a random number in a thread-safe manner. It locks the mutex
 * before generating the random number and unlocks it after the number is generated,
 * ensuring that no two threads can generate a random number simultaneously.
 *
 * @return int A random number generated using the standard rand() function.
 */
DTHREAD_API int dthread_rng_random(void);

/**
 * @macro dthread_rng_seed_maker
 * @brief Seeds the random number generator with a unique seed.
 *
 * This macro seeds the random number generator using a combination of the current time
 * and the thread ID, ensuring that each thread has a different random seed. This helps
 * in generating different random sequences across different threads.
 *
 * Usage of this macro should be done in each thread before calling dthread_rng_random().
 */
#define dthread_rng_seed_maker() srand((dthread_uint_t)(((dthread_ulong_t)time(NULL)) ^ ((uintptr_t)dthread_self())))

#endif // DTHREAD_RANDOM_H_
