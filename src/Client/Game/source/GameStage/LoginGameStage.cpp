
#include "VTimer.h"
#include "VGameStageManager.h"
#include "LoginGameStage.h"
#include "VTrace.h"


V_IMPLEMENT_GAMESTAGE(LoginGameStage);

V_BEGIN_EVENT_MAP(LoginGameStage, VEventHandler)
V_END_EVENT_MAP()


LoginGameStage::LoginGameStage()
: mTimer(VNULL)
{

}

LoginGameStage::~LoginGameStage()
{
	delete mTimer;
}


VUINT32 LoginGameStage::getStageID() const
{
	return 1;
}

VBOOL LoginGameStage::setup()
{
 	V_TRACE("LoginGameStage::setup()\n");
	mTimer = new VTimer();
	mTimer->setObserver(this);
	return VTRUE;
}

VBOOL LoginGameStage::enter()
{
 	V_TRACE("LoginGameStage::enter()\n");
	mRunning = VTRUE;
	mTimer->start(1000);
	return VTRUE;
}

VBOOL LoginGameStage::run(const Real &time,  const Real &dt)
{
	return mRunning;
}

VBOOL LoginGameStage::exit()
{
	VGAME_STAGE_MANAGER.setNextStage(2);
	V_TRACE("LoginGameStage::exit()\n");
	return VFALSE;
}

void LoginGameStage::destroy()
{
	V_TRACE("LoginGameStage::destroy()\n");
}

void LoginGameStage::onTimer(VUINT32 timerID, VUINT32 dt, VUINT32 time)
{
	if (VNULL != mTimer && mTimer->getTimerID() == timerID)
	{
		V_TRACE("LoginGameStage::onTimer timer id = %u\n", timerID);
		mRunning = VFALSE;
		mTimer->stop();
	}
}
