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

#ifndef CBS_MEM_H
#define CBS_MEM_H

#include "cbs_types.h"
#include "cbs_list.h"
#include "cbs_util.h"

class CMEMPOOL
{
    struct chunk_header
    {
        qu_queue_t chunk_list;
        uint8 *chunk_addr;
        uint32 chunk_index;
    };
public:
    CMEMPOOL(uint32 chunk_size, uint32 chunk_num) : _chunk_size(chunk_size), _chunk_num(chunk_num)
    {
        spin_lock_init(&_lock);
        qu_QueueInit(&_free_list, CMEMPOOL_CHUNK_Q_HEAD);
        _pool_addr = NULL;
        _free_count = 0;
    }

    virtual~CMEMPOOL()
    {
        if (_pool_addr != NULL)
        {
            rtu_MemFree(_pool_addr);
        }
    }

private:
    uint32 _chunk_size; //chunk size specified by caller
    uint32 _chunk_num; //num of chunks the caller want

    qu_queue_t _free_list;
    uint8 *_pool_addr;

    spinlock_t _lock;
    uint32 _free_count;

public:
    RT_STATUS init();
    uint8* chunk_alloc();
    void chunk_free(uint8 *p_buf);
};

class CChunk4KPool
{
public:
    CChunk4KPool(uint32 chunk_num) : _pool(4 * 1024, chunk_num),_chunk_num(chunk_num)
    { }

    virtual~CChunk4KPool() { }
private:
    CMEMPOOL _pool;
    uint32 _chunk_num;
public:
    RT_STATUS init();
    uint8* alloc();
    void free(uint8 *p_buf);
};

#endif


