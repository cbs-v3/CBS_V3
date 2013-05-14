#include <iostream>
#include <unistd.h>
#include "cbs_target.h"
#include "cbs_util.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
CTargetManager mg;
static int flag = 1;
static void func(void *data)
{
	flag = 0;
	std::cout<<"timeout"<<std::endl;
}

int main(int argc, char** argv) {
	CTarget my_target;
	mg.target_ClassInit(TARGET_CLASS_NULL,0,"my_test", &my_target);
	
	cbs_buf_t* p_cbuf = cbuf_alloc(0);
	if (p_cbuf!=NULL) 
	{
		printf("seq:%u\n", MAKE_CBUF_HANDLE(p_cbuf));
		cbuf_free(p_cbuf);
	}
	
	p_cbuf = cbuf_alloc(0);
	if (p_cbuf!=NULL) 
	{
		uint32 handle = MAKE_CBUF_HANDLE(p_cbuf);
		printf("index:%u, sequence:%u, handle:%u\n", GET_CBUF_INDEX_BY_HANDLE(handle), GET_CBUF_SEQUENCE_BY_HANDLE(handle), handle);
		printf("addr:%p, handle addr:%p\n", p_cbuf, get_cbuf_by_handle(handle));
		
		cbuf_free(p_cbuf);
	}
	
	tm_timer_t my_timer;
	tm_timer_set(&my_timer, func, 20, NULL);
	CTimerQueue timer_q;
	timer_q.add_timer(&my_timer);
	while(flag)
	{
		timer_q.check_expire();
		Sleep(1000);
		if(timer_q.timer_running(&my_timer))
			printf("timer running\n");
		else
			printf("timer not running\n");
	}
	
	if(timer_q.timer_running(&my_timer))
		printf("timer running\n");
	else
		printf("timer not running\n");
	
	return 0;
}
