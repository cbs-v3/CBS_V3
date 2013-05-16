/*******************************************************************
*
*    DESCRIPTION:Basic class defination for initiator
*
*    AUTHOR:raidmanji
*
*    HISTORY:
*
*    DATE:2013/4/27
*
*    Copyright (C) 1998-2013 Tencent Inc. All Rights Reserved
*******************************************************************/

#ifndef CBS_INITIATOR_H
#define CBS_INITIATOR_H

#include "cbs_types.h"
#include "cbs_buf.h"

enum init_class_order
{
    INIT_CLASS_THREAD,
    INIT_CLASS_ISCSI,
    INIT_MAX_CLASSES
};

#define INIT_ISCSI_MAX_DEVICE_ID  1
#define INIT_ISCSI_MAX_CHANNEL_ID 0x40 //64 sessions

#define INIT_MAX_INSTANCES_THREAD       1
#define INIT_MAX_INSTANCES_ISCSI             (INIT_ISCSI_MAX_DEVICE_ID * INIT_ISCSI_MAX_CHANNEL_ID)

#define INIT_MAX_INSTANCE (INIT_MAX_INSTANCES_THREAD + \
                                            INIT_MAX_INSTANCES_ISCSI)

typedef struct generic_init_area {
    uint8 sub_type;  /* sub type should located at the begining of init_area */
    uint8 status;
    uint8 rsvds[2];
    uint32 args[6];
}generic_init_area_t;

typedef struct iscsi_init_area {
    unique_id_t iqn; /* Hash iqn from 255 length to 64 length */
    uint32 args[4];
}iscsi_init_area_t;

class CInitiator
{
public:
    uint32 _index;
    u32_64 _state;
    unique_id_t _unique;
    uint32 _class_id;
    uint32 _instance_id; //sequence
    uint32 _command_count;
    struct
    {
        union
        {
            generic_init_area_t gen;
            iscsi_init_area_t iscsi; /** iscsi init */
        }type;
    }_init;

public:
    virtual void io_done(cbs_buf_t *p_cbuf);
    virtual void io_send(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
    virtual void io_receive(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
    virtual void xfer_sg(cbs_buf_t *p_cbuf);
    virtual void req2cbuf(cbs_buf_t *p_cbuf, u32_64 current_req);
    virtual cbs_buf_t* io_alloc(uint32 priority);
    virtual void io_free(cbs_buf_t *p_cbuf);
    virtual void process_request(u32_64 current_req/*pointer of the request that is going to be processed*/);

    virtual void io_start(cbs_buf_t *p_cbuf);
};

class CInitiatorPool
{
private:
    CInitiator *G_initiators[INIT_MAX_INSTANCE];

public:
    CInitiator* get_init_by_index(uint32 index);
};

extern void init_IODone(cbs_buf_t *p_cbuf);
extern void init_IOSend(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
extern void init_IOReceive(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
extern void init_XferSG(cbs_buf_t *p_cbuf);
extern void init_Req2Cbuf(cbs_buf_t *p_cbuf, u32_64 current_req);

#endif


