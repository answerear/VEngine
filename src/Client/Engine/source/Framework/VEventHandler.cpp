
#include "VEventHandler.h"
#include "VEventManager.h"


VEventHandler::VEventHandler(VBOOL isAutoRegister /* = VTRUE */)
	: mInstanceID(VEVENT_HANDLER_INVALID_ID)
{
	if (isAutoRegister)
	{
		registerHandler();
	}
}

VEventHandler::~VEventHandler()
{
	unregisterHandler();
}

VBOOL VEventHandler::sendEvent(VUINT32 eventID, VUINT32 param, VUINT64 receiver)
{
	return VEVENT_MANAGER.sendEvent(eventID, param, receiver);
}

VBOOL VEventHandler::postEvent(VUINT32 eventID, VUINT32 param, VUINT64 receiver)
{
	return VEVENT_MANAGER.postEvent(eventID, param, receiver);
}

VUINT64 VEventHandler::registerHandler()
{
	VEVENT_MANAGER.unregisterEventHandler(mInstanceID);
	mInstanceID = VEVENT_MANAGER.registerEventHandler(this);
	return mInstanceID;
}

void VEventHandler::unregisterHandler()
{
	VEVENT_MANAGER.unregisterEventHandler(mInstanceID);
}

VBOOL VEventHandler::processEvent(VUINT32 eventID, VUINT32 param)
{
	return eventProc(eventID, param);
}

VBOOL VEventHandler::eventProc(VUINT32 eventID, VUINT32 param)
{
	return VTRUE;
}

