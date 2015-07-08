

#include "VEventManager.h"
#include "VEventHandler.h"
#include "VMutex.h"
#include "VSingleLock.h"


template<> VEventManager *VSingleton<VEventManager>::mInstance = VNULL;


VEventManager::VEventManager()
	: mCurQueue(0)
	, mSyncObject(VNULL)
{

}

VEventManager::~VEventManager()
{
	mHandlerList.clear();
	clearEventQueue();

	delete mSyncObject;
}

VRESULT VEventManager::startup()
{
	mHandlerList.clear();
	clearEventQueue();

	mCurQueue = 0;
	mHandlerList.resize(128, VNULL);

	mSyncObject = new VMutex();

	return VERR_NONE;
}

void VEventManager::shutdown()
{
	mHandlerList.clear();
	clearEventQueue();

	delete mSyncObject;
	mSyncObject = VNULL;
}

VUINT64 VEventManager::registerEventHandler(VEventHandler *handler)
{
	VSingleLock locker(mSyncObject, VTRUE);

	// 先查找原来数组中是否有空位，如果有空位直接存入数组空位，没有则需要扩展数组大小
	VBOOL found = VFALSE;
	VINT32 index = 0;
	VHandlerVector::iterator itr = mHandlerList.begin();
	while (itr != mHandlerList.end())
	{
		if (*itr == VNULL)
		{
			mHandlerList[index] = handler;
			found = VTRUE;
			break;
		}

		++itr;
		++index;
	}

	if (!found)
	{
		mHandlerList.push_back(handler);
		index = mHandlerList.size() - 1;
	}

#if defined PLATFORM_WIN32
	VUINT32 address = (VUINT32)handler;
	VUINT64 instance = (((VUINT64)index << 32) | (VUINT64)address);
#elif defined PLATFORM_MAC
	VUINT64 instance = (VUINT64)handler;
#endif
	return instance;
}

void VEventManager::unregisterEventHandler(VUINT64 instanceID)
{
	if (0 == instanceID)
	{
		return;
	}

	VSingleLock locker(mSyncObject, VTRUE);

	VINT32 index = ((instanceID >> 32) & 0xFFFFFFFF);
	VINT32 i = 0;
	VHandlerVector::iterator itr = mHandlerList.begin();
	while (itr != mHandlerList.end())
	{
		if (index == i)
		{
			mHandlerList[index] = VNULL;
			break;
		}

		++itr;
		++index;
	}
}

VBOOL VEventManager::sendEvent(VUINT32 eventID, VUINT32 param, VUINT64 receiver)
{
	if (VEVENT_HANDLER_INVALID_ID == receiver)
	{
		return VFALSE;
	}

	VEventHandler *handler = VNULL;

	VSingleLock locker(mSyncObject, VTRUE);

	// 判断是否有注册对象
	VINT32 index = ((receiver >> 32) & 0xFFFFFFFF);
	if (mHandlerList[index] == VNULL)
	{
		return VFALSE;
	}

	VBOOL result = getEventHandler(receiver, handler);
	if (!result)
	{
		return VFALSE;
	}

	return handler->processEvent(eventID, param);
}

VBOOL VEventManager::postEvent(VUINT32 eventID, VUINT32 param, VUINT64 receiver)
{
	if (VEVENT_HANDLER_INVALID_ID == receiver)
	{
		return VFALSE;
	}

	VEventHandler *handler = VNULL;
	VSingleLock locker(mSyncObject, VTRUE);

	if (0 != receiver)
	{
		// 非广播事件，判断是否有注册对象
		VINT32 index = ((receiver >> 32) & 0xFFFFFFFF);
		if (mHandlerList[index] == VNULL)
		{
			return VFALSE;
		}
	}

	VEventItem item = {eventID, param, receiver};
	mEventQueue[mCurQueue].push_back(item);

	return VTRUE;
}

VBOOL VEventManager::getEventHandler(VUINT64 instanceID, VEventHandler *&handler)
{
	VINT32 index = ((instanceID >> 32) & 0xFFFFFFFF);
	if (mHandlerList[index] == VNULL)
	{
		// 对象已经被注销，不能接受事件
		return VFALSE;
	}

	VUINT32 address = (VUINT32)(instanceID & 0xFFFFFFFF);
	handler = (VEventHandler *)address;
	return VTRUE;
}

VBOOL VEventManager::peekEvent(VEventItem &item, VBOOL removable /* = VTRUE */)
{
	VBOOL result = VFALSE;

	VSingleLock locker(mSyncObject, VTRUE);
	if (!mEventQueue[mCurQueue].empty())
	{
		VEventList::iterator itr = mEventQueue[mCurQueue].begin();
		item = *itr;
		result = VTRUE;
	}
	
	return result;
}

VBOOL VEventManager::dispatchEvent()
{
	VSingleLock locker(mSyncObject);

	locker.lock();
	VINT32 index = mCurQueue;
	mCurQueue = (++mCurQueue) % VMAX_EVENT_QUEUE;
	locker.unlock();

	if (mEventQueue[index].empty())
	{
		return VFALSE;
	}

	VEventList::iterator itr = mEventQueue[index].begin();
	while (!mEventQueue[index].empty())
	{
		VEventItem &item = mEventQueue[index].front();

		if (VEVENT_HANDLER_BROADCAST_ID == item.mReceiver)
		{
			// 广播事件通知
			VHandlerVector::iterator itr = mHandlerList.begin();
			while (itr != mHandlerList.end())
			{
				if (VNULL != mHandlerList[index])
				{
					VEventHandler *handler = VNULL;
					if (getEventHandler(item.mReceiver, handler))
					{
						handler->processEvent(item.mEventID, item.mParam);
					}
				}

				++itr;
			}
		}
		else
		{
			// 单个对象事件通知
			VEventHandler *handler = VNULL;
			if (getEventHandler(item.mReceiver, handler))
			{
				handler->processEvent(item.mEventID, item.mParam);
			}
		}

		mEventQueue[index].pop_front();
	}

	return VTRUE;
}

void VEventManager::clearEventQueue()
{
	VINT32 i = 0;
	for (i = 0; i < VMAX_EVENT_QUEUE; ++i)
	{
		mEventQueue[i].clear();
	}
}

