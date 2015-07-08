

#include "VCriticalSectionImp.h"
#include <stdio.h>


VCriticalSectionImp::VCriticalSectionImp()
{
	pthread_mutex_init(&mMutex, 0);
}

VCriticalSectionImp::~VCriticalSectionImp()
{
	pthread_mutex_destroy(&mMutex);
}

VBOOL VCriticalSectionImp::lock(VDWORD dwTimeOut /* = -1 */)
{
	if (pthread_mutex_lock(&mMutex))
	{
		return VFALSE;
	}

	return VTRUE;
}

VBOOL VCriticalSectionImp::unlock()
{
	if (pthread_mutex_unlock(&mMutex))
	{
		return VFALSE;
	}

	return VTRUE;
}

