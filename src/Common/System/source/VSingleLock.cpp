

#include "VSingleLock.h"


VSingleLock::VSingleLock(ISyncObject *object, VBOOL initLock /* = VFALSE */)
	: mObject(object)
	, mIsLocked(VFALSE)
{
	if (initLock)
	{
		mObject->lock();
		mIsLocked = VTRUE;
	}
}

VSingleLock::~VSingleLock()
{
	if (isLocked())
	{
		mObject->unlock();
		mIsLocked = VFALSE;
	}
}

VBOOL VSingleLock::lock(VDWORD timeout /* = -1 */)
{
	VBOOL result = VFALSE;

	if (VNULL != mObject)
	{
		result = mObject->lock(timeout);
	}

	return result;
}

VBOOL VSingleLock::unlock()
{
	VBOOL result = VFALSE;

	if (VNULL != mObject)
	{
		result = mObject->unlock();
	}

	return result;
}
