

#include "VCamera.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


VCamera::VCamera(VBOOL bAutoCreated /* = VFALSE */, const VString &name /* = "" */)
{
	VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();

	if (bAutoCreated)
	{
		mNativeCamera = pGfxSystem->mSceneMgr->createCamera(name);
		mAutoCreated = bAutoCreated;
	}
	else
	{
		mNativeCamera = pGfxSystem->mCamera;
		mAutoCreated = bAutoCreated;
	}
}

VCamera::~VCamera()
{
	if (mAutoCreated)
	{
		VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();
		pGfxSystem->mSceneMgr->destroyCamera(mNativeCamera);
	}
}


