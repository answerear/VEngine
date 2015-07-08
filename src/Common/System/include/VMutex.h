

#ifndef __VISION_MUTEX_H__
#define __VISION_MUTEX_H__


#include "ISyncObject.h"


class IMutexImp;


class VSYSTEM_DLL VMutex : public ISyncObject
{
public:
	VMutex(VBOOL recursive = VTRUE, VCHAR *name = VNULL);
	virtual ~VMutex();

private:
	VMutex(const VMutex &);
	VMutex &operator =(const VMutex &);

public:
	VBOOL lock(VDWORD timeout = -1);
	VBOOL unlock();

private:
	IMutexImp	*mHandle;
};


#endif
