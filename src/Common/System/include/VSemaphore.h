

#ifndef __VISION_SEMAPHORE_H__
#define __VISION_SEMAPHORE_H__


#include "VsystemConfig.h"


class ISemaphoreImp;


class VSYSTEM_DLL VSemaphore
{
public:
	VSemaphore(VLONG count = 1, VCHAR *name = VNULL);
	virtual ~VSemaphore();

private:
	VSemaphore(const VSemaphore &);
	VSemaphore &operator =(const VSemaphore &);

public:
	VBOOL lock(VDWORD timeout = -1);
	VBOOL unlock();
	VBOOL unlock(VLONG count, VLONG *prevCount = 0);

protected:
	ISemaphoreImp	*mHandle;
};



#endif	/*__VISION_SEMAPHORE_H__*/
