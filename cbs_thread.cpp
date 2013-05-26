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

#include "cbs_thread.h"

void CThreadDevicePool::create_thread(uint32 thread_id, start_routine p_entry, uint32 priority, const char *name)
{
    int ret;
    pthread_t tid;

    uint32 device_no = CBS_THREAD_DEVICES_SHIFT + thread_id;
    CThreadDevice *p_thread_dev = new CThreadDevice(thread_id, p_entry, priority, name);
    if (p_thread_dev == NULL)
    {
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_THREAD, "alloc thread device:%s failed", name);
    }


    ret = pthread_create(&tid, NULL, p_thread->_p_entry, (void *)p_thread);
    if (ret != 0)
    {
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_THREAD, "create thread:%s failed", p_thread->_name.c_str());
        assert(false);
    }
    p_thread->_tid = tid;

    return 0;
}

CThreadDevice* CThreadDevicePool::get_thread(uint32 thread_id)
{
    uint32 device_no = CBS_THREAD_DEVICES_SHIFT + thread_id;
    CThreadDevice *p_thread_dev = NULL;
    p_thread_dev = cbs_GetDevice(device_no);

    return p_thread_dev;
}
