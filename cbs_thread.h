/*******************************************************************
*
*    DESCRIPTION:
*
*    AUTHOR:raidmanji
*
*    HISTORY:
*
*    DATE:5/25/2013
*
*    Copyright (C) 1998-2013 Tencent Inc. All Rights Reserved
*******************************************************************/

#ifndef CBS_THREAD_H
#define CBS_THREAD_H

#include <string>
#include <map>

#include "cbs_initiator.h"
#include "cbs_target.h"
#include "cbs_device.h"

#define CBS_THREAD_FREE          0x0
#define CBS_THREAD_SUSPEND       0x1
#define CBS_THREAD_RUN           0x2
#define CBS_THREAD_SCHEDULE      0x3
#define CBS_THREAD_SLEEP         0x4
#define CBS_THREAD_PENDING_IO    0x5
#define CBS_THREAD_WAIT_SEM      0x6 
#define CBS_THREAD_WAIT_MUTEX    0x7 
#define CBS_THREAD_WAIT_COND     0x8 
#define CBS_THREAD_WAIT_SIGNAL   0x9
#define CBS_THREAD_WAIT_BARRIER  0xa
#define CBS_THREAD_WAIT_MESSAGE  0xb


#define CBS_THREAD_CMD_MAX_RETRY 3
enum cbs_thread_id
{
    CBS_THREAD_ID_IDLE,
    CBS_THREAD_CONFIG,
    CBS_THREAD_ID_MAX
};

typedef void* (*start_routine)(void *);
class CThreadDevice : public CDevice
{
public:
    CThreadDevice(uint32 thread_id, start_routine p_entry, const char* name, uint32 priority = 0)
    {}
public:
    pthread_t _tid;
    uint32 _thread_id;
    uint32 _cpu_no;
    uint32 _priority;
	uint32 _state;

    u32_64 _args[4];

    start_routine _p_entry;
};

class CThreadDevicePool
{
public:
    void create_thread(uint32 thread_id, start_routine p_entry, uint32 priority, const char *name);
    CThreadDevice* get_thread(uint32 thread_id);

private:
    std::map<pthread_t, uint32> _map_thread; //map the pthread id with the thread id
};

class CThreadInitiator:public CInitiator
{
};

class CThreadTarget:public CTarget
{
};

extern cbs_GetThread(uint32 thread_id);

#endif
