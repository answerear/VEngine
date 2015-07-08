

#include "VMutexImp.h"
#include <time.h>


VMutexImp::VMutexImp(VBOOL bRecursive /* = VTRUE */, VCHAR *pszName /* = 0 */)
{
	pthread_mutexattr_t mutexattr;

	pthread_mutexattr_init(&mutexattr);

	if (bRecursive)
	{
		pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
	}

	pthread_mutex_init(&mMutex, &mutexattr);
	pthread_mutexattr_destroy(&mutexattr);
}

VMutexImp::~VMutexImp()
{
	pthread_mutex_destroy(&mMutex);
}


VBOOL VMutexImp::lock(VDWORD dwTimeOut /* = -1 */)
{
	if ((VDWORD)-1 == dwTimeOut)
	{
		if (pthread_mutex_lock(&mMutex))
		{
			return VFALSE;
		}
	}
	else
	{
#if 0
		struct timespec ts;

		if (clock_gettime(CLOCK_REALTIME, &ts))
		{
			return VFALSE;
		}

		ts.tv_sec += (dwTimeOut / 1000);
		ts.tv_nsec += (dwTimeOut % 1000) / 1000);

		if (pthread_mutex_timedlock(&mMutex, &ts))
			return VFALSE;
#endif
	}

	return VTRUE;
}

VBOOL VMutexImp::unlock()
{
	if (pthread_mutex_unlock(&mMutex))
	{
		return VFALSE;
	}

	return VTRUE;
}

