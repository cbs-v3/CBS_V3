#ifndef CBS_BUF_H
#define CBS_BUF_H

#include <list>
#include <time.h>

#include "cbs_types.h"
#include "cbs_util.h"
#include "cbs_list.h"
#include "cbs_timer.h"
#include "cbs_scsi.h"

/**
 * The SCSI command wrapper
 */
typedef struct scsi_cmd {
    uint32 handle;
    uint8 target_lun;
    uint8 target_id;
    uint16 timeout;  /** requested timeout in seconds */
    uint16 control_flags;
    uint16 pad;
    uint8 cdb[16];
}scsi_cmd_t;

typedef struct scsi_rsp {
    uint32 handle;
    uint8  status;
    uint8  status_flags;
    uint16 completion_status;
    uint16 resp_info_len;
    uint16 sense_data_len;
    uint32 residual_count;
    uint8 resp_info[8];
    scsi_sense_info_t sense_info;
}scsi_rsp_t;

typedef struct scsi_pkt {
    scsi_cmd_t  cmd;
    scsi_rsp_t  response;
}scsi_pkt_t;

#define CBUF_STATUS_FLAG_UNDER               0x08
#define CBUF_STATUS_FLAG_OVER                0x04
#define CBUF_STATUS_FLAG_SENSE_LEN_VALID     0x02
#define CBUF_STATUS_FLAG_RSP_LEN_VALID       0x01

#define CBUF_RESP_COMPLETE           0x00
#define CBUF_RESP_NO_DEVICE          0x01
#define CBUF_RESP_NO_RESOURCES       0x02
#define CBUF_RESP_ERROR              0x03
#define CBUF_RESP_TIMEOUT            0x04
#define CBUF_RESP_ABORTED            0x05
#define CBUF_RESP_CHECK              0x06
#define CBUF_RESP_PENDING            0x07

#define CBUF_TARGET_XFER_NULL      0
#define CBUF_TARGET_XFER_PENDING   1
#define CBUF_TARGET_XFER_DONE      2
#define CBUF_TARGET_XFER_ERROR     3
#define CBUF_TARGET_XFER_ABORTED   4

/********************Initiator Defination*******************/
typedef struct common_init{
    uint32 init_index;    /* init instance index */
    uint32 init_instance; /* init instance */
    uint32 init_class;    /* class id */
    uint32 sense_context;
    uint32 original_init;
	uint32 arg;
}common_init_t;

typedef struct iscsi_init {
    common_init_t common;
    
    uint16 index;  /*iscsi cmnd index*/
    uint16 sequence;  /*iscsi cmnd sequence*/
    
    uint32 bytes_sent_get;
    uint32 task_id;
    uint32 args[1];
}iscsi_init_t;

typedef union initiator_specific{ 
    common_init_t gen;     /* Generic Initiator*/
    iscsi_init_t iscsi;    /** iSCSI Initiator */
}initiator_specific_t;
/********************End of Initiator Defination***********/

/*******************Target Defination**********************/
typedef struct common_target{
    uint32 sense_context;
    uint32 target_class;    /*class id*/
    uint32 xfer_state;
}common_tar_t;

typedef struct generic_target{
    common_tar_t common;
    uint64 handle; /* flag (uint8) | mpool_index */
    uint32 args[5]; // args[0] will be cleared by target_CmdDone.
}generic_tar_t;

typedef union target_specific{
    common_tar_t gen;      /** Generic Target        */
    generic_tar_t generic; /** common + handle + arg1 + arg2 */
}target_specific_t;
/*********************End of Target Defination*****************/

#define INIT_ZERO (initiator_specific_t){{0,0,0,0}}
#define TARGET_ZERO (target_specific_t){{0,0,0}}

/**
 * Scatter Gather primitives
 */
typedef struct sg_element {
    uint64 addr;
    uint32 len;
}sg_element_t;

typedef struct sg_list {
    uint32 len;
    sg_element_t sg[1];
}sg_list_t;

typedef struct cbs_buf {
	qu_queue_t links;
	
	uint32 device_no;
	uint32 io_class;
	scsi_pkt_t packet;
	
	uint16 index; // readonly after initialize
	uint16 sequence; // readonly after initialize
	
	uint16 q_flags;
	uint16 q_sequence;
	uint16 err_flags;
	uint16 reserved;
	uint32 flags;
	
	tm_timer_t timer;
	
	time_t queue_time;   //time since cbuf was added to queue
	time_t start_time;   //time since cbuf started executing
	time_t done_time;    //time since cbuf done
	
	uint32 response;
	uint32 requested_transfer_size;
	uint32 residual;
	uint32 total_blocks;
	uint32 start_block;
	uint32 data_dir;
	
	initiator_specific_t initiator;
	target_specific_t target;
	
	sg_list_t* p_sglist;
    sg_list_t sge;  /** single entry scatter gather list */
}cbs_buf_t;

#define CBUF_FLAG_SCAN_LATER	0x8000
#define CBUF_FLAG_QUEUE_FIRST	0x4000

#define CBUF_POOL_SIZE	1024
class CCBufPool
{
public:
	CCBufPool()
	{
		_free_count = 0;
		spin_lock_init(&_lock);
		qu_QueueInit(&_free, CBUF_FREE_Q_HEAD);
		for (int i = 0; i < CBUF_POOL_SIZE; i++)
		{
			_cbs_pool[i].index = i;
			_cbs_pool[i].sequence = 0;
			cbuf_push(&_cbs_pool[i]);
		}
	}
	
	virtual ~CCBufPool(){}
	
private:
	cbs_buf_t _cbs_pool[CBUF_POOL_SIZE];
	qu_queue_t _free; //list of free cbuf
	uint32 _free_count;
	
	spinlock_t _lock;
	
public:
	cbs_buf_t* cbuf_pop();
	void cbuf_push(cbs_buf_t* p_cbuf);
	
	cbs_buf_t* _get_cbuf_by_index(uint16 index);
	cbs_buf_t* _get_cbuf_by_handle(uint32 handle); //if sequence mismatch, return null
	
	void reset_cbuf(cbs_buf_t* p_cbuf);
private:
	void init_cbuf(cbs_buf_t* p_cbuf);
	void clear_cbuf(cbs_buf_t* p_cbuf);
};

extern cbs_buf_t* cbuf_alloc(uint32 priority);
extern void cbuf_free(cbs_buf_t* p_cbuf);
extern cbs_buf_t* get_cbuf_by_index(uint16 index);
extern cbs_buf_t* get_cbuf_by_handle(uint32 handle);

#define MAKE_CBUF_HANDLE(p_cbuf) \
	(((uint32)((p_cbuf)->index) << 16) | (uint32)((p_cbuf)->sequence)&0x0000ffff)

#define GET_CBUF_INDEX_BY_HANDLE(handle) \
	(uint16)((handle>> 16) & 0x0000ffff)
	
#define GET_CBUF_SEQUENCE_BY_HANDLE(handle)\
	(uint16)(handle&0x0000ffff)

#endif
