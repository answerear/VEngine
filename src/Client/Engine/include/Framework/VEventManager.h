

#ifndef __VISION_EVENT_MANAGER_H__
#define __VISION_EVENT_MANAGER_H__


#include <vector>
#include <list>

#include "VEngineConfig.h"
#include "VSingleton.h"


class ISyncObject;
class VEventHandler;


#define VMAX_EVENT_QUEUE			2


class VENGINE_DLL VEventManager : public VSingleton<VEventManager>
{
	friend class VEventHandler;
	friend class VFramework;

	struct VEventItem
	{
		VUINT32 mEventID;
		VUINT32 mParam;
		VUINT64 mReceiver;
	};

private:
	~VEventManager();
	VEventManager();

	VEventManager(const VEventManager &);
	VEventManager &operator =(const VEventManager &);

public:
	VBOOL sendEvent(VUINT32 eventID, VUINT32 param, VUINT64 receiver);
	VBOOL postEvent(VUINT32 eventID, VUINT32 param, VUINT64 receiver);

	VBOOL getEventHandler(VUINT64 instanceID, VEventHandler *&handler);

private:

	VRESULT startup();
	void shutdown();

	VUINT64 registerEventHandler(VEventHandler *handler);
	void unregisterEventHandler(VUINT64 instanceID);

	VBOOL peekEvent(VEventItem &item, VBOOL removable = VTRUE);
	VBOOL dispatchEvent();

	void clearEventQueue();

private:
	typedef std::vector<VEventHandler*> VHandlerVector;
	typedef std::list<VEventItem> VEventList;

	VHandlerVector	mHandlerList;
	VEventList		mEventQueue[VMAX_EVENT_QUEUE];
	VINT32			mCurQueue;

	ISyncObject	*mSyncObject;
};


#define VEVENT_MANAGER	VEventManager::instance()


#endif	/*__VISION_EVENT_MANAGER_H__*/
