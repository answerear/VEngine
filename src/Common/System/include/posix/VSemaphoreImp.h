

#ifndef __VISION_SEMAPHORE_IMP_H__
#define __VISION_SEMAPHORE_IMP_H__


#include "ISemaphoreImp.h"
#include <semaphore.h>


class VSemaphoreImp : public ISemaphoreImp
{
public:
	VSemaphoreImp(VLONG count = 1, VCHAR *name = VNULL);
	virtual ~VSemaphoreImp();

private:
	VSemaphoreImp(const VSemaphoreImp &);
	VSemaphoreImp &operator =(const VSemaphoreImp &);

public:
	VBOOL lock(VDWORD timeout = -1);
	VBOOL unlock();
	VBOOL unlock(VLONG count, VLONG *prevCount = 0);

protected:
	sem_t	mSem;
};


#endif	/*__VISION_SEMAPHORE_IMP_H__*/
