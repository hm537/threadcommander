#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <malloc.h>
#include "threadcommander.h"

static THREAD_HANDLE handle1 = 0;
static THREAD_HANDLE handle2 = 0;
static int gstTestCnt = 0;

static void TestThreadProcCleanUp(void *arg)
{
	int * pArg = (int *)arg;
	fprintf(stderr, "TestThreadProc1CleanUp, proc run %d times!\r\n",*pArg);
	free(pArg);
	fprintf(stderr, "TestThreadProc1CleanUp, free arg!\r\n");
}

static void TestThreadProc(void *arg) {
	int * pArg = (int *)arg;
	(*pArg) += 1;
	gstTestCnt += 1;
	fprintf(stderr, "thread:%x TestThreadProc run!\r\n", (unsigned int)pthread_self());
}



#if 0
static void TestThreadProc2(void *arg) {
	fprintf(stderr, "TestThreadProc2 run!\r\n");
	sleep(1);
}

static void TestThreadProc3(void *arg) {
	fprintf(stderr, "TestThreadProc3 run!\r\n");
	sleep(1);
}
#endif


int main(int argc, char **argv)
{
	pthread_mutex_t mutex;
	int * ThreadProc1Arg = 0, * ThreadProc2Arg = 0;
	THREAD_PARAM_T param;

	ThreadProc1Arg = (int *)calloc(1, sizeof(int));
	ThreadProc2Arg =  (int *)calloc(1, sizeof(int));
	assert(ThreadProc1Arg != NULL);
	assert(ThreadProc2Arg != NULL);
	*ThreadProc1Arg = 0;
	*ThreadProc2Arg = 0;

	pthread_mutex_init(&mutex, NULL);

	param.runTimes = 20;
	param.delayMs = 1000;
	param.pMutex = &mutex;
	param.run = TestThreadProc;
	param.threadArg = (void *)ThreadProc1Arg;
	param.cbCleanUpRoutine = TestThreadProcCleanUp;
	handle1 = mthread_create(&param);
	assert(handle1 != NULL);

	param.runTimes = 20;
	param.delayMs = 500;
	param.pMutex = NULL;
	param.run = TestThreadProc;
	param.threadArg = (void *)ThreadProc2Arg;;
	param.cbCleanUpRoutine = TestThreadProcCleanUp;
	handle2 = mthread_create(&param);
	assert(handle2 != NULL);
	
	fprintf(stderr, "create 2 thread, let thread1 thread2 run 5 sec!\r\n");
	sleep(5);
	fprintf(stderr, "lock thread1 run() 5 sec!\r\n");
	pthread_mutex_lock(&mutex);
	sleep(5);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);
	fprintf(stderr, "unlock thread1 run()\r\n");
	fprintf(stderr, "let thread1 thread2 run 5 sec!\r\n");
	sleep(5);
	fprintf(stderr, "kill thread1 thread2\r\n");
	assert(0 == mthread_kill(handle1));
	assert(0 == mthread_kill(handle2));

	fprintf(stderr, "gstTestCnt:%d\r\n", gstTestCnt);
	
	#if 0
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

	#endif
    return 0;
}


