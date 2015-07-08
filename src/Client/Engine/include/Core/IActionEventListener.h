

#ifndef __IACTION_EVENT_LISTENER_H__
#define __IACTION_EVENT_LISTENER_H__


#include "VEngineConfig.h"


enum VActionTag
{
	VTAG_ANIM_START = 0,
	VTAG_ANIM_END = 1,
	VTAG_ANIM_FINISH = 2
};

class IActionEventListener
{
public:
	virtual VBOOL handleAnimationEvent(const VString &animation, VUINT32 tag) = 0;
};


#endif	/*__IACTION_EVENT_LISTENER_H__*/
