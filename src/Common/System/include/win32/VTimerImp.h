

#ifndef __VISION_TIMER_IMP_H__
#define __VISION_TIMER_IMP_H__


#include "ITimerImp.h"
#include <map>


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
	static void CALLBACK onTimer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

private:
	typedef std::pair<UINT, VTimerImp*> VTimerPair;
	typedef std::map<UINT, VTimerImp*> VTimerMap;

	static VTimerMap mTimerMap;

	ITimerObserver	*mObserver;
	VUINT32			mTimerID;
};


#endif	/*__VISION_TIMER_IMP_H__*/

