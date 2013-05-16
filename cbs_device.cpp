/*******************************************************************
*
*    DESCRIPTION:
*
*    AUTHOR:raidmanji
*
*    HISTORY:
*
*    DATE:2013/4/28
*
*    Copyright (C) 1998-2013 Tencent Inc. All Rights Reserved
*******************************************************************/
#include <time.h>

#include "cbs_device.h"
#include "cbs_target.h"

static CDevicePool g_device_pool;

CDevice* get_device_by_index(uint32 index)
{
    return g_device_pool._get_device_by_index(index);
}


/*-----------------class device------------------*/
void CDevice::cmd_done(cbs_buf_t *p_cbuf)
{
    p_cbuf->done_time = time(NULL);

    /*step 1. del timer*/
    _q_timer.del_timer(&p_cbuf->timer);

    /*step 2. del cbuf from the queue*/
    spin_lock(&_q_lock);
    switch (p_cbuf->links.flag)
    {
    case CBUF_DEVICE_Q:
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_CBUF, "delete cbuf:[%u,%u] from the waiting Q of device:%u",
                  p_cbuf->index, p_cbuf->sequence, p_cbuf->device_no);
        qu_DeQueue(&p_cbuf->links, CBUF_DEVICE_Q);
        _waitings--;
        break;
    case CBUF_DEVICE_PENDING_Q:
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_CBUF, "delete cbuf:[%u,%u] from the pending Q of device:%u",
                  p_cbuf->index, p_cbuf->sequence, p_cbuf->device_no);
        qu_DeQueue(&p_cbuf->links, CBUF_DEVICE_PENDING_Q);
        _pendings--;
    case QUEUE_FREE_ELEMENT:
    default:
        LOG_ERROR("flag of cbus is %u, device:%u", p_cbuf->links.flag, p_cbuf->device_no);
        break;
    }
    spin_unlock(&_q_lock);

    return;
}

CDevicePool::CDevicePool() 
{
    _uninit_dev = new CDevice(TARGET_CLASS_UNINIT,  CBS_MAX_UNINIT_DEVICES, 0, "Uninit Device");
    for (int i = 0; i < CBS_MAX_DEVICES; i++)
    {
        G_devices[i] = _uninit_dev; //init all device as uninit.
    }
}

CDevice* CDevicePool::_get_device_by_index(uint32 index)
{
    //invalid device index;
    index = (index >= CBS_MAX_DEVICES ? CBS_MAX_UNINIT_DEVICES : index);
    return G_devices[index];
}

RT_STATUS CDevicePool::_device_register(CDevice *p_dev)
{
    if (p_dev == NULL)
    {
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_DEV, "Device pointer is null");
        return RT_FAIL;
    }

    if (p_dev->_index >= CBS_MAX_DEVICES)
    {
        LOG_ERROR("Register devcie:%s failed, wrong devcie no:%u", p_dev->_name.c_str(), p_dev->_index);
        return RT_FAIL;
    }

    if (G_devices[p_dev->_index] != _uninit_dev)
    {
        LOG_ERROR("Device no:%u has been registered for %s", p_dev->_index, G_devices[p_dev->_index]->_name.c_str());
        return RT_FAIL;
    }

    G_devices[p_dev->_index] = p_dev;
    p_dev->_state = CBS_DEVICE_ALLOCED;

    LOG_NORMAL("Register device:%s successfully, device no is:%u", p_dev->_name.c_str(), p_dev->_index);

    return RT_OK;
}

void CDevicePool::_device_unregister(CDevice *p_dev)
{
    if (p_dev == NULL)
    {
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_DEV, "Device pointer is null");
        return;
    }

    if (p_dev->_index >= CBS_MAX_DEVICES)
    {
        LOG_ERROR("Unregister devcie:%s failed, wrong devcie no:%u", p_dev->_name.c_str(), p_dev->_index);
        return;
    }

    if (p_dev != G_devices[p_dev->_index])
    {
        LOG_ERROR("Unregister devcie:%s failed, the device no:%u that has been registerd is device:%s",
                  p_dev->_name.c_str(),
                  p_dev->_index,
                  G_devices[p_dev->_index]->_name.c_str());
        return;
    }

    p_dev->clear_sense_info();
    p_dev->_state = CBS_DEVICE_FREE;
    G_devices[p_dev->_index] = _uninit_dev;

    LOG_NORMAL("Unregister device:%s successfully, device no is:%u", p_dev->_name.c_str(), p_dev->_index);

    return;
}
