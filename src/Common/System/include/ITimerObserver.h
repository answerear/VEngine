

#ifndef __VISION_ITIMER_OBSERVER_H__
#define __VISION_ITIMER_OBSERVER_H__


#include "VSystemConfig.h"


class ITimerObserver
{
public:
	virtual void onTimer(VUINT32 timerID, VUINT32 dt, VUINT32 time) = 0;
};


#endif	/*__VISION_ITIMER_OBSERVER_H__*/
