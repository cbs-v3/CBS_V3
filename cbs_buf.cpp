#include "cbs_buf.h"
#include "cbs_device.h"

static CCBufPool g_cbuf_pool;
cbs_buf_t* cbuf_alloc(uint32 priority)
{
    return g_cbuf_pool.cbuf_pop();
}

void cbuf_free(cbs_buf_t *p_cbuf)
{
    g_cbuf_pool.cbuf_push(p_cbuf);
    return;
}

cbs_buf_t* get_cbuf_by_index(uint16 index)
{
    return g_cbuf_pool._get_cbuf_by_index(index);
}

cbs_buf_t* get_cbuf_by_handle(uint32 handle)
{
    return g_cbuf_pool._get_cbuf_by_handle(handle);
}

void cbuf_done(cbs_buf_t *p_cbuf)
{
    CDevice* p_dev = get_device_by_index(p_cbuf->device_no);
    return;
}

void CCBufPool::init_cbuf(cbs_buf_t *p_cbuf)
{
    p_cbuf->initiator = INIT_ZERO;
    p_cbuf->target = TARGET_ZERO;

    p_cbuf->response = 0;
    p_cbuf->flags = 0;
    p_cbuf->queue_time = 0;
    p_cbuf->start_time = 0;
    p_cbuf->packet.response.status = 0;
    p_cbuf->packet.response.status_flags = 0;
    p_cbuf->packet.response.completion_status = 0;
    p_cbuf->packet.response.sense_info.addt_sc = 0;
    p_cbuf->packet.response.sense_info.addt_sc_qualifier = 0;
    p_cbuf->sequence++;

    return;
}

void CCBufPool::clear_cbuf(cbs_buf_t *p_cbuf)
{
    /** clear up some sensitive area */
    p_cbuf->err_flags = 0;
    p_cbuf->packet.cmd.target_lun = 0;
    p_cbuf->sequence++;

    return;
}

void CCBufPool::reset_cbuf(cbs_buf_t *p_cbuf)
{
    /** clear up some sensitive area since we didnot free this cbuf */
    p_cbuf->err_flags = 0;
    p_cbuf->packet.response.status_flags = 0;
    p_cbuf->packet.response.completion_status = 0;
    p_cbuf->packet.response.sense_data_len = 0;
    p_cbuf->packet.response.resp_info_len = 0;
    p_cbuf->packet.response.sense_info.addt_sc = 0;
    p_cbuf->packet.response.sense_info.addt_sc_qualifier = 0;
    p_cbuf->target.gen.xfer_state = CBUF_TARGET_XFER_NULL;
    p_cbuf->sequence++;

    return;
}

cbs_buf_t* CCBufPool::cbuf_pop()
{
    cbs_buf_t *p_cbuf = NULL;

    spin_lock(&_lock);
    if (_free_count != 0)
    {
        p_cbuf = qu_Entry(qu_GetHeadTail(&_free),
                          cbs_buf_t, links);
        qu_DeQueue(&p_cbuf->links, CBUF_FREE_Q);
        _free_count--;
    }
    spin_unlock(&_lock);

    if (p_cbuf != NULL)  init_cbuf(p_cbuf);

    return p_cbuf;
}

void CCBufPool::cbuf_push(cbs_buf_t *p_cbuf)
{
    clear_cbuf(p_cbuf);
    spin_lock(&_lock);
    qu_EnQueue(&p_cbuf->links, &_free, CBUF_FREE_Q);
    _free_count++;
    spin_unlock(&_lock);

    return;
}

cbs_buf_t* CCBufPool::_get_cbuf_by_index(uint16 index)
{
    cbs_buf_t *p_cbuf = NULL;

    if (index < CBUF_POOL_SIZE)
    {
        p_cbuf = &_cbs_pool[index];
    }

    return p_cbuf;
}

cbs_buf_t* CCBufPool::_get_cbuf_by_handle(uint32 handle)
{
    cbs_buf_t *p_cbuf = NULL;

    if ((p_cbuf = _get_cbuf_by_index(GET_CBUF_INDEX_BY_HANDLE(handle))) != NULL)
    {
        if (p_cbuf->sequence != GET_CBUF_SEQUENCE_BY_HANDLE(handle)) p_cbuf = NULL;  //sequence mismatch
    }

    return p_cbuf;
}
