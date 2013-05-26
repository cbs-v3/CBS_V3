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
    CThreadDevice()
    { `
    }
public:
    qu_queue_t _links;
    pthread_t _tid;
    uint32 _thread_id;
    uint32 _cpu_no;
    uint32 _priority;

    u32_64 _args[4];
    rtu_sem_t _sem;

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

class CThreadInitiator : public CInitiator
{

};

class CThreadTarget : public CTarget
{
public:



};

#endif
