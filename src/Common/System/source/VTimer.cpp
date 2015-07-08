

#include "VSystem.h"
#include "ISystemFactory.h"
#include "VTimer.h"
#include "ITimerImp.h"


#define TIMER_HANDLE	((ITimerImp*)mHandle)


VTimer::VTimer()
	: mHandle(VNULL)
{
	mHandle = VSYSTEM_FACTORY.createTimer();
}

VTimer::~VTimer()
{
	delete TIMER_HANDLE;
	mHandle = VNULL;
}

VUINT32 VTimer::start(VUINT32 interval)
{
	return TIMER_HANDLE->start(interval);
}

void VTimer::stop()
{
	return TIMER_HANDLE->stop();
}

void VTimer::setObserver(ITimerObserver *observer)
{
	TIMER_HANDLE->setObserver(observer);
}

VUINT32 VTimer::getTimerID() const
{
	return TIMER_HANDLE->getTimerID();
}

