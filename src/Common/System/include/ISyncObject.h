
#ifndef __VISION_ISYNC_OBJECT_H__
#define __VISION_ISYNC_OBJECT_H__


#include "VSystemConfig.h"


class ISyncObject
{
public:
	virtual VBOOL lock(VDWORD timeout = -1) = 0;
	virtual VBOOL unlock() = 0;
};


#endif	/*__VISION_ISYNC_OBJECT_H__*/

