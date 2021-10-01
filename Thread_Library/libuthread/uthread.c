#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "queue.h"
#include "private.h"
#include "uthread.h"

#define ACTIVE 1
#define BLOCKED 0
#define ZOMBIE -1

// Global variable queue for round-robin scheduling of threads.
queue_t scheduler;
// Global variable to keep track of TIDs (main TID = 0).
uthread_t TIDcount = 0;
// Global variable to keep track of current thread.
struct TCB *currentThread;
// Global variable to keep track if preempt_start used.
int preempt_call;
// Global variable for main's context;
ucontext_t mainContext;


// Thread Control Block (TCB) Data Structure.
struct TCB {
	uthread_t TID;
	int state;
	uthread_ctx_t *context;
	void *sp;
	int returnValue;
	int being_joined;
};

int uthread_start(int preempt)
{
	if (preempt) {
		preempt_start();
		preempt_call = 1;
	}

	// Initialize queues.
	scheduler = queue_create();
	if (!scheduler)
		return -1;

	currentThread = malloc(sizeof(*currentThread));
	if (!currentThread)
		return -1;

	if (TIDcount == USHRT_MAX)
		return -1;

	currentThread->TID = TIDcount++;
	currentThread->state = ACTIVE;
	currentThread->context = &mainContext;	
	return 0;
}

int uthread_stop(void)
{
	uthread_ctx_destroy_stack(currentThread->sp);
	free(currentThread);
	currentThread = NULL;

	if (queue_destroy(scheduler) == -1)
		return -1;

	if (preempt_call)
		preempt_stop();

	return 0;
}

int uthread_create(uthread_func_t func)
{
	// Disable preemption when adding new thread to scheduler.
	if (preempt_call)
		preempt_disable();

	struct TCB *thread = malloc(sizeof(*thread));
	if (!thread)
		return -1;

	if (TIDcount == USHRT_MAX)
		return -1;

	thread->TID = TIDcount++;
	thread->state = ACTIVE;
	thread->sp = uthread_ctx_alloc_stack();

	if (!thread->sp)
		return -1;
	
	ucontext_t context;
	thread->context = &context;

	if (uthread_ctx_init(thread->context, thread->sp, func) != 0)
		return -1;

	queue_enqueue(scheduler, thread);

	// Resume preemption.
	if (preempt_call)
		preempt_enable();

	return thread->TID;
}

void uthread_yield(void)
{
	// Need >= 1 thread in scheduler to perform yield.
	if (queue_length(scheduler) < 1)
		return;

	// Critical section! Manipulates global currentThread.
	if (preempt_call)
		preempt_disable();

	struct TCB *prevThread = currentThread;
	queue_dequeue(scheduler, (void **)&currentThread);
	queue_enqueue(scheduler, prevThread);
	uthread_ctx_switch(prevThread->context, currentThread->context);
}

uthread_t uthread_self(void)
{
	return currentThread->TID;
}

void uthread_exit(int retval)
{
	// Critical section! Can possibly change currentThread.
	if (preempt_call)
		preempt_disable();

	currentThread->state = ZOMBIE;
	currentThread->returnValue = retval;

	if (preempt_call)
		preempt_enable();

	uthread_yield();
}

/* Callback function that finds matching TID */
// Updates data to have pointer to thread with matching TID.
static int find_TID(queue_t q, void *data, void *arg)
{
    struct TCB *thread = (struct TCB *)data;
	uthread_t TID = thread->TID;
    uthread_t match = (uthread_t)(long)arg;
    (void)q; //unused

    if (TID == match)
        return 1;

    return 0;
}

int uthread_join(uthread_t tid, int *retval)
{
	// Critical section! Disable preempt to get expected currentThread.
	if (preempt_call)
		preempt_disable();

	struct TCB *callingThread = currentThread;
	struct TCB *ptr;
	queue_iterate(scheduler, find_TID, (void *)(long)tid, (void **)&ptr);

	if (ptr->TID != tid)
		return -1;

	if (tid == 0 || tid == callingThread->TID || ptr->being_joined == 1) {
		return -1;
	}
	ptr->being_joined = 1;

	if (preempt_call)
		preempt_enable();

	while (1) {
		if (ptr->state != ZOMBIE) {
			callingThread->state = BLOCKED;
		} else {
			if (callingThread->state == BLOCKED) {
				callingThread->state = ACTIVE; 
			}
			if (retval) 
				*retval = ptr->returnValue;
			
			queue_delete(scheduler, ptr);
			uthread_ctx_destroy_stack(ptr->sp);
			free(ptr); 
			ptr = NULL;
			uthread_yield();
			break;
		}
		if (currentThread->state == BLOCKED) {
			uthread_yield();
		}
	}
	return 0;
}
