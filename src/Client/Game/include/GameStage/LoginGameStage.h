

#ifndef __LOGIN_GAME_STAGE_H__
#define __LOGIN_GAME_STAGE_H__


#include "VGameStage.h"
#include "VEventHandler.h"
#include "ITimerObserver.h"


class VTimer;


class LoginGameStage 
	: public VGameStage
	, public VEventHandler
	, public ITimerObserver
{
 	V_DECLARE_EVENT_MAP();
	V_DECLARE_GAMESTAGE();

public:
	LoginGameStage();
	virtual ~LoginGameStage();

	VUINT32 getStageID() const;

	VBOOL setup();
	VBOOL enter();
	VBOOL run(const Real &time,  const Real &dt);
	VBOOL exit();
	void destroy();

	void onTimer(VUINT32 timerID, VUINT32 dt, VUINT32 time);

protected:
	VTimer	*mTimer;
	VBOOL	mRunning;
};

#endif	/*__LOGIN_GAME_STAGE_H__*/
