
#include "VTimer.h"
#include "VGameStageManager.h"
#include "SceneGameStage.h"
#include "VTrace.h"
#include "Player.h"
#include "GameEvent.h"
#include "VAssert.h"
#include "VEngine.h"

#include "VFlyingCamera.h"
#include "VScene.h"
#include "VisionExpatParser.h"


V_IMPLEMENT_GAMESTAGE(SceneGameStage);

V_BEGIN_EVENT_MAP(SceneGameStage, VEventHandler)
V_END_EVENT_MAP()


SceneGameStage::SceneGameStage()
: mTimer(VNULL),
mVelocity(Vector3::ZERO),
mTopSpeed(100),
mFastMove(FALSE)
{

}

SceneGameStage::~SceneGameStage()
{
	delete mTimer;
}


VUINT32 SceneGameStage::getStageID() const
{
	return 2;
}

VBOOL SceneGameStage::setup()
{
	V_TRACE("SceneGameStage::setup()\n");

	VRuntimeClass *runtime = VRUNTIME_CLASS(Player);
	mPlayer = (Player*)runtime->createObject();

	V_ASSERT(mPlayer->isKindOf(VRUNTIME_CLASS(VObject)));
	postEvent(EV_CREATE, 0, mPlayer->getInstanceID());

	mScene = new VScene();

	return VTRUE;
}

VBOOL SceneGameStage::enter()
{
	V_TRACE("SceneGameStage::enter()\n");

	mCamera = new VFlyingCamera();
	mRunning = VTRUE;

	ExpatParser parserExpat(NULL);
	mScene->load(&parserExpat, "dali.Scene", "Vision", VTRUE);
	mScene->create();

	mCamera->setPosition(8815.33f, 800.0f, -2966.08f);

	return VTRUE;
}

VBOOL SceneGameStage::run(const Real &time,  const Real &dt)
{
	VKeyboard *keyboard = VENGINE.getKeyboard();
	VMouse *mouse = VENGINE.getMouse();

#if 0

	Vector3 t_CameraMove = Vector3::ZERO;

	if(keyboard->isKeyDown(OIS::KC_W))
		t_CameraMove += mCamera->getDirection();
	if(keyboard->isKeyDown(OIS::KC_S))
		t_CameraMove -= mCamera->getDirection();
	if(keyboard->isKeyDown(OIS::KC_A))
		t_CameraMove -= mCamera->getRight();
	if(keyboard->isKeyDown(OIS::KC_D))
		t_CameraMove += mCamera->getRight();

	if(keyboard->isKeyDown(OIS::KC_LSHIFT))
		mFastMove = TRUE;
	else
		mFastMove = FALSE;

	if(keyboard->isKeyDown(OIS::KC_ESCAPE))
		exit();



	Ogre::Real topSpeed = mFastMove ? mTopSpeed * 20 : mTopSpeed;
	if (t_CameraMove.squaredLength() != 0)
	{
		t_CameraMove.normalise();
		mVelocity += t_CameraMove * topSpeed * dt * 10;
	}
	else mVelocity -= mVelocity * dt * 10;

	Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

	if (mVelocity.squaredLength() > topSpeed * topSpeed)
	{
		mVelocity.normalise();
		mVelocity *= topSpeed;
	}
		else if (mVelocity.squaredLength() < tooSmall * tooSmall)
			mVelocity = Ogre::Vector3::ZERO;
		if (mVelocity != Ogre::Vector3::ZERO) mCamera->getNativeCamera()->move(mVelocity *dt);


	mCamera->getNativeCamera()->yaw(Ogre::Degree(-mouse->getMouseState().X.rel * 0.15f));
	mCamera->getNativeCamera()->pitch(Ogre::Degree(-mouse->getMouseState().Y.rel * 0.15f));
	
#else

	const float moveSpeed = 500.0f;

	if (keyboard->isKeyDown(OIS::KC_UP))
	{
		mCamera->walk(-moveSpeed * dt);
	}
	else if (keyboard->isKeyDown(OIS::KC_DOWN))
	{
		mCamera->walk(moveSpeed * dt);
	}

	if (keyboard->isKeyDown(OIS::KC_LEFT))
	{
		mCamera->strafe(-moveSpeed * dt);
	}
	else if (keyboard->isKeyDown(OIS::KC_RIGHT))
	{
		mCamera->strafe(moveSpeed * dt);
	}

	const float rotationSpeed = 0.5f;

	if (keyboard->isKeyDown(OIS::KC_A))
	{
		mCamera->yaw(rotationSpeed * dt);
	}
	else if (keyboard->isKeyDown(OIS::KC_D))
	{
		mCamera->yaw(-rotationSpeed * dt);
	}

	if (keyboard->isKeyDown(OIS::KC_W))
	{
		mCamera->pitch(-rotationSpeed * dt);
	}
	else if (keyboard->isKeyDown(OIS::KC_S))
	{
		mCamera->pitch(rotationSpeed * dt);
	}

#endif

	return mRunning;
}

VBOOL SceneGameStage::exit()
{
	VGAME_STAGE_MANAGER.setNextStage(-1);

	sendEvent(EV_DESTROY, 0, mPlayer->getInstanceID());
	delete mPlayer;

	mScene->destroy();

	delete mCamera;

	V_TRACE("SceneGameStage::exit()\n");
	return VTRUE;
}

void SceneGameStage::destroy()
{
	delete mScene;
	V_TRACE("SceneGameStage::destroy()\n");
}

void SceneGameStage::onTimer(VUINT32 timerID, VUINT32 dt, VUINT32 time)
{
	if (VNULL != mTimer && mTimer->getTimerID() == timerID)
	{
		V_TRACE("SceneGameStage::onTimer timer id = %u\n", timerID);
		mRunning = VFALSE;
		mTimer->stop();
	}
}
