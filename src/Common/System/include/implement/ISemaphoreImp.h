

#ifndef __VISION_ISEMAPHORE_IMP_H__
#define __VISION_ISEMAPHORE_IMP_H__


#include "VSystemConfig.h"


class ISemaphoreImp
{
public:
	virtual VBOOL lock(VDWORD timeout = -1) = 0;
	virtual VBOOL unlock() = 0;
	virtual VBOOL unlock(VLONG count, VLONG *prevCount) = 0;
};

#endif	/*__VISION_ISEMAPHORE_IMP_H__*/
