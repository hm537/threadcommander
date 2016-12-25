#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "threadcommander.h"


static void TestThreadProc1(void *arg) {
	THREAD_INFO_T * pThreadInfo = (THREAD_INFO_T *)arg;
	while(1)
	{
		threadcommander_waitctrl(pThreadInfo);

		fprintf(stderr, "TestThreadProc1 run!\r\n");
		sleep(1);
	}
}

static void TestThreadProc2(void *arg) {
	THREAD_INFO_T * pThreadInfo = (THREAD_INFO_T *)arg;
	while(1)
	{
		threadcommander_waitctrl(pThreadInfo);

		fprintf(stderr, "TestThreadProc2 run!\r\n");
		sleep(1);
	}
}

static void TestThreadProc3(void *arg) {
	THREAD_INFO_T * pThreadInfo = (THREAD_INFO_T *)arg;
	while(1)
	{
		threadcommander_waitctrl(pThreadInfo);

		fprintf(stderr, "TestThreadProc3 run!\r\n");
		sleep(1);
	}
}




int main(int argc, char **argv)
{
	assert(threadcommander_ok == threadcommander_init(2));
	assert(threadcommander_ok ==threadcommander_create("testthread1", TestThreadProc1, NULL, 0, 1));
	fprintf(stderr, "testthread1 create but no start, wait 5 sec!\r\n");
	sleep(5);
	assert(threadcommander_ok ==threadcommander_ctrl("testthread1", 1));
	fprintf(stderr, "testthread1 start, wait 5 sec!\r\n");
	sleep(5);
	assert(threadcommander_ok ==threadcommander_create("testthread2", TestThreadProc2, NULL, 1, 1));
	fprintf(stderr, "testthread2 create and start, wait 5 sec!\r\n");
	sleep(5);
	
	assert(threadcommander_queue_full ==threadcommander_create("testthread3", TestThreadProc3, NULL, 1, 1));
	fprintf(stderr, "testthread3 create but queue full!\r\n");

	assert(threadcommander_ok ==threadcommander_ctrl("testthread1", 0));
	fprintf(stderr, "testthread1 pause, wait 5 sec!\r\n");
	sleep(5);
	assert(threadcommander_ok ==threadcommander_ctrl("testthread2", 0));
	fprintf(stderr, "testthread2 pause, wait 5 sec!\r\n");
	sleep(5);
	assert(threadcommander_err ==threadcommander_ctrl("testthread3", 0));
	fprintf(stderr, "testthread3 not found, return err\r\n");

	assert(threadcommander_ok ==threadcommander_ctrl("testthread1", 1));
	fprintf(stderr, "testthread1 restart, wait 5 sec!\r\n");
	sleep(5);
	assert(threadcommander_ok ==threadcommander_ctrl("testthread2", 1));
	fprintf(stderr, "testthread2 restart, wait 5 sec!\r\n");
	sleep(5);

	assert(threadcommander_ok ==threadcommander_kill("testthread1"));
	fprintf(stderr, "testthread1 kill, wait 5 sec!\r\n");
	sleep(5);
	assert(threadcommander_ok ==threadcommander_kill("testthread2"));
	assert(threadcommander_ok ==threadcommander_kill("testthread3"));

	fprintf(stderr, "kill all thread!\r\n");

    return 0;
}


