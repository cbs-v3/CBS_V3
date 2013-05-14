#ifndef CBS_TARGET_H
#define CBS_TARGET_H

#include <string>
#include "cbs_types.h"
#include "cbs_buf.h"
#include "cbs_device.h"

enum target_class_order{
	TARGET_CLASS_UNINIT,
	TARGET_CLASS_INVALID,
    TARGET_CLASS_NULL,
    TARGET_CLASS_TFS,
    TARGET_CLASS_TSSD, 
    TARGET_CLASS_THREAD,
    TARGET_CLASS_MAX
};

class CTarget
{
public:
	CTarget(){}
	virtual ~CTarget(){}
	
	uint32 _class_id;
	uint32 _flags;
	
	std::string _name;
	
protected:
	virtual RT_STATUS queue_cbuf(CDevice* p_dev, cbs_buf_t* p_cbuf);
	virtual void queue_scan(CDevice* p_dev)=0;
    virtual receive_data(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
    virtual void cmd_done(cbs_buf_t* p_cbuf);
public:
	void target_receive_data(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
	void target_cmd_done(cbs_buf_t* p_cbuf);
	uint32 target_extract_sense_info(scsi_sense_info_t& sense_info, uint32 device_no);
	void target_reply_good(cbs_buf_t* p_cbuf);
    void target_reply_nodevice(cbs_buf_t* p_cbuf);
	
private:
	void translate_sense_ctx(uint32 sense_ctx, scsi_sense_info_t *p_sense_info);
	void get_sense_info_and_update(cbs_buf_t* p_cbuf);
	scsi_sense_info_t* get_device_sense(uint32 device_no);
	void build_check_resp_info(cbs_buf_t* p_cbuf);
};

class CTargetManager
{
public:
	CTargetManager()
	{
		for (int i = 0; i < TARGET_CLASS_MAX; i++)
			G_targets[i] = NULL;
	}
	virtual ~CTargetManager(){}
	
private:
	CTarget* G_targets[TARGET_CLASS_MAX]; //array of target class
public:
	void target_ClassInit(uint32 class_id, uint32 flags, const char* p_name, CTarget* p_target);
	void get_target_by_class_id(uint32 class_id);
};

class CTargetUninit:public CTarget
{

protected:
    virtual void queue_scan(CDevice *p_dev);
    virtual void cmd_done(cbs_buf_t *p_cbuf);
};


#endif
