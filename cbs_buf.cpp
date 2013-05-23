#include "cbs_buf.h"
#include "cbs_device.h"
#include "cbs_target.h"

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

/*
 *Function Name:cbuf_Done
 *
 *Parameters:
 *
 *Description: General function when a cbuf is finished!
 * 1.Do common things,like stop timer and get the cbuf out of pending queue.
 * 2.Call the target callback of a cbuf which actually is target_IODone. 
 * 3.Call init_IODone. 
 *
 *Returns:
 *
 */
void cbuf_Done(cbs_buf_t *p_cbuf)
{
    assert(p_cbuf != NULL);

    /*1. delete timer from device timer queue 
      *2. delete cbuf from device queue. 
      */
    device_CmdDone(p_cbuf);

    /* cbuf should not be in any queue. */
    assert(p_cbuf->links.flag == QUEUE_FREE_ELEMENT);

    target_CmdDone(p_cbuf);

    return;
}

/* 
 *Function Name:cbuf_ScsiCdb 
 *  
 * Parameters:
 *
 * @param op scsi opreation code.
 * @param block_address block address.
 * @param xfer_blocks transfer length.
 * @param p_cdb pointer to the data which will hold cdb when return.   
 * 
 * Returns: 
 */
void cbuf_ScsiCdb(uint8 op, uint32 block_address, uint32 xfer_blocks, uint8 *p_cdb)
{

    p_cdb[0] = op;

    p_cdb[1] = 0x00;
    p_cdb[5] = (uint8)(block_address & 0x000000ff);
    p_cdb[4] = (uint8)((block_address & 0x0000ff00) >> 8);
    p_cdb[3] = (uint8)((block_address & 0x00ff0000) >> 16);
    p_cdb[2] = (uint8)((block_address & 0xff000000) >> 24);
    p_cdb[6] = 0;
    p_cdb[7] = (uint8)((xfer_blocks & 0xff00) >> 8);
    p_cdb[8] = (uint8)(xfer_blocks & 0x00ff);
    p_cdb[9] = 0;
    p_cdb[10] = 0;
    p_cdb[11] = 0;
    p_cdb[12] = 0;
    p_cdb[13] = 0;
    p_cdb[14] = 0;
    p_cdb[15] = 0;
}

void cbuf_ScsiCdb6(uint8 op, uint32 block_address, uint32 xfer_blocks, uint8 *p_cdb)
{

    p_cdb[0] = op;

    p_cdb[3] = (uint8)(block_address & 0x000000ff);
    p_cdb[2] = (uint8)((block_address & 0x0000ff00) >> 8);
    p_cdb[1] = (uint8)((block_address & 0x001f0000) >> 16);
    p_cdb[4] = (uint8)(xfer_blocks & 0x00ff);
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
