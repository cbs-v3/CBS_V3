#ifndef CBS_UTIL_H
#define CBS_UTIL_H

#include <pthread.h>

typedef int pthread_spinlock_t //for mac,disable pthread_spinlock_t
#define spin_lock_init(lock) 

//typedef pthread_spinlock_t spinlock_t;
//#define spin_lock_init(lock) pthread_spin_init((lock), PTHREAD_PROCESS_SHARED)
#ifdef MULTI_THREAD
	#define spin_lock(lock) pthread_spin_lock(lock)
	#define spin_unlock(lock) pthread_spin_unlock(lock)
#else
	#define spin_lock(lock) 
	#define spin_unlock(lock)
#endif //multi_thread

#endif
