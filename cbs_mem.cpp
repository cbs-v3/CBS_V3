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

#include "cbs_mem.h"

RT_STATUS CMEMPOOL::init()
{
    uint32 size;
    uint8 *p_buf;
    struct chunk_header *p_head;

    size = _chunk_num * (_chunk_size + sizeof(struct chunk_header));
    _pool_addr = rtu_MemAlloc(size);
    if (_pool_addr == NULL)
    {
        return RT_ALLOC_FAILED;
    }

    p_buf = _pool_addr;
    for (uint32 i = 0; i < _chunk_num; i++)
    {
        p_head = (struct chunk_header *)p_buf;
        p_head->chunk_addr = p_buf + sizeof(struct chunk_header);
        p_head->chunk_index = i;
        qu_EnQueueTail(&p_head->chunk_list, &_free_list, CMEMPOOL_CHUNK_Q);
        p_buf += _chunk_size + sizeof(struct chunk_header);
        _free_count++;
    }

    return RT_OK;
}

uint8* CMEMPOOL::chunk_alloc()
{
    uint8 *p_buf = NULL;
    struct chunk_header *p_head;

    spin_lock(&_lock);
    if (_free_count != 0)
    {
        p_head = qu_Entry(_free_list.fwd, struct chunk_header, chunk_list);
        p_buf = p_head->chunk_addr;
        qu_DeQueue(&p_head->chunk_list, CMEMPOOL_CHUNK_Q);
        _free_count--;
    }
    spin_unlock(&_lock);

    return p_buf;
}

void CMEMPOOL::chunk_free(uint8 *p_buf)
{
    struct chunk_header *p_head;

    p_head = (struct chunk_header *)(p_buf - sizeof(struct chunk_header));
    spin_lock(&_lock);
    qu_EnQueueTail(&p_head->chunk_list, &_free_list, CMEMPOOL_CHUNK_Q);
    _free_count++;
    spin_unlock(&_lock);

    return;
}

RT_STATUS CChunk4KPool::init()
{
    return _pool.init();
}

uint8* CChunk4KPool::alloc()
{
    return _pool.chunk_alloc();
}

void CChunk4KPool::free(uint8 *p_buf)
{ 
    _pool.chunk_free(p_buf);
}


