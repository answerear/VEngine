

#ifndef __VISION_ICRITICAL_SECTION_IMP_H__
#define __VISION_ICRITICAL_SECTION_IMP_H__


#include "VSystemConfig.h"


class ICriticalSectionImp
{
public:
	virtual VBOOL lock(VDWORD timeout = -1) = 0;
	virtual VBOOL unlock() = 0;
};


#endif	/*__VISION_ICRITICAL_SECTION_IMP_H__*/
