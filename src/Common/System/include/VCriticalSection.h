

#ifndef __VISION_CRITICAL_SECTION_H__
#define __VISION_CRITICAL_SECTION_H__


#include "ISyncObject.h"


class ICriticalSectionImp;


class VSYSTEM_DLL VCriticalSection : public ISyncObject
{
public:
	VCriticalSection();
	virtual ~VCriticalSection();

private:
	VCriticalSection(const VCriticalSection &);
	VCriticalSection &operator =(const VCriticalSection &);

public:
	VBOOL lock(VDWORD timeout = -1);
	VBOOL unlock();

private:
	ICriticalSectionImp	*mHandle;
};


#endif	/*__VISION_CRITICAL_SECTION_H__*/
