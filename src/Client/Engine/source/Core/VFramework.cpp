

#include "VFramework.h"
#include "VEventManager.h"
#include "VGameStageManager.h"


VFramework::VFramework()
	: mEventMgr(VNULL)
	, mGameStageMgr(VNULL)
{
	mEventMgr = new VEventManager();
	mGameStageMgr = new VGameStageManager();
}

VFramework::~VFramework()
{
	delete mGameStageMgr;
	mGameStageMgr = VNULL;

	delete mEventMgr;
	mEventMgr = VNULL;
}


void VFramework::startup(VUINT32 initStageID)
{
	VEVENT_MANAGER.startup();
	VGAME_STAGE_MANAGER.startup(initStageID);
}

void VFramework::shutdown()
{
	VGAME_STAGE_MANAGER.shutdown();
	VEVENT_MANAGER.shutdown();
}

VBOOL VFramework::update(const Real &time, const Real &dt)
{
	VBOOL result = VGAME_STAGE_MANAGER.run(time, dt);
	if (!result)
		return VFALSE;

	VEVENT_MANAGER.dispatchEvent();

	return VTRUE;
}

