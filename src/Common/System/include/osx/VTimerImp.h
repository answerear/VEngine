

#ifndef __VISION_TIMER_IMP_H__
#define __VISION_TIMER_IMP_H__


#include "ITimerImp.h"
#include <CoreFoundation/CoreFoundation.h>


class VTimerImp : public ITimerImp
{
public:
	VTimerImp();
	virtual ~VTimerImp();

private:
	VTimerImp(const VTimerImp &);
	VTimerImp &operator =(const VTimerImp &);

protected:
	virtual VUINT32 start(VUINT32 interval);
	virtual void stop();
	virtual void setObserver(ITimerObserver *observer);
	virtual VUINT32 getTimerID() const;

private:
	static void onTimer(CFRunLoopTimerRef pTimerRef, void *pInfo);

	CFRunLoopTimerRef	mTimerRef;
	ITimerObserver		*mObserver;
	VUINT32				mTimerID;

	static VUINT32	msTimerIDRef;
};



#endif	/*__VISION_TIMER_IMP_H__*/

