#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 * (Converted to once every 10000 microseconds)
 */
#define HZ 100

struct sigaction sa;
struct itimerval timer;
struct sigaction old;
struct itimerval oldtimer;
sigset_t block;

// Signal handler to force current thread to yield.
void signal_handler(int signum) {
	(void)signum; // Unused.
	uthread_yield();
}

void preempt_start(void)
{
	// Install signal handler.
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);

	// Configure timer.
	// Configure to expire after 100HZ (every 10 ms / 10000 microseconds).
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 1000000 / HZ;

	// Set to expire every 100 HZ after first expire.
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 1000000 / HZ;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);

}

void preempt_stop(void)
{
	// Restore previous signal action.
	sigaction(SIGVTALRM, &old, NULL);

	// Restore previous timer configuration.
	setitimer(ITIMER_VIRTUAL, &oldtimer, NULL);

}

void preempt_enable(void)
{
	// Initialize signal mask.
	sigemptyset(&block);
	sigaddset(&block, SIGVTALRM);

	// Unblock signal.
	sigprocmask(SIG_UNBLOCK, &block, NULL);
}

void preempt_disable(void)
{
	// Initialize signal mask.
	sigemptyset(&block);
	sigaddset(&block, SIGVTALRM);
	
	// Block signal.
	sigprocmask(SIG_BLOCK, &block, NULL);
}

