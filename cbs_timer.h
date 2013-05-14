#ifndef CBS_TIMER_H
#define CBS_TIMER_H

#include <time.h>
#include "cbs_types.h"
#include "cbs_list.h"
#include "cbs_util.h"

typedef void (*expire_callback_fn)(void*);
typedef struct tm_timer
{
	qu_queue_t links;
	time_t access_time;
	time_t gap;
	expire_callback_fn on_expire; //notice:this function was called in spinlock context
	void* data; //private data
}tm_timer_t;

/*
* @brief tm_timer_set -set the params of the timer
* @author raidmanji (2013/04/23)
* @param x: timer to be set, the pointer of the timer
*		 f: callback function, will be called when the timer expired, called in spinlock context
*		 y: time that expire on
*		 z: private data, passed to callback function
*/
#define tm_timer_set(x, f, y, z) \
	do { \
		(x)->on_expire = (f); \
		(x)->access_time = time(NULL); \
		(x)->gap = (y); \
		(x)->data = (z); \
	}while(0)

class CTimerQueue
{
public:
	CTimerQueue()
	{
		qu_QueueInit(&_timer_q, TM_TIME_Q_HEAD);
		spin_lock_init(&_lock);
	}
	virtual ~CTimerQueue(){}
	
private:
	spinlock_t _lock;
	qu_queue_t _timer_q;
	
public:
	RT_STATUS add_timer(tm_timer_t* p_timer);
	RT_STATUS del_timer(tm_timer_t* p_timer);
	bool timer_running(tm_timer_t* p_timer); 
	//check the timer queue, call the expire function 
	//if the timer expired and delete it from the queue
	void check_expire();	
};

#endif
