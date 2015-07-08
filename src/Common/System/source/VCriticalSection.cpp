

#include "VSystem.h"
#include "ISystemFactory.h"
#include "VCriticalSection.h"
#include "ICriticalSectionImp.h"


VCriticalSection::VCriticalSection()
	: mHandle(VNULL)
{
	mHandle = VSYSTEM_FACTORY.createCriticalSection();
}

VCriticalSection::~VCriticalSection()
{
	delete mHandle;
	mHandle = VNULL;
}

VBOOL VCriticalSection::lock(VDWORD timeout /* = -1 */)
{
	if (VNULL == mHandle)
	{
		return VFALSE;
	}

	return mHandle->lock(timeout);
}

VBOOL VCriticalSection::unlock()
{
	if (VNULL == mHandle)
	{
		return VFALSE;
	}

	return mHandle->unlock();
}
