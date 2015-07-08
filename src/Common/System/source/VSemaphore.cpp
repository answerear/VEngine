

#include "VSystem.h"
#include "ISystemFactory.h"
#include "VSemaphore.h"
#include "ISemaphoreImp.h"


VSemaphore::VSemaphore(VLONG count /* = 1 */, VCHAR *name /* = VNULL */)
	: mHandle(VNULL)
{
	mHandle = VSYSTEM_FACTORY.createSemaphore(count, name);
}

VSemaphore::~VSemaphore()
{
	delete mHandle;
	mHandle = VNULL;
}

VBOOL VSemaphore::lock(VDWORD timeout /* = -1 */)
{
	if (VNULL == mHandle)
	{
		return VFALSE;
	}

	return mHandle->lock(timeout);
}

VBOOL VSemaphore::unlock()
{
	if (VNULL == mHandle)
	{
		return VFALSE;
	}

	return mHandle->unlock();
}

VBOOL VSemaphore::unlock(VLONG count, VLONG *prevCount /* = 0 */)
{
	if (VNULL == mHandle)
	{
		return VFALSE;
	}

	return mHandle->unlock(count, prevCount);
}
