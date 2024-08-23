// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: _posix.c
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

int dthread_create(DThread* thread, DThreadAttr* attr)
{
    dthread_debug("dthread_create");

    assert(thread && "`thread` cannot be NULL in dthread_create");

    pthread_attr_t p_attr;

    if (attr)
    {
        if (pthread_attr_init(&p_attr))
            return 1;

        if (attr->detachstate)
            pthread_attr_setdetachstate(&p_attr, attr->detachstate);

        if (attr->guardsize)
            pthread_attr_setguardsize(&p_attr, attr->guardsize);

        if (attr->inheritsched)
            pthread_attr_setinheritsched(&p_attr, attr->inheritsched);

        if (attr->schedpolicy)
            pthread_attr_setschedpolicy(&p_attr, attr->schedpolicy);

        if (attr->scope)
            pthread_attr_setscope(&p_attr, attr->scope);

        if (attr->stack)
            pthread_attr_setstack(&p_attr, attr->stackaddr, attr->stack);

        if (attr->stacksize)
            pthread_attr_setstacksize(&p_attr, attr->stacksize);
    }

    return pthread_create(&thread->handle, attr ? &p_attr : NULL, thread->_func, thread->_data);
}

int dthread_detach(DThread* thread)
{
    dthread_debug("dthread_detach");

    return pthread_detach(thread->handle);
}

int dthread_join(DThread* thread)
{
    dthread_debug("dthread_join");

    void* code = (void**)&thread->_result;

    return pthread_join(thread->handle, code);
}

int dthread_equal(DThread* thread1, DThread* thread2)
{
    dthread_debug("dthread_equal");

    return pthread_equal(thread1->handle, thread2->handle);
}

uintptr_t dthread_self(void)
{
    dthread_debug("dthread_self");

    return (uintptr_t)pthread_self();
}

uintptr_t dthread_id(DThread* thread)
{
    dthread_debug("dthread_id");

    return (uintptr_t)thread->handle;
}

void dthread_exit(void* code)
{
    dthread_debug("dthread_exit");

    pthread_exit(code);
}

int dthread_cancel(DThread* thread)
{
    dthread_debug("dthread_cancel");

    return pthread_cancel(thread->handle);
}

int dthread_mutex_init(DThreadMutex* mutex, DThreadMutexAttr* attr)
{
    dthread_debug("dthread_mutex_init");

    pthread_mutexattr_t p_attr;

    if (attr)
    {
        if (pthread_mutexattr_init(&p_attr))
            return 1;
        if (attr->pshared)
            pthread_mutexattr_setpshared(&p_attr, attr->pshared);

        if (attr->type)
            pthread_mutexattr_settype(&p_attr, attr->type);

#if DTHREAD_MUTEX_ROBUST_AND_COND_CLOCK_AVAILABLE
        if (attr->robust)
            pthread_mutexattr_setrobust(&p_attr, attr->robust);
#endif

        if (attr->protocol)
            pthread_mutexattr_setprotocol(&p_attr, attr->protocol);

        if (attr->prioceiling)
            pthread_mutexattr_setprioceiling(&p_attr, attr->prioceiling);
    }

    return pthread_mutex_init(&mutex->handle, attr ? &p_attr : NULL);
}

int dthread_mutex_lock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_lock");

    return pthread_mutex_lock(&mutex->handle);
}

int dthread_mutex_trylock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_trylock");

    return pthread_mutex_trylock(&mutex->handle);
}

int dthread_mutex_unlock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_unlock");

    return pthread_mutex_unlock(&mutex->handle);
}

int dthread_mutex_destroy(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_destroy");

    return pthread_mutex_destroy(&mutex->handle);
}

int dthread_cond_init(DThreadCond* cond, DThreadCondAttr* attr)
{
    dthread_debug("dthread_cond_init");

    pthread_condattr_t p_attr;

    if (attr)
    {
        if (pthread_condattr_init(&p_attr) != 0)
            return 1;

        if (attr->pshared)
            pthread_condattr_setpshared(&p_attr, attr->pshared);

#if DTHREAD_MUTEX_ROBUST_AND_COND_CLOCK_AVAILABLE
        if (attr->clock)
            pthread_condattr_setclock(&p_attr, attr->clock);
#endif
    }

    return pthread_cond_init(&cond->handle, attr ? &p_attr : NULL);
}

int dthread_cond_signal(DThreadCond* cond)
{
    dthread_debug("dthread_cond_signal");

    return pthread_cond_signal(&cond->handle);
}

int dthread_cond_broadcast(DThreadCond* cond)
{
    dthread_debug("dthread_cond_broadcast");

    return pthread_cond_broadcast(&cond->handle);
}

int dthread_cond_destroy(DThreadCond* cond)
{
    dthread_debug("dthread_cond_destroy");

    return pthread_cond_destroy(&cond->handle);
}

int dthread_cond_wait(DThreadCond* cond, DThreadMutex* mutex)
{
    dthread_debug("dthread_cond_wait");

    return pthread_cond_wait(&cond->handle, &mutex->handle);
}

int dthread_rwlock_init(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_init");

    return pthread_rwlock_init(&rwlock->handle, NULL);
}

int dthread_rwlock_rdlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_rdlock");

    return pthread_rwlock_rdlock(&rwlock->handle);
}

int dthread_rwlock_unlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_unlock");

    return pthread_rwlock_unlock(&rwlock->handle);
}

int dthread_rwlock_wrlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_wrlock");

    return pthread_rwlock_wrlock(&rwlock->handle);
}

int dthread_rwlock_destroy(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_destroy");

    return pthread_rwlock_destroy(&rwlock->handle);
}

void dthread_barrier_init(DThreadBarrier* barrier, int num_threads)
{
    dthread_debug("dthread_barrier_init");

#ifdef __APPLE__
    if (num_threads <= 0)
    {
        return;
    }

    if (pthread_mutex_init(&barrier->mutex, NULL) != 0)
    {
        return;
    }

    if (pthread_cond_init(&barrier->cond, NULL) != 0)
    {
        pthread_mutex_destroy(&barrier->mutex);
        return;
    }

    barrier->trip_count = num_threads;
    barrier->num_threads = 0;
#else
    pthread_barrier_init(&barrier->handle, NULL, num_threads);
    barrier->num_threads = num_threads;
#endif
}

void dthread_barrier_wait(DThreadBarrier* barrier)
{
    dthread_debug("dthread_barrier_wait");

#ifdef __APPLE__
    pthread_mutex_lock(&barrier->mutex);
    barrier->num_threads++;

    if (barrier->num_threads >= barrier->trip_count)
    {
        barrier->num_threads = 0;
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->mutex);
    }
    else
    {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
        pthread_mutex_unlock(&barrier->mutex);
    }
#else
    pthread_barrier_wait(&barrier->handle);
#endif
}

void dthread_barrier_destroy(DThreadBarrier* barrier)
{
    dthread_debug("dthread_barrier_destroy");

#ifdef __APPLE__
    pthread_cond_destroy(&barrier->cond);
    pthread_mutex_destroy(&barrier->mutex);
#else
    pthread_barrier_destroy(&barrier->handle);
#endif
}

int dthread_semaphore_init(DThreadSemaphore* semaphore, uint32_t initial_value)
{
    dthread_debug("dthread_semaphore_init");

#ifdef __APPLE__
    sem_unlink("/dthread_semaphore_osx");
    semaphore->handle = sem_open("/dthread_semaphore_osx", O_CREAT, 0644, initial_value);

    return semaphore->handle == SEM_FAILED;
#else
    return sem_init(&semaphore->handle, 0, initial_value);
#endif
}

int dthread_semaphore_wait(DThreadSemaphore* semaphore)
{
    dthread_debug("dthread_semaphore_wait");

#ifdef __APPLE__
    return sem_wait(semaphore->handle);
#else
    return sem_wait(&semaphore->handle);
#endif
}

int dthread_semaphore_post(DThreadSemaphore* semaphore)
{
    dthread_debug("dthread_semaphore_post");

#ifdef __APPLE__
    return sem_post(semaphore->handle);
#else
    return sem_post(&semaphore->handle);
#endif
}

int dthread_semaphore_destroy(DThreadSemaphore* semaphore)
{
    dthread_debug("dthread_semaphore_destroy");

#ifdef __APPLE__
    // Use sem_close and sem_unlink for named semaphores
    if (sem_close(semaphore->handle) == -1)
    {
        perror("sem_close failed");
        return -1;
    }

    if (sem_unlink("/dthread_semaphore_osx") == -1)
    {
        perror("sem_unlink failed");
        return -1;
    }

    return 0;
#else
    return sem_destroy(&semaphore->handle);
#endif
}
