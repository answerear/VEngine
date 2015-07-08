

#include "VSystem.h"
#include "ISystemFactory.h"
#include "VMutex.h"
#include "IMutexImp.h"


VMutex::VMutex(VBOOL recursive /* = VTRUE */, VCHAR *name /* = VNULL */)
	: mHandle(VNULL)
{
	mHandle = VSYSTEM_FACTORY.createMutex(recursive, name);
}

VMutex::~VMutex()
{
	delete mHandle;
	mHandle = VNULL;
}

VBOOL VMutex::lock(VDWORD timeout /* = -1 */)
{
	if (VNULL == mHandle)
	{
		return VFALSE;
	}

	return mHandle->lock(timeout);
}

VBOOL VMutex::unlock()
{
	if (VNULL == mHandle)
	{
		return VFALSE;
	}

	return mHandle->unlock();
}
