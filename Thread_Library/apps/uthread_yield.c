/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void)
{
	uthread_yield();
	printf("thread%d\n", uthread_self());
	return 3;
}

int thread2(void)
{
	uthread_t tid3 = uthread_create(thread3);
	uthread_yield();
	printf("thread%d\n", uthread_self());
	int retval3;
	uthread_join(tid3, &retval3);
	printf("return value of thread3 = %d\n", retval3);
	return 2;
}

int thread1(void)
{
	uthread_t tid2 = uthread_create(thread2);
	uthread_yield();
	printf("thread%d\n", uthread_self());
	uthread_yield();
	int retval2;
	uthread_join(tid2, &retval2);
	printf("return value of thread2 = %d\n", retval2);
	return 1;
}

int main(void)
{
	uthread_start(0);
	uthread_t tid1 = uthread_create(thread1);
	int retval1;
	uthread_join(tid1, &retval1);
	printf("return value of thread1 = %d\n", retval1);
	uthread_stop();
	return 0;
}
