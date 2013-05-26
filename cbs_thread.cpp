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

static CThreadDevicePool g_thread_pool;

extern CThreadDevice* cbs_GetThread(uint32 thread_id)
{
	return g_thread_pool.get_thread(thread_id);
}


/*
*set the max_pending=1
*/
CThreadDevice::CThreadDevice(uint32 thread_id, start_routine p_entry, 
	const char* name, uint32 priority):CDevice(TARGET_CLASS_THREAD, CBS_THREAD_DEVICES_SHIFT+thread_id, 1, name)
{
	_thread_id = thread_id;
	_p_entry = p_entry;
	_priority = priority;
	_state = CBS_THREAD_FREE;
}

void CThreadDevicePool::create_thread(uint32 thread_id, start_routine p_entry, uint32 priority, const char *name)
{
    int ret;
    pthread_t tid;

	/*step 1: alloc the thread device, and initialise it */
    uint32 device_no = CBS_THREAD_DEVICES_SHIFT + thread_id;
    CThreadDevice *p_thread_dev = new CThreadDevice(thread_id, p_entry, priority, name);
    if (p_thread_dev == NULL)
    {
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_THREAD, "alloc thread device:%s failed", name);
		assert(false);
    }

	/*step 2: register the thread device to the device pool */
	if (cbs_DeviceAlloc((CDevice *)p_thread_dev) != RT_OK)
	{
		LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_THREAD, "Register thread device:%s failed", name);
		assert(false);
	}

	/*step 3: create work thread */
    ret = pthread_create(&tid, NULL, p_thread_dev->_p_entry, (void *)p_thread_dev);
    if (ret != 0)
    {
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_THREAD, "create thread:%s failed", p_thread_dev->_name.c_str());
        assert(false);
    }
    p_thread_dev->_tid = tid;
	_map_thread[tid] = thread_id; /* relate the thread id with the tid*/

    return;
}

CThreadDevice* CThreadDevicePool::get_thread(uint32 thread_id)
{
    uint32 device_no = CBS_THREAD_DEVICES_SHIFT + thread_id;
    CThreadDevice *p_thread_dev = NULL;
    p_thread_dev = cbs_GetDevice(device_no);

    return p_thread_dev;
}

CThreadInitiator::io_start(cbs_buf_t *p_cbuf)
{
	CThreadDevice *p_dev = cbs_GetThread(p_cbuf.initiator.thread.thread_id);

	LOG_DEBUG(CBS_DEBUG_LEVEL,CBS_DEBUG_THREAD,
		"start send cbuf:%u to thread:%s", 
		p_cbuf->index, p_dev->get_name());

	p_cbuf->response = CBUF_RESP_PENDING;
	if (cbs_IsDeviceAlloced(p_dev))
	{
		/*step 1: start timer*/
    	p_dev->add_timer(&p_cbuf->timer);
		p_dev->_args[1] = (u32_64)p_cbuf;
		p_dev->_state = CBS_THREAD_PENDING_IO;
		target_QueueCbuf(p_dev, p_cbuf);
		while (p_cbuf->response)
	}
	else
	{
		LOG_DEBUG(CBS_DEBUG_LEVEL,CBS_DEBUG_THREAD,
			"thread device:%u has not been alloced", p_cbuf->device_no);
			target_ReplyNoDevice(p_cbuf);
	}

	return;
}	
	

