

#ifndef __VISION_ITIMER_IMP_H__
#define __VISION_ITIMER_IMP_H__


#include "VSystemConfig.h"


extern const VUINT32 V_INVALID_TIMER_ID;


class ITimerObserver;


class ITimerImp
{
public:
	virtual VUINT32 start(VUINT32 interval) = 0;
	virtual void stop() = 0;
	virtual void setObserver(ITimerObserver *observer) = 0;
	virtual VUINT32 getTimerID() const = 0;
};


#endif	/*__VISION_ITIMER_IMP_H__*/

