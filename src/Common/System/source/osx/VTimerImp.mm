

#include "VTimerImp.h"
#include "ITimerObserver.h"


VUINT32 VTimerImp::msTimerIDRef = 0;


VTimerImp::VTimerImp()
: mTimerRef(NULL)
, mObserver(NULL)
{
}


VTimerImp::~VTimerImp()
{
	if (NULL != mTimerRef)
	{
		CFRunLoopRef pLoop = CFRunLoopGetCurrent();
		CFRunLoopRemoveTimer(pLoop, mTimerRef, kCFRunLoopCommonModes);
		CFRelease(mTimerRef);
		mTimerRef = NULL;
	}
}

VUINT32 VTimerImp::start(VUINT32 nInterval)
{
	CFRunLoopTimerContext context = {0, this, NULL, NULL, NULL};
	CFAbsoluteTime timeStamp = CFAbsoluteTimeGetCurrent();
	CFTimeInterval interval = (CFTimeInterval)nInterval / 1000.0;
	mTimerRef = CFRunLoopTimerCreate(0, timeStamp+interval, interval, 0, 0, VTimerImp::onTimer, &context);
	CFRunLoopRef pLoop = CFRunLoopGetMain();
	CFRunLoopAddTimer(pLoop, mTimerRef, kCFRunLoopDefaultMode);
	mTimerID = ++msTimerIDRef;
	return mTimerID;
}


void VTimerImp::stop()
{
	if (NULL != mTimerRef)
	{
		CFRunLoopRef pLoop = CFRunLoopGetCurrent();
		CFRunLoopRemoveTimer(pLoop, mTimerRef, kCFRunLoopCommonModes);
		CFRelease(mTimerRef);
		mTimerRef = NULL;
	}
	
	mTimerID = 0;
}


void VTimerImp::setObserver(ITimerObserver *pObserver)
{
	mObserver = pObserver;
}

VUINT32 VTimerImp::getTimerID() const
{
	return mTimerID;
}

void VTimerImp::onTimer(CFRunLoopTimerRef pTimerRef, void *pInfo)
{
	VTimerImp *pTimer = (VTimerImp *)pInfo;
	if (NULL != pTimer && NULL != pTimer->mObserver && pTimerRef == pTimer->mTimerRef && pTimer->mTimerID != 0)
	{
		VUINT32 dt = CFRunLoopTimerGetInterval(pTimerRef);
		VUINT32 time = 0;
		pTimer->mObserver->onTimer(pTimer->mTimerID, dt, time);
	}
}
