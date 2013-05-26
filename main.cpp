#include <iostream>
#include <unistd.h>
#include "cbs_target.h"
#include "cbs_util.h"
#include "cbs_mem.h"

void* func(void * data)
{
    printf("thread self:%d\n", (int)pthread_self());
    return NULL;
}

int main(int argc, char **argv)
{
    CMEMPOOL pool(64*1024, 1024);
    uint8* p_buf[1025];
    if (pool.init() != RT_OK)
    {
        printf("pool init failed\n");
    }

    for (int i = 0; i < 1025;i++)
    {
        p_buf [i] = pool.chunk_alloc();
        if (p_buf[i] == NULL)
        {
       //     printf("alloc chunk failed:%d\n", i);
        }
        else
        {
    //        printf("alloc chunk sucessful:%d\n", i);
        }
    }

    for (int i = 0; i < 1025; i++)
    {
        if (p_buf[i] != NULL)
        {
        //    printf("free buf:%d\n", i);
            pool.chunk_free(p_buf[i]);
        }
    }

    for (int i = 0; i < 1024; i++)
    {
        p_buf[0] = pool.chunk_alloc();
        if (p_buf[0] != NULL)
        {
         //   printf("alloc sucessful, now, free it:%d\n", i);
            pool.chunk_free(p_buf[0]);
        }
    }

    pthread_t tid;
    int ret = 0;
    ret = pthread_create(&tid, NULL, func, NULL);
    if (ret == 0)
    {
       printf("thread id:%d\n", (int)tid);
    }
    else
    {
        printf("thread create failed\n");
    }

    pthread_join(tid, NULL);
    return 0;
}
