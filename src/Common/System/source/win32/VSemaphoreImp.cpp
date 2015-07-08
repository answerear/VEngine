

#include <windows.h>
#include "VSemaphoreImp.h"


VSemaphoreImp::VSemaphoreImp(VLONG count /* = 1 */, VCHAR *name /* = VNULL */)
{
	mSemaphore = ::CreateSemaphore(NULL, count, count, name);
}

VSemaphoreImp::~VSemaphoreImp()
{
	::CloseHandle(mSemaphore);
	mSemaphore = NULL;
}

VBOOL VSemaphoreImp::lock(VDWORD timeout /* = -1 */)
{
	VDWORD result = ::WaitForSingleObject(mSemaphore, timeout);
	if (WAIT_OBJECT_0 == result)
		return VFALSE;

	return VTRUE;
}

VBOOL VSemaphoreImp::unlock()
{
	if (!::ReleaseSemaphore(mSemaphore, 1, NULL))
		return VFALSE;

	return VTRUE;
}

VBOOL VSemaphoreImp::unlock(VLONG count, VLONG *prevCount /* = 0 */)
{
	if (!::ReleaseSemaphore(mSemaphore, count, prevCount))
		return FALSE;

	return VTRUE;
}
