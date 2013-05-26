#ifndef CBS_UTIL_H
#define CBS_UTIL_H

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>

typedef pthread_spinlock_t spinlock_t;
#define spin_lock_init(lock) pthread_spin_init((lock), PTHREAD_PROCESS_SHARED)
#ifdef MULTI_THREAD
#define spin_lock(lock) pthread_spin_lock(lock)
#define spin_unlock(lock) pthread_spin_unlock(lock)
#else
#define spin_lock(lock)
#define spin_unlock(lock)
#endif //multi_thread

typedef sem_t rtu_sem_t;
#define rtu_sem_init(p_sem, val) sem_init(p_sem, 0, val) 
#define rtu_sem_post(p_sem) sem_post(p_sem)
#define rtu_sem_wait(p_sem) sem_wait(p_sem)
#define rtu_sem_destroy(p_sem) sem_destroy(p_sem)

#ifdef LITTLE_ENDIAN /** CPU endian is Little endian */
#define rtu_CpuToBe16 cpu_to_be16
#define rtu_CpuToBe32 cpu_to_be32
#define rtu_CpuToBe64 cpu_to_be64

#define rtu_Be16ToCpu be16_to_cpu
#define rtu_Be32ToCpu be32_to_cpu
#define rtu_Be64ToCpu be64_to_cpu
#else
#define rtu_CpuToBe16
#define rtu_CpuToBe32
#define rtu_CpuToBe64

#define rtu_Be16ToCpu
#define rtu_Be32ToCpu
#define rtu_Be64ToCpu
#endif

static inline void rtu_MemCopy(uint8 *p_mem_dest, const uint8 *p_mem_src, uint32 n_bytes)
{
    memcpy(p_mem_dest, p_mem_src, n_bytes);
}

static inline void rtu_MemZero(uint8 *p_address, uint32 n_bytes)
{
    memset(p_address, 0, n_bytes);
}

static inline uint8* rtu_MemAlloc(uint32 size)
{
    return (uint8 *)malloc(size);
}

static inline void rtu_MemFree(uint8 *p_buf)
{
    free(p_buf);
}

#endif
