#ifndef _THREAD_COMMANDER_H_
#define _THREAD_COMMANDER_H_

#include <pthread.h>
#include <semaphore.h>

typedef enum {
	threadcommander_ok			= 0,
    threadcommander_err			= -1,
    threadcommander_param_err	= -2,
    threadcommander_queue_full	= -3,
    threadcommander_ctl_unable	= -4
} threadpool_error_t;


typedef struct{
	int times;//run() excute times, 0 - infinite loop
	int delayMs;//run() delay time
	pthread_mutex_t * pMutex;
	void (*run)(void * priParam);
	void ** pthreadPriParamPoint;//point to thread private param point 
	void (*priParamDestructor)(void * priParam);
}THREAD_PARAM_T;

typedef void * THREAD_HANDLE;


THREAD_HANDLE threadcommander_create(THREAD_PARAM_T * param);
void threadcommander_start(THREAD_HANDLE handle);
int threadcommander_stop(THREAD_HANDLE handle);
int threadcommander_destroy(THREAD_HANDLE handle);

#endif
