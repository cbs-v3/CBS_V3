#ifndef CBS_LIST_H
#define CBS_LIST_H

#include <stddef.h>
#include "cbs_types.h"
#include "cbs_debug.h"

/**
 * Queue flag definitions
 */
#define QUEUE_FREE_ELEMENT              0x0000
#define QUEUE_HEAD_FLAG                 0x8000

#define CBUF_FREE_Q                     0x0200
#define CBUF_FREE_Q_HEAD                0x8200

#define CBUF_DEVICE_Q					0x0201
#define CBUF_DEVICE_Q_HEAD				0x8201
#define CBUF_DEVICE_PENDING_Q			0x0202
#define CBUF_DEVICE_PENDING_Q_HEAD		0x8202

#define TM_TIME_Q						0x0300
#define TM_TIME_Q_HEAD					0x8300



#define DEBUG_QUEUE

typedef struct qu_queue
{
    struct qu_queue *fwd;   /* Points to next entry on the queue */
    struct qu_queue *bwd;   /* Points to previous entry on the queue */
    uint32          flag;   /* Flag, for debug */
}qu_queue_t;

static inline void __qu_EnQueue(qu_queue_t* new1, qu_queue_t *bwd, qu_queue_t *fwd)
{
    fwd->bwd = new1;
    new1->fwd = fwd;
    new1->bwd = bwd;
    bwd->fwd = new1;   
}

static inline void __qu_DeQueue(qu_queue_t *bwd, qu_queue_t *fwd)
{
    fwd->bwd = bwd;
    bwd->fwd = fwd;
}

static inline void qu_QueueInit(qu_queue_t *entry, uint32 flag)
{
    entry->fwd = entry;
    entry->bwd = entry;
    entry->flag = flag;
    
#ifdef DEBUG_QUEUE
	if ((flag & QUEUE_HEAD_FLAG) == 0)
		LOG_ERROR("Error queue head flag:0x%x", flag);
#endif 
}

static inline void qu_HookInit(qu_queue_t *entry)
{
    entry->fwd = NULL;
    entry->bwd = NULL;
    entry->flag = 0;
}

static inline void qu_EnQueue(qu_queue_t *new1, qu_queue_t *head, uint32 flag)
{
    __qu_EnQueue(new1, head, head->fwd);
    new1->flag = flag;

#ifdef DEBUG_QUEUE
	if ((flag & QUEUE_HEAD_FLAG) != 0)
		LOG_ERROR("error enqueue flag:0x%x", flag);
#endif 
}

static inline void qu_EnQueueTail(qu_queue_t *new1, qu_queue_t *head, uint32 flag)
{
    __qu_EnQueue(new1, head->bwd, head);
    new1->flag = flag;
    
#ifdef DEBUG_QUEUE
	if ((flag & QUEUE_HEAD_FLAG) != 0)
		LOG_ERROR("error enqueue flag:0x%x", flag);
#endif 
}

static inline qu_queue_t *qu_DeQueue(qu_queue_t *entry, uint32 flag)
{
#ifdef DEBUG_QUEUE
	if ((flag & QUEUE_HEAD_FLAG) != 0)
	{
		LOG_ERROR("error dequeue flag:0x%x", flag);
		return (qu_queue_t *)NULL;	
	}
	
	if (entry->flag == QUEUE_FREE_ELEMENT)
	{
		LOG_ERROR("entry %p is free element, not in list:0x%x", entry, flag);
		return (qu_queue_t *)NULL;
	}
	
	if (entry->flag != flag)
	{
		LOG_ERROR("entry %p is in list 0x%x, not in list:0x%x", entry, entry->flag, flag);
		return (qu_queue_t *)NULL;
	}		
#endif 
	
    __qu_DeQueue(entry->bwd, entry->fwd);
    entry->fwd = entry;
    entry->bwd = entry;
    entry->flag = QUEUE_FREE_ELEMENT;

    return entry;
}

static inline bool qu_IsEmpty(qu_queue_t *entry)
{
    return (entry->bwd == entry) && (entry->fwd == entry);
}

static inline qu_queue_t *qu_GetHead(qu_queue_t *entry)
{
    if(qu_IsEmpty(entry)) {
        return NULL;
    }

    return entry->fwd;
}

static inline qu_queue_t *qu_GetHeadTail(qu_queue_t *entry )
{
    if(qu_IsEmpty(entry)) {
        return NULL;
    }

    return entry->bwd;
}

#define qu_Entry(ptr, type, member) \
    ((type *)((char *)(ptr)-offsetof(type,member)))
    
#define qu_ForEach(pos, head) \
    for (pos = (head)->fwd, __builtin_prefetch(pos->fwd,0,1); \
	 pos != (head); \
	 pos = pos->fwd, __builtin_prefetch(pos->fwd,0,1))
    
#define qu_ForEachSafe(pos, next, head) \
    for (pos = (head)->fwd, next = pos->fwd; pos != (head); \
        pos = next, next = pos->fwd)

#define qu_ForEachEntry(pos, head, member) \
    for(pos = qu_Entry((head)->fwd, typeof(*pos),  member), \
    __builtin_prefetch(pos->member.fwd,0,1);  \
    &pos->member != (head);  \
    pos = qu_Entry(pos->member.fwd, typeof(*pos),  member), \
    __builtin_prefetch(pos->member.fwd,0,1))

#define qu_ForEachEntrySaft(pos, next, head, member) \
	for (pos = qu_Entry((head)->fwd, typeof(*pos), member),	\
	     next = qu_Entry(pos->member.fwd, typeof(*pos), member);					\
	     &pos->member != (head); 					\
	     pos = next, next = qu_Entry(next->member.fwd, typeof(*pos), member))

#endif
