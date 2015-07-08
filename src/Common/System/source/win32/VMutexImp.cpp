

#include <windows.h>
#include "VMutexImp.h"


VMutexImp::VMutexImp(VBOOL recursive /* = VTRUE */, VCHAR *name /* = VNULL */)
	: mMutex(NULL)
{
	mMutex = ::CreateMutex(NULL, FALSE, name);
}

VMutexImp::~VMutexImp()
{
	if (NULL != mMutex)
	{
		::CloseHandle(mMutex);
		mMutex = NULL;
	}
}

VBOOL VMutexImp::lock(VDWORD timeout /*= -1 */)
{
	DWORD result = ::WaitForSingleObject(mMutex, timeout);
	if (WAIT_OBJECT_0 != result)
		return VFALSE;

	return VTRUE;
}

VBOOL VMutexImp::unlock()
{
	if (!::ReleaseMutex(mMutex))
		return VFALSE;

	return VTRUE;
}
