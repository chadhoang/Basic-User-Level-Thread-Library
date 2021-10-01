#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void)
{
	int i = 0;
    while (i < 1000000000) {
		if (i % 5000000 == 0)
			printf("thread%d\n", uthread_self());
        i++;
    }
	return 0;
}

int thread2(void)
{
	int i = 0;
    while (i < 1000000000) {
		if (i % 5000000 == 0)
			printf("thread%d\n", uthread_self());
        i++;
    }
	return 0;
}

int thread1(void)
{	
	int i = 0;
    while (i < 1000000000) {
		if (i % 5000000 == 0)
			printf("thread%d\n", uthread_self());
        i++;
    }
	return 0;
}

int main(void)
{
    uthread_start(1);
	uthread_t tid1 = uthread_create(thread1);
	uthread_t tid2 = uthread_create(thread2);
	uthread_t tid3 = uthread_create(thread3);
	uthread_join(tid1, NULL);
	uthread_join(tid2, NULL);
	uthread_join(tid3, NULL);
	uthread_stop();
	return 0;
}
