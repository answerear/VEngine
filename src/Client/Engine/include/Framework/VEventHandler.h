

#ifndef __VISION_EVENT_HANDLER_H__
#define __VISION_EVENT_HANDLER_H__


#include "VEngineConfig.h"
#include "VEventMacro.h"


#define VEVENT_HANDLER_INVALID_ID			0
#define VEVENT_HANDLER_BROADCAST_ID			0x00000000FFFFFFFF


class VENGINE_DLL VEventHandler
{
	friend class VEventManager;

public:
	VEventHandler(VBOOL isAutoRegister = VTRUE);
	virtual ~VEventHandler();

public:
	VBOOL sendEvent(VUINT32 eventID, VUINT32 param, VUINT64 receiver);
	VBOOL postEvent(VUINT32 eventID, VUINT32 param, VUINT64 receiver);

	VUINT64 getInstanceID() const
	{
		return mInstanceID;
	}

protected:
	VUINT64 registerHandler();
	void unregisterHandler();

	virtual VBOOL processEvent(VUINT32 eventID, VUINT32 param);
	VBOOL eventProc(VUINT32 eventID, VUINT32 param);

private:
	VUINT64 mInstanceID;
};


#endif	/*__VISION_EVENT_HANDLER_H__*/
