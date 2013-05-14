#include "cbs_timer.h"
#include "cbs_debug.h"

RT_STATUS CTimerQueue::add_timer(tm_timer_t* p_timer)
{
	spin_lock(&_lock);
	if (p_timer->links.flag == TM_TIME_Q)
	{
		LOG_ERROR("add to timer queue failed, because the timer is running");
		spin_unlock(&_lock);
		
		return RT_FAIL;
	}
	
	qu_EnQueueTail(&p_timer->links, &_timer_q, TM_TIME_Q);
	spin_unlock(&_lock);
	
	return RT_OK;
}

RT_STATUS CTimerQueue::del_timer(tm_timer_t* p_timer)
{
	qu_queue_t* p_links_tmp;
	
	spin_lock(&_lock);
	if (p_timer->links.flag != TM_TIME_Q)
	{
		LOG_ERROR("del from timer queue failed, because the timer is not running");
		spin_unlock(&_lock);
		
		return RT_FAIL;
	}
	
	qu_ForEach(p_links_tmp, &_timer_q)
	{
		if (p_links_tmp == &p_timer->links)
		{
			qu_DeQueue(&p_timer->links, TM_TIME_Q);
			break;
		}
	}
	
	spin_unlock(&_lock);
	
	return RT_OK;	
}

bool CTimerQueue::timer_running(tm_timer_t* p_timer)
{
	bool ret = false;
	qu_queue_t* p_links_tmp;
	
	spin_lock(&_lock);
	if (p_timer->links.flag == TM_TIME_Q)
	{
		qu_ForEach(p_links_tmp, &_timer_q)
		{
			if (p_links_tmp == &p_timer->links)
			{
				ret = true;
				break;
			}
		}
	}
	spin_unlock(&_lock);
	
	return ret;
}
	
void CTimerQueue::check_expire()
{
	tm_timer_t* p_timer, *p_timer_tmp;
	time_t current_time = time(NULL);
	
	spin_lock(&_lock);
	qu_ForEachEntrySaft(p_timer, p_timer_tmp, &_timer_q, links)
	{
		if ((current_time - p_timer->access_time) > p_timer->gap)
		{
			qu_DeQueue(&p_timer->links, TM_TIME_Q);
			p_timer->on_expire(p_timer->data); //this function was called in spinlock context
		}
	}
	spin_unlock(&_lock);
	
	return;
}

