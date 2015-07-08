

#include "IThreadObserver.h"
#include "VThreadImp.h"
#include <assert.h>


VThreadImp::VThreadImp(IThreadObserver *pObserver)
: mThread(0)
, mObserver(pObserver)
, mIsRunning(VFALSE)
{
}

VThreadImp::~VThreadImp()
{
}

VRESULT VThreadImp::create(VBOOL bSuspend /* = VFALSE */)
{
	pthread_attr_t attr;
	sched_param param;

	pthread_attr_init(&attr);
#if 1
	if (0)
	{
		pthread_attr_getschedparam(&attr, &param);
		param.sched_priority = 20;
		pthread_attr_setschedparam(&attr, &param);
	}
	else
	{
		int max_priority = 0;
		//		int min_priority = 0;
		//		int policy = 0;

		//		pthread_attr_setschedpolicy(&attr,SCHED_RR);/*设置线程调度策略*/
		//		pthread_attr_getschedpolicy(&attr,&policy);/*取得线程的调度策略*/
		//		if(policy==SCHED_FIFO)
		//			printf("Schedpolicy:SCHED_FIFO\r\n");
		//		if(policy==SCHED_RR)
		//			printf("Schedpolicy:SCHED_RR\n");
		//		if(policy==SCHED_OTHER)
		//			printf("Schedpolicy:SCHED_OTHER\n");

		max_priority = sched_get_priority_max(max_priority);/*获得系统支持的线程优先权的最大值*/
		//		min_priority = sched_get_priority_min(min_priority);/* 获得系统支持的线程优先权的最小值*/
		//		printf("Max priority:%u\n",max_priority);
		//		printf("Min priority:%u\n",min_priority);

		pthread_attr_getschedparam(&attr, &param);
		param.sched_priority = max_priority;
		pthread_attr_setschedparam(&attr, &param);
	}
#endif
	if (pthread_create(&mThread, &attr, VThreadImp::threadProc, this))
	{
		return VERR_FAILED;
	}

	mIsRunning = VTRUE;
	return VERR_NONE;
}

VRESULT VThreadImp::suspend()
{
	assert(0);
	return VERR_NONE;
}

VRESULT VThreadImp::resume()
{
	assert(0);
	return VERR_NONE;
}

VRESULT VThreadImp::terminate(VDWORD exitCode)
{
	if (pthread_cancel(mThread))
	{
		return VERR_FAILED;
	}

	mThread = 0;
	mIsRunning = VFALSE;
	return VERR_NONE;
}

VRESULT VThreadImp::wait(VDWORD dwTimeOut /* = -1 */)
{
	void *ptr;
	if (pthread_join(mThread, &ptr))
	{
		return VERR_FAILED;
	}

	return VERR_NONE;
}

VBOOL VThreadImp::isRunning() const
{
	return mIsRunning;
}

VHANDLE VThreadImp::getThreadID() const
{
	return (VHANDLE)mThread;
}


void *VThreadImp::threadProc(void *pParam)
{
	VThreadImp *pThreadImp = reinterpret_cast<VThreadImp *> (pParam);

	VINT32 nResult = 0;

	if (0 != pThreadImp && 0 != pThreadImp->mObserver)
	{
		nResult = pThreadImp->mObserver->run();
		pThreadImp->mIsRunning = VFALSE;
	}

	return (void*)nResult;
}


