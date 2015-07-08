

#ifndef __VISION_SINGLE_LOCK_H__
#define __VISION_SINGLE_LOCK_H__


#include "ISyncObject.h"


class VSYSTEM_DLL VSingleLock
{
public:
	VSingleLock(ISyncObject *object, VBOOL initLock = VFALSE);
	~VSingleLock();

private:
	VSingleLock(const VSingleLock &);
	VSingleLock &operator =(const VSingleLock &);

public:
	VBOOL isLocked() const { return mIsLocked; }

	VBOOL lock(VDWORD timeout = -1);
	VBOOL unlock();

protected:
	ISyncObject	*mObject;
	VBOOL		mIsLocked;
};


#endif	/*__VISION_SINGLE_LOCK_H__*/
