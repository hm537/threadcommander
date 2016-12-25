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

/**
 *  @struct THREAD_INFO_T
 *  @brief The thread info struct
 *
 *  @var threadName			thread name
 *  @var threadID			thread id
 *  @var start_rtn			thread routine
 *  @var bEnableCtrl		
 *  @var ctrlSem			control thread  pause / restart
 *  @var ctrlRetSem			thread  deal signal thern post this
 *  @var isRun				thread run status
 *  @var arg					thread routine	 param
 */


typedef struct{
	char threadName[64];
	pthread_t threadID;
	void (*start_rtn)(void*);
	int bEnableCtrl;
	sem_t ctrlSem;
	sem_t ctrlRetSem;
	int isRun;
	void *arg;
}THREAD_INFO_T;


int threadcommander_init(int threadMaxNum);
int threadcommander_create(char * pThreadName, void (*start_rtn)(void *), void *arg, int isRun, int bEnableCtrl);
int threadcommander_ctrl(char * pThreadName, int isRun);
int threadcommander_kill(char * pThreadName);
void threadcommander_waitctrl(THREAD_INFO_T * arg);

#endif
