

#ifndef __VISION_TIMER_H__
#define __VISION_TIMER_H__


#include "VSystemConfig.h"


class ITimerObserver;


class VSYSTEM_DLL VTimer
{
public:
	VTimer();
	virtual ~VTimer();

private:
	VTimer(const VTimer &);
	VTimer &operator =(const VTimer &);

public:
	VUINT32 start(VUINT32 interval);
	void stop();
	void setObserver(ITimerObserver *observer);
	VUINT32 getTimerID() const;

private:
	VHANDLE	mHandle;
};


#endif	/*__VISION_TIMER_H__*/
