

#include <windows.h>
#include "VTimerImp.h"
#include "ITimerObserver.h"


const VUINT32 V_INVALID_TIMER_ID = 0;


VTimerImp::VTimerMap VTimerImp::mTimerMap;


VTimerImp::VTimerImp()
	: mObserver(VNULL)
	, mTimerID(V_INVALID_TIMER_ID)
{

}

VTimerImp::~VTimerImp()
{
	if (V_INVALID_TIMER_ID != mTimerID)
	{
		mTimerMap.erase(mTimerID);
		::KillTimer(NULL, mTimerID);
		mTimerID = V_INVALID_TIMER_ID;
	}
}

VUINT32 VTimerImp::start(VUINT32 interval)
{
	mTimerID = ::SetTimer(NULL, 1000, interval, VTimerImp::onTimer);
	mTimerMap.insert(VTimerPair(mTimerID, this));
	return mTimerID;
}

void VTimerImp::stop()
{
	if (V_INVALID_TIMER_ID != mTimerID)
	{
		mTimerMap.erase(mTimerID);
		::KillTimer(NULL, mTimerID);
		mTimerID = V_INVALID_TIMER_ID;
	}
}

void VTimerImp::setObserver(ITimerObserver *observer)
{
	mObserver = observer;
}

VUINT32 VTimerImp::getTimerID() const
{
	return mTimerID;
}

void VTimerImp::onTimer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	VTimerMap::iterator itr = mTimerMap.find(idEvent);
	if (itr != mTimerMap.end())
	{
		itr->second->mObserver->onTimer(itr->first, dwTime, 0);
	}
}
