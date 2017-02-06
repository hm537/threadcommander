#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "threadcommander.h"

/**
 *  @struct THREAD_INFO_T
 *  @brief The thread info struct
 *
 *  @var userParam			user param
 *  @var threadID			thread id
 */
typedef struct{
	THREAD_PARAM_T userParam;
	pthread_t threadID;
}THREAD_INFO_T;

static void ThreadCleanUpRoutine(void * arg)
{
	THREAD_PARAM_T * pUserParam = &((THREAD_INFO_T *)arg)->userParam;
	if(pUserParam->pMutex)
		pthread_mutex_unlock(pUserParam->pMutex);
	if(pUserParam->cbCleanUpRoutine)
		pUserParam->cbCleanUpRoutine(pUserParam->threadArg);
}

static void * ThreadStartRoutine(void * arg)
{
	THREAD_PARAM_T * pUserParam = &((THREAD_INFO_T *)arg)->userParam;
	int excuteCnt = 0;
	int siDelayS = pUserParam->delayMs/1000;
	int siDelayUs = (pUserParam->delayMs%1000) * 1000;
	
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	
	pthread_cleanup_push(ThreadCleanUpRoutine, arg);
	while(1)
	{
		/*ensure deal cancel*/ 
		pthread_testcancel();

		if(pUserParam->pMutex)
			pthread_mutex_lock(pUserParam->pMutex);

		/*run user work!*/ 
		pUserParam->run(pUserParam->threadArg);

		if(pUserParam->pMutex)
			pthread_mutex_unlock(pUserParam->pMutex);

		/*delay time*/
		if(siDelayS)
		{
			sleep(siDelayS);
			pthread_testcancel();
		}
		if(siDelayUs)
			usleep(siDelayUs);
	
		/*run counter*/
		if(pUserParam->runTimes&& ++excuteCnt >= pUserParam->runTimes)
			pthread_exit("thread exit!");

		pthread_testcancel();
	}
	
	pthread_cleanup_pop(0);

}

static int bUserParamOK(THREAD_PARAM_T * pUserParam)
{
	if(
		!pUserParam->run ||
		pUserParam->runTimes < 0 ||
		pUserParam->delayMs < 0)
		return 0;
	return 1;
}

THREAD_HANDLE mthread_create(THREAD_PARAM_T * param)
{
	THREAD_INFO_T * handle = 0;
	if(!param || !bUserParamOK(param))
		return 0;

	handle = (THREAD_INFO_T *)calloc(1, sizeof(THREAD_INFO_T));
	
	if(!handle)
		return 0;

	memcpy(&handle->userParam, param, sizeof(THREAD_PARAM_T));
	
	if(-1 == pthread_create(&handle->threadID, NULL, ThreadStartRoutine, (void *)handle))
	{
		memset(handle, 0, sizeof(THREAD_INFO_T));
		free(handle);
		return 0;
	}
	
	return (THREAD_HANDLE)handle;
}


int mthread_kill(THREAD_HANDLE handle)
{
	THREAD_INFO_T * pThreadInfo = (THREAD_INFO_T *)handle;
	
	if(!pThreadInfo || !pThreadInfo->threadID)
		return -1;
	
	pThreadInfo = (THREAD_INFO_T *)handle;

	while(1)
	{/*cancel thread repeatedly to Interrupt blocking*/
		if(ESRCH == pthread_kill(pThreadInfo->threadID,0))
			break;
		pthread_cancel(pThreadInfo->threadID);
		usleep(5*1000);
	}

	memset(pThreadInfo, 0, sizeof(THREAD_INFO_T));
	free(pThreadInfo);
	
	return 0;
}

