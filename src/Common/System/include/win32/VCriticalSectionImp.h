

#ifndef __VISION_CRITICAL_SECTION_IMP_H__
#define __VISION_CRITICAL_SECTION_IMP_H__


#include "ICriticalSectionImp.h"


class VCriticalSectionImp : public ICriticalSectionImp
{
public:
	VCriticalSectionImp();
	virtual ~VCriticalSectionImp();

private:
	VCriticalSectionImp(const VCriticalSectionImp &);
	VCriticalSectionImp &operator =(const VCriticalSectionImp &);

public:
	VBOOL lock(VDWORD timeout = -1);
	VBOOL unlock();

protected:
	CRITICAL_SECTION	mCS;
};


#endif	/*__VISION_CRITICAL_SECTION_IMP_H__*/
