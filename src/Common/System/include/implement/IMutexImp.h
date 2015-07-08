

#ifndef __VISION_IMUTEX_IMP_H__
#define __VISION_IMUTEX_IMP_H__


#include "VSystemConfig.h"


class IMutexImp
{
public:
	virtual VBOOL lock(VDWORD timeout = -1) = 0;
	virtual VBOOL unlock() = 0;
};


#endif	/*__VISION_IMUTEX_IMP_H__*/
