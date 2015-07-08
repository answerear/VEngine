

#ifndef __VISION_ITHREAD_OBSERVER_H__
#define __VISION_ITHREAD_OBSERVER_H__


#include "VSystemConfig.h"


class IThreadObserver
{
public:
	virtual VINT32 run() = 0;
};


#endif	/*__VISION_ITHREAD_OBSERVER_H__*/

