

#ifndef __VISION_MUTEX_IMP_H__
#define __VISION_MUTEX_IMP_H__


#include "IMutexImp.h"
#include <pthread.h>


class VMutexImp : public IMutexImp
{
public:
	VMutexImp(VBOOL recursive = VTRUE, VCHAR *name = VNULL);
	virtual ~VMutexImp();

private:
	VMutexImp(const VMutexImp &);
	VMutexImp &operator =(const VMutexImp &);

public:
	VBOOL lock(VDWORD timeout = -1);
	VBOOL unlock();

protected:
	pthread_mutex_t	mMutex;
};


#endif	/*__VISION_MUTEX_IMP_H__*/
