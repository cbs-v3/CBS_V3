/*******************************************************************
*
*    DESCRIPTION: 
*
*    AUTHOR:raidmanji
*
*    HISTORY:
*
*    DATE:5/16/2013
*
*    Copyright (C) 1998-2013 Tencent Inc. All Rights Reserved
*******************************************************************/
#include <assert.h>

#include "cbs_debug.h"
#include "cbs_initiator.h"
#include "cbs_device.h"
#include "cbs_target.h"

static CInitiatorPool g_initiator_pool;

void init_IODone(cbs_buf_t *p_cbuf)
{
    CInitiator *p_init = g_initiator_pool.get_init_by_index(p_cbuf->initiator.gen.init_index);
    p_init->io_done(p_cbuf);
    return;
}

void init_IOSend(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size)
{
    CInitiator *p_init = g_initiator_pool.get_init_by_index(p_cbuf->initiator.gen.init_index);
    p_init->io_send(p_cbuf, p_data, size);
    return;
}

void init_IOReceive(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size)
{
    CInitiator *p_init = g_initiator_pool.get_init_by_index(p_cbuf->initiator.gen.init_index);
    p_init->io_receive(p_cbuf, p_data, size);
    return;
}

void init_XferSG(cbs_buf_t *p_cbuf)
{
    CInitiator *p_init = g_initiator_pool.get_init_by_index(p_cbuf->initiator.gen.init_index);
    p_init->xfer_sg(p_cbuf);
    return;
}

void init_Req2Cbuf(cbs_buf_t *p_cbuf, u32_64 current_req)
{
    CInitiator *p_init = g_initiator_pool.get_init_by_index(p_cbuf->initiator.gen.init_index);
    p_init->req2cbuf(p_cbuf, current_req);
}

/*-------------------------class CInitiator---------------------------------*/
void CInitiator::io_done(cbs_buf_t *p_cbuf)
{
    LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_INITIATOR, "io_done in initiator basic class");
    return;
}

void CInitiator::io_send(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size)
{
    LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_INITIATOR, "io_send in initiator basic class");
    return;
}

void CInitiator::io_receive(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size)
{
    LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_INITIATOR, "io_receive in initiator basic class");
    return;
}

void CInitiator::xfer_sg(cbs_buf_t *p_cbuf)
{
    LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_INITIATOR, "xfer_sg in initiator basic class");
    return;
}

cbs_buf_t* CInitiator::io_alloc(uint32 priority)
{
    return cbuf_alloc(priority);
}

void CInitiator::io_free(cbs_buf_t *p_cbuf)
{
    cbuf_free(p_cbuf);
}

void CInitiator::io_start(cbs_buf_t *p_cbuf)
{
    CDevice *p_dev;

    /* get the target and do some work */
    p_dev = get_device_by_index(p_cbuf->device_no);
    p_cbuf->response = CBUF_RESP_PENDING;

    /*start timer*/
    p_dev->add_timer(&p_cbuf->timer);

    /*queue to device*/
    if (device_IsAllocted(p_dev))
    {
        target_QueueCbuf(p_dev, p_cbuf);
    }else
    {
        LOG_ERROR("CBUF was queued to device:%u that has not been allocted", p_dev->get_device_no());
        target_ReplyNoDevice(p_cbuf);
    }

    return;
}

void CInitiator::process_request(u32_64 current_req/*pointer of the request that is going to be processed*/)
{
    cbs_buf_t *p_cbuf;

    /** get a free buf */
    p_cbuf = io_alloc(CBUF_PRIORITY_NORMAL);
    if (p_cbuf != NULL)
    {
        /** translate the current request packet to cbuf format */
        req2cbuf(p_cbuf, current_req);
        /** do StartIO */
        io_start(p_cbuf);   /** iscsi_IOStart/thread_IOStart... */
    } else
    {
        LOG_FATAL("cbuf exhausted!!!");
        assert(false);
    }

    return;
}

void CInitiator::req2cbuf(cbs_buf_t *p_cbuf, u32_64 current_req)
{
        LOG_DEBUG(CBS_DEBUG_LEVEL, CBS_DEBUG_INITIATOR, "req2cbuf in initiator basic class");
}
/*-------------------------class CInitiatorPool-------------------------------*/
CInitiator* CInitiatorPool::get_init_by_index(uint32 index)
{
    assert(index >= INIT_MAX_INSTANCE);
    return G_initiators[index];
}
