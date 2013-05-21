#ifndef CBS_TARGET_H
#define CBS_TARGET_H

#include <string>
#include <vector>
#include <map>

#include "cbs_types.h"
#include "cbs_buf.h"
#include "cbs_device.h"

enum target_class_order
{
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
    typedef uint32(CTarget::*load_inq_func)(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    CTarget()
    {
        _inq_pages[0x00] = NULL;
        _inq_pages[0x80] = &CTarget::load_unit_serial;
        _inq_pages[0x83] = &CTarget::load_wwn;
        _inq_pages[0xC0] = &CTarget::_load_Firmware;
    }
    virtual ~CTarget() { }

    uint32 _class_id;
    uint32 _flags;

    std::string _name;
    std::map<int, load_inq_func>  _inq_pages;

protected:
    virtual void receive_data(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
    virtual void cmd_done(cbs_buf_t *p_cbuf);
/** load inquiry page */
    virtual uint32 load_stand_inquiry_page(CDevice *p_dev, cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    virtual uint32 load_unit_serial(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    virtual uint32 load_wwn(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);

public:
    RT_STATUS queue_cbuf(CDevice *p_dev, cbs_buf_t *p_cbuf);
    virtual void device_scan(CDevice *p_dev) = 0; 

    void target_receive_data(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
    void target_cmd_done(cbs_buf_t *p_cbuf);
    uint32 target_extract_sense_info(scsi_sense_info_t& sense_info, uint32 device_no);

    void reply_good(cbs_buf_t *p_cbuf);
    void reply_nodevice(cbs_buf_t *p_cbuf);
    void reply_cmd_aborted(cbs_buf_t *p_cbuf);
    void reply_cmd_with_error(cbs_buf_t *p_cbuf, uint32 sense);
    void reply_complete(cbs_buf_t *p_cbuf);
    void reply_error(cbs_buf_t *p_cbuf);
    uint32 load_inquiry_page(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);


private:
    void translate_sense_ctx(uint32 sense_ctx, scsi_sense_info_t *p_sense_info);
    void get_sense_info_and_update(cbs_buf_t *p_cbuf);
    scsi_sense_info_t* get_device_sense(uint32 device_no);
    void build_check_resp_info(cbs_buf_t *p_cbuf);

/** functions load inquiry pages */
    uint32 _load_stand_inquiry_page(CDevice *p_dev, cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    uint32 _load_Firmware(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);

};

class CTargetPool
{
public:
    CTargetPool()
    {
        for (int i = 0; i < TARGET_CLASS_MAX; i++) G_targets[i] = NULL;
    }
    virtual ~CTargetPool() { }

private:
    CTarget *G_targets[TARGET_CLASS_MAX]; //array of target class

public:
    inline CTarget* get_target_by_class_id(uint32 class_id)
    { return G_targets[class_id]; }

    void target_class_init(uint32 class_id, uint32 flags, const char *p_name, CTarget *p_target);
    void init();
};

class CTargetUninit : public CTarget
{
protected:
    virtual void device_scan(CDevice *p_dev);
    virtual void cmd_done(cbs_buf_t *p_cbuf);
};

/** pack functions of class CTargetPool  */
extern CTarget* get_target_by_class_id(uint32 class_id);
extern CTarget* get_target_by_device_no(uint32 device_no);

extern void target_ReplyGood(cbs_buf_t *p_cbuf);
extern void target_ReplyNoDevice(cbs_buf_t *p_cbuf);
extern void target_ReplyCmdAborted(cbs_buf_t *p_cbuf);
extern void target_ReplyCmdDoneWithError(cbs_buf_t *p_cbuf, uint32 sense);
extern void target_ReplyComplete(cbs_buf_t *p_cbuf);
extern void target_ReplyError(cbs_buf_t *p_cbuf);

/**/
extern uint32 target_LoadInquiryPage(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);

extern void target_QueueCbuf(CDevice *p_dev,  cbs_buf_t *p_cbuf);
extern void target_DeviceScan(CDevice *p_dev);

#endif
