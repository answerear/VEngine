

#include "VSemaphoreImp.h"


VSemaphoreImp::VSemaphoreImp(VLONG lCount /* = 1 */, VCHAR *pszName /* = 0 */)
{
	sem_init(&mSem, 0, lCount);
}

VSemaphoreImp::~VSemaphoreImp()
{
	sem_destroy(&mSem);
}

VBOOL VSemaphoreImp::lock(VDWORD dwTimeOut /* = -1 */)
{
	if ((VDWORD)-1 == dwTimeOut)
	{
		if (sem_wait(&mSem))
		{
			return VFALSE;
		}
	}
	else
	{
#if 0
		if (sem_timedwait(&mSem, dwTimeOut))
		{
			return VFALSE;
		}
#endif
	}

	return VTRUE;
}

VBOOL VSemaphoreImp::unlock()
{
	if (sem_post(&mSem))
	{
		return VFALSE;
	}

	return VTRUE;
}

VBOOL VSemaphoreImp::unlock(VLONG lCount, VLONG *lpPrevCount /* = 0 */)
{
	if (0 != lpPrevCount)
	{
		if (sem_getvalue(&mSem, reinterpret_cast<int *> (lpPrevCount)))
		{
			return VFALSE;
		}
	}

	sem_post(&mSem);

	if (sem_getvalue(&mSem, reinterpret_cast<int *> (&lCount)))
	{
		return VFALSE;
	}

	return VTRUE;
}
