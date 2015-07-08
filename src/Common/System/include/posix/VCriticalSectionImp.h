

#ifndef __VISION_CRITICAL_SECTION_H__
#define __VISION_CRITICAL_SECTION_H__


#include "ICriticalSectionImp.h"
#include <pthread.h>


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
	pthread_mutex_t	mMutex;
};


#endif	/*__VISION_CRITICAL_SECTION_H__*/
