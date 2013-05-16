#ifndef CBS_DEVICE_H
#define CBS_DEVICE_H

#include <string>
#include <string.h>

#include "cbs_types.h"
#include "cbs_util.h"
#include "cbs_timer.h"
#include "cbs_scsi.h"
#include "cbs_buf.h"

#define CBS_MAX_UNINIT_DEVICES 1
#define CBS_MAX_DISK_DEVICES    48
#define CBS_MAX_NULL_DEVICES    1
#define CBS_MAX_TSSD_DEVICES    CBS_MAX_DISK_DEVICES
#define CBS_MAX_TFS_DEVICES     CBS_MAX_DISK_DEVICES
#define CBS_MAX_THREAD_DEVICES  8
#define CBS_MAX_CONFIG_DEVICES  1

#define CBS_MAX_DEVICES (CBS_MAX_UNINIT_DEVICES + \
                        CBS_MAX_DISK_DEVICES + \
                        CBS_MAX_NULL_DEVICES + \
                        CBS_MAX_TSSD_DEVICES + \
                        CBS_MAX_TFS_DEVICES + \
                        CBS_MAX_THREAD_DEVICES + \
                        CBS_MAX_CONFIG_DEVICES)

#define CBS_DEVICE_FREE                 0x00000000
#define CBS_DEVICE_STOP                 0x00000001
#define CBS_DEVICE_GONE                 0x00000002
#define CBS_DEVICE_ABORT                0x00000004
#define CBS_DEVICE_ALLOCED              0x00000008
#define CBS_DEVICE_CHECK                0x00000010

class CDevice
{
public:
	CDevice(uint32 class_id, uint32 device_no, uint16 max_pending, const char* name):_total_cmd(0),_sequence(0),_waitings(0),_pendings(0),_last_started(0)
	{
		qu_QueueInit(&_cbufs, CBUF_DEVICE_Q_HEAD);
		qu_QueueInit(&_cbufs_pending, CBUF_DEVICE_PENDING_Q_HEAD);
		spin_lock_init(&_q_lock);

        _class_id = class_id;
        _index = device_no;
        _max_pending = max_pending;
        _state = CBS_DEVICE_FREE;
        _name.assign(name);
	}
	virtual ~CDevice(){}
	
public:
	qu_queue_t _cbufs;
	qu_queue_t _cbufs_pending;
	spinlock_t _q_lock;
	
    uint32 _class_id;
	uint32 _index;
	uint32 _total_cmd;
	uint16 _sequence;
	uint16 _waitings;
    uint16 _pendings;
	
	CTimerQueue _q_timer;
	time_t _last_started;

    scsi_sense_info_t _latest_sense;

    std::string _name;
    uint32 _state;
    uint16 _max_pending;
public:
    virtual void cmd_done(cbs_buf_t* p_cbuf);
    inline void clear_sense_info()
    {
        memset(&_latest_sense, 0, sizeof(scsi_sense_info_t));
    }
    inline uint32 get_device_no()
    {
        return _index;
    }
    inline void add_timer(tm_timer_t* p_timer)
    {
        _q_timer.add_timer(p_timer);
    }
    inline void check_expire()
    {
        _q_timer.check_expire();
    }
};

class CDevicePool
{
public:
    CDevicePool();
    virtual~ CDevicePool()
    {
        if (_uninit_dev != NULL)
        {
            delete _uninit_dev;
        }
    }

private:
    CDevice* G_devices[CBS_MAX_DEVICES];
    CDevice* _uninit_dev;

public:
    CDevice* _get_device_by_index(uint32 index);
    RT_STATUS _device_register(CDevice* p_dev);
    void _device_unregister(CDevice *p_dev);
};

extern CDevice* get_device_by_index(uint32 index);

#endif
