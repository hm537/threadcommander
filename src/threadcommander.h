#ifndef _THREAD_COMMANDER_H_
#define _THREAD_COMMANDER_H_
#include <unistd.h>
#include <pthread.h>

/**
 *  @struct THREAD_PARAM_T
 *  @brief The thread param struct
 *
 *  @var run				user wrok process
 *  @var threadArg			run() param
 *  @var cbCleanUpRoutine	if not null, will be called when kill thread
 *  @var pMutex				run() lock
 *  @var runTimes			excute times, 0 - infinite loop
 *  @var delayMs			run() delay time
 */
typedef struct{
	void (*run)(void *);
	void * threadArg;
	void (*cbCleanUpRoutine)(void *);
	pthread_mutex_t * pMutex;
	int runTimes;
	int delayMs;
}THREAD_PARAM_T;

typedef void * THREAD_HANDLE;


THREAD_HANDLE mthread_create(THREAD_PARAM_T * param);
int mthread_kill(THREAD_HANDLE handle);
void * mthread_getpridate(THREAD_HANDLE handle);

#endif
