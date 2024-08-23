// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: dthread.h
//    Date: 2024-08-12
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description:
// *  `dthreads` is a cross-platform threading library in C that abstracts the underlying
// *  threading mechanisms of different operating systems. This header provides the API
// *  definitions for thread management, synchronization primitives like mutexes and
// *  condition variables, and other threading utilities.
// ***************************************************************************************

#ifndef DTHREAD_H_

#define DTHREAD_H_

#include "_headers/api.h"

#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if (defined(_WIN32) || defined(_WIN64))

#include "_headers/windows.h"

#else

#include "_headers/posix.h"

#endif

    /**
     * @typedef DThreadRoutine
     * @brief Defines a function pointer type for thread routines.
     *
     * A thread routine is a function that accepts a single void* argument and returns a void*.
     */
    typedef void* (*DThreadRoutine)(void*);

/**
 * @macro dthread_define_routine
 * @brief Simplifies the definition of thread routines.
 *
 * This macro helps define a thread routine that is compatible with the DThreads library.
 *
 * @param NAME The name of the thread routine function.
 */
#define dthread_define_routine(NAME) void* NAME(void* data)

#ifdef DTHREAD_DEBUG
/**
 * @macro dthread_debug
 * @brief Outputs debug information if DTHREAD_DEBUG is defined.
 *
 * When DTHREAD_DEBUG is defined, this macro prints the provided message to the console.
 * Otherwise, it does nothing.
 *
 * @param X The message to print for debugging purposes.
 *
 * NOTE: Adds "\\n" automatically
 */
#define dthread_debug(X) puts(X)

/**
 * @macro dthread_debug_args
 * @brief Outputs debug information with passed arguments if DTHREAD_DEBUG is defined.
 *
 * When DTHREAD_DEBUG is defined, this macro prints the provided message with passed args
 * to the console Otherwise, it does nothing.
 *
 * @param X The message to print for debugging purposes.
 * @param ... The rest of arguments for passing to `printf`.
 *
 * NOTE: Adds "\\n" automatically
 */
#define dthread_debug_args(X, ...) printf(X "\n", __VA_ARGS__)
#else
#define dthread_debug(X)
#define dthread_debug_args(X, ...)
#endif

    /**
     * @struct DThread
     * @brief Configuration structure for creating threads.
     *
     * This structure holds the function pointer to the thread routine and its data.
     * It also holds the void pointer to the result
     */
    typedef struct
    {
        DThreadRoutine _func;
        void* _data;
        void* _result;

        _DThreadHandle handle;

    } DThread;

/**
 * @macro dthread_init_thread
 * @brief Initializes a DThread structure.
 *
 * This macro simplifies the initialization of a DThread structure by allowing you
 * to specify the thread routine and its arguments.
 *
 * @param FUNC The thread routine function.
 * @param DATA The data to pass to the thread routine.
 */
#define dthread_init_thread(FUNC, DATA) \
    (DThread)                           \
    {                                   \
        ._func = FUNC,                  \
        ._data = (void*)(DATA),         \
        ._result = NULL                 \
    }

/**
 * @macro dthread_set_data
 * @brief sets thread data for given thread reference
 *
 * @param THREAD_PTR The reference (pointer) to the thread.
 * @param T The type the conversion must be done (type of data); must be pointer.
 * @param DATA The data to pass to the thread routine to be set.
 */
#define dthread_set_data(THREAD_PTR, T, DATA) \
    *(T)(THREAD_PTR)->_data = DATA

/**
 * @macro dthread_set_func
 * @brief sets thread routine for given thread reference
 *
 * @param THREAD_PTR The reference (pointer) to the thread.
 * @param FUNC The routine pointer to be set.
 */
#define dthread_set_func(THREAD_PTR, FUNC) THREAD_PTR->_func = FUNC

/**
 * @macro dthread_get_result
 * @brief gets thread routine result after the thread join completes for given thread reference
 *
 * @param THREAD_PTR The reference (pointer) to the thread.
 */
#define dthread_get_result(THREAD_PTR) ((THREAD_PTR)->_result)

/**
 * @macro dthread_get_result_as
 * @brief gets thread routine result after the thread join completes for given thread reference
 * and casting it to given type (must be a pointer type)
 *
 * @param THREAD_PTR The reference (pointer) to the thread.
 * @param T The pointer type for casting the _result field.
 */
#define dthread_get_result_as(THREAD_PTR, T) ((T)((THREAD_PTR)->_result))

    /**
     * @brief Creates a new thread.
     *
     * This function creates a new thread using the internal thread configuration and given attributes.
     *
     * @param thread A pointer to the DThread structure to be initialized.
     * @param attr Optional thread attributes; can be NULL for default attributes.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_create(DThread* thread, DThreadAttr* attr);

    /**
     * @brief Detaches a thread, allowing it to run independently.
     *
     * Once a thread is detached, it cannot be joined. It will automatically clean up its resources when it terminates.
     *
     * @param thread The pointer to the thread to detach.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_detach(DThread* thread);

    /**
     * @brief Waits for a thread to complete.
     *
     * This function blocks the calling thread until the specified thread terminates.
     *
     * @param thread The pointer to the thread to wait for.
     * @return 0 on success, non-zero on failure.
     *
     * NOTE: On Windows, the return code from the thread routine is already saved in the DThread's _result
     * passed to the dthread_create; might be NULL.
     */
    DTHREAD_API int dthread_join(DThread* thread);

    /**
     * @brief Compares two threads for equality.
     *
     * This function checks whether two threads are equal (i.e., represent the same thread).
     *
     * @param thread1 The first pointer to the thread to compare.
     * @param thread2 The second pointer to the thread to compare.
     * @return Non-zero if the threads are equal, zero otherwise.
     */
    DTHREAD_API int dthread_equal(DThread* thread1, DThread* thread2);

    /**
     * @brief Returns the calling thread.
     *
     * This function returns a DThread structure representing the current thread.
     *
     * @return A DThread structure representing the current thread.
     */
    DTHREAD_API uintptr_t dthread_self(void);

    /**
     * @brief Returns the ID of a thread.
     *
     * This function returns the unique identifier of the specified thread.
     *
     * @param thread The pointer to the thread whose ID to retrieve.
     * @return The thread's ID as an uintptr_t.
     */
    DTHREAD_API uintptr_t dthread_id(DThread* thread);

    /**
     * @brief Exits the calling thread.
     *
     * This function terminates the calling thread and optionally returns a value to the thread that joined it.
     *
     * @param code The return code to pass to the joining thread.
     */
    DTHREAD_API void dthread_exit(void* code);

    /**
     * @brief Cancels a thread.
     *
     * This function sends a cancellation request to the specified thread.
     *
     * @param thread The pointer to the thread to cancel.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_cancel(DThread* thread);

    /**
     * @brief Initializes a mutex.
     *
     * This function initializes a mutex with optional attributes.
     *
     * @param mutex A pointer to the mutex to initialize.
     * @param attr Optional mutex attributes; can be NULL for default attributes.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_mutex_init(DThreadMutex* mutex, DThreadMutexAttr* attr);

    /**
     * @brief Locks a mutex.
     *
     * This function locks the specified mutex, blocking the calling thread if necessary.
     *
     * @param mutex A pointer to the mutex to lock.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_mutex_lock(DThreadMutex* mutex);

    /**
     * @brief Attempts to lock a mutex without blocking.
     *
     * This function tries to lock the specified mutex. If the mutex is already locked, the function returns immediately.
     *
     * @param mutex A pointer to the mutex to try locking.
     * @return 0 if the mutex was successfully locked, non-zero if it was already locked.
     */
    DTHREAD_API int dthread_mutex_trylock(DThreadMutex* mutex);

    /**
     * @brief Unlocks a mutex.
     *
     * This function unlocks the specified mutex, allowing other threads to lock it.
     *
     * @param mutex A pointer to the mutex to unlock.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_mutex_unlock(DThreadMutex* mutex);

    /**
     * @brief Destroys a mutex.
     *
     * This function destroys the specified mutex, releasing any resources it holds.
     *
     * @param mutex A pointer to the mutex to destroy.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_mutex_destroy(DThreadMutex* mutex);

    /**
     * @brief Initializes a condition variable.
     *
     * This function initializes a condition variable with optional attributes.
     *
     * @param cond A pointer to the condition variable to initialize.
     * @param attr Optional condition variable attributes; can be NULL for default attributes.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_cond_init(DThreadCond* cond, DThreadCondAttr* attr);

    /**
     * @brief Signals a condition variable, waking one waiting thread.
     *
     * This function unblocks one of the threads currently waiting on the specified condition variable.
     *
     * @param cond A pointer to the condition variable to signal.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_cond_signal(DThreadCond* cond);

    /**
     * @brief Broadcasts a condition variable, waking all waiting threads.
     *
     * This function unblocks all threads currently waiting on the specified condition variable.
     *
     * @param cond A pointer to the condition variable to broadcast.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_cond_broadcast(DThreadCond* cond);

    /**
     * @brief Destroys a condition variable.
     *
     * This function destroys the specified condition variable, releasing any resources it holds.
     *
     * @param cond A pointer to the condition variable to destroy.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_cond_destroy(DThreadCond* cond);

    /**
     * @brief Waits on a condition variable.
     *
     * This function atomically releases the specified mutex and blocks the calling thread until the condition variable is signaled.
     *
     * @param cond A pointer to the condition variable to wait on.
     * @param mutex A pointer to the mutex associated with the condition variable.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_cond_wait(DThreadCond* cond, DThreadMutex* mutex);

    /**
     * @brief Initializes a read-write lock.
     *
     * This function initializes a read-write lock, allowing multiple threads to read or one thread to write at a time.
     *
     * @param rwlock A pointer to the read-write lock to initialize.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_rwlock_init(DThreadRWLock* rwlock);

    /**
     * @brief Acquires a read lock on a read-write lock.
     *
     * This function locks the read-write lock for reading, allowing multiple threads to acquire read locks simultaneously.
     *
     * @param rwlock A pointer to the read-write lock to lock.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_rwlock_rdlock(DThreadRWLock* rwlock);

    /**
     * @brief Unlocks a read-write lock.
     *
     * This function releases a previously acquired read or write lock on the read-write lock.
     *
     * @param rwlock A pointer to the read-write lock to unlock.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_rwlock_unlock(DThreadRWLock* rwlock);

    /**
     * @brief Acquires a write lock on a read-write lock.
     *
     * This function locks the read-write lock for writing, allowing only one thread to acquire the write lock at a time.
     *
     * @param rwlock A pointer to the read-write lock to lock.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_rwlock_wrlock(DThreadRWLock* rwlock);

    /**
     * @brief Destroys a read-write lock.
     *
     * This function destroys the specified read-write lock, releasing any resources it holds.
     *
     * @param rwlock A pointer to the read-write lock to destroy.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_rwlock_destroy(DThreadRWLock* rwlock);

    /**
     * @brief Initializes a barrier.
     *
     * This function initializes a barrier for a specified number of threads. All threads must reach the barrier before any can proceed.
     *
     * @param barrier A pointer to the barrier to initialize.
     * @param num_threads The number of threads required to reach the barrier.
     */
    DTHREAD_API void dthread_barrier_init(DThreadBarrier* barrier, int num_threads);

    /**
     * @brief Waits at a barrier.
     *
     * This function blocks the calling thread until the specified number of threads have reached the barrier.
     *
     * @param barrier A pointer to the barrier to wait on.
     */
    DTHREAD_API void dthread_barrier_wait(DThreadBarrier* barrier);

    /**
     * @brief Destroys a barrier.
     *
     * This function destroys the specified barrier, releasing any resources it holds.
     *
     * @param barrier A pointer to the barrier to destroy.
     */
    DTHREAD_API void dthread_barrier_destroy(DThreadBarrier* barrier);

    /**
     * @brief Initializes a semaphore.
     *
     * This function initializes a semaphore with the specified initial value.
     *
     * @param semaphore A pointer to the semaphore to initialize.
     * @param initial_value The initial value of the semaphore.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_semaphore_init(DThreadSemaphore* semaphore, uint32_t initial_value);

    /**
     * @brief Waits on a semaphore.
     *
     * This function decrements the semaphore value, blocking the calling thread if the value is zero until another thread increments it.
     *
     * @param semaphore A pointer to the semaphore to wait on.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_semaphore_wait(DThreadSemaphore* semaphore);

    /**
     * @brief Posts to a semaphore.
     *
     * This function increments the semaphore value, potentially unblocking a waiting thread.
     *
     * @param semaphore A pointer to the semaphore to post to.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_semaphore_post(DThreadSemaphore* semaphore);

    /**
     * @brief Destroys a semaphore.
     *
     * This function destroys the specified semaphore, releasing any resources it holds.
     *
     * @param semaphore A pointer to the semaphore to destroy.
     * @return 0 on success, non-zero on failure.
     */
    DTHREAD_API int dthread_semaphore_destroy(DThreadSemaphore* semaphore);

#include "_headers/random.h"
#ifdef __cplusplus
}
#endif

#ifdef DTHREAD_IMPL
#if (defined(_WIN32) || defined(_WIN64))

#include "_windows.c"

#else

#include "_posix.c"

#endif

#include "_random.c"

#endif

#endif // DTHREAD_H_
