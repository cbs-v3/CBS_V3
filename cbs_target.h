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
    typedef uint32(CTarget::*load_mod_func)(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    CTarget()
    {
        _inq_pages[0x00] = &CTarget::_load_zero_page;
        _inq_pages[0x80] = &CTarget::load_unit_serial_page;
        _inq_pages[0x83] = &CTarget::load_wwn_page;
        _inq_pages[0xC0] = &CTarget::_load_Firmware_page;

        _mod_pages[0x02][0] = &CTarget::load_disconnect_reconnect_page;
        _mod_pages[0x03][0] = &CTarget::load_format_parameters_page;
        _mod_pages[0x04][0] = &CTarget::load_disk_geometry_page;
        _mod_pages[0x08][0] = &CTarget::load_cacheing_parameters_page;
        _mod_pages[0x0a][0] = &CTarget::load_control_page;
        _mod_pages[0x1c][0] = &CTarget::load_info_exceptions_page;
    }
    virtual ~CTarget() { }

    uint32 _class_id;
    uint32 _flags;

    std::string _name;
    std::map<int, load_inq_func>  _inq_pages;
    std::map<int, std::map<int, load_mod_func> > _mod_pages;

protected:
    virtual void receive_data(cbs_buf_t *p_cbuf, uint8 *p_data, uint32 size);
    virtual void cmd_done(cbs_buf_t *p_cbuf);
/** load inquiry page */
    virtual uint32 load_stand_inquiry_page(CDevice *p_dev, cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    virtual uint32 load_unit_serial_page(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    virtual uint32 load_wwn_page(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);

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
    uint32 load_mode_page(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);


private:
    void translate_sense_ctx(uint32 sense_ctx, scsi_sense_info_t *p_sense_info);
    void get_sense_info_and_update(cbs_buf_t *p_cbuf);
    scsi_sense_info_t* get_device_sense(uint32 device_no);
    void build_check_resp_info(cbs_buf_t *p_cbuf);

/** functions load inquiry pages */
    uint32 _load_stand_inquiry_page(CDevice *p_dev, cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    uint32 _load_Firmware_page(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);
    uint32 _load_zero_page(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);

/** functions load mode pages */
    uint32 load_disconnect_reconnect_page(cbs_buf *p_cbuf, uint8 *p_buf, uint32 n_bytes);
    uint32 load_format_parameters_page(cbs_buf *p_cbuf, uint8 *p_buf, uint32 n_bytes);
    uint32 load_disk_geometry_page(cbs_buf *p_cbuf, uint8 *p_buf, uint32 n_bytes);
    uint32 load_cacheing_parameters_page(cbs_buf *p_cbuf, uint8 *p_buf, uint32 n_bytes);
    uint32 load_control_page(cbs_buf *p_cbuf, uint8 *p_buf, uint32 n_bytes);
    uint32 load_info_exceptions_page(cbs_buf *p_cbuf, uint8 *p_buf, uint32 n_bytes);
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
extern uint32 target_LoadModePage(cbs_buf_t *p_cbuf, uint8 *p_buf, uint32 nbytes);

extern void target_QueueCbuf(CDevice *p_dev,  cbs_buf_t *p_cbuf);
extern void target_DeviceScan(CDevice *p_dev);

#endif
