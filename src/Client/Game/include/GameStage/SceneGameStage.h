

#ifndef __SCENE_GAME_STAGE_H__
#define __SCENE_GAME_STAGE_H__


#include "VGameStage.h"
#include "VEventHandler.h"
#include "ITimerObserver.h"
#include "VInputSystem.h"


class Player;
class VScene;
class VFlyingCamera;


class SceneGameStage 
	: public VGameStage
	, public VEventHandler
	, public ITimerObserver
{
	V_DECLARE_EVENT_MAP();
	V_DECLARE_GAMESTAGE();

public:
	SceneGameStage();
	virtual ~SceneGameStage();

public:
	VUINT32 getStageID() const;

	VBOOL setup();
	VBOOL enter();
	VBOOL run(const Real &time,  const Real &dt);
	VBOOL exit();
	void destroy();

protected:
	void onTimer(VUINT32 timerID, VUINT32 dt, VUINT32 time);

protected:
	VScene	*mScene;
	Player	*mPlayer;
	VTimer	*mTimer;
	VFlyingCamera	*mCamera;
	VBOOL	mRunning;

	VFLOAT  mTopSpeed;
	VBOOL	mFastMove;
	Vector3 mVelocity;


};


#endif	/*__SCENE_GAME_STAGE_H__*/
