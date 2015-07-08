/**************************************************************************************************
 * Copyright (C) 2011. Vision Studio.
 * All rights reserved.
 *
 * @file    VEngine.cpp
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/7/28 0:12:56
 * @version v1.00
 **************************************************************************************************/


#include "VEngine.h"
#include "VSystem.h"
#include "VFramework.h"
#include "VGraphicsSystem.h"
#include "VInputSystem.h"


VEngine VEngine::mInstance;


VEngine::VEngine()
	: mSystem(VNULL)
	, mGfxSystem(VNULL)
	, mInputSystem(VNULL)
	, mFramework(VNULL)
	, mRunning(VFALSE)
{
	mSystem = new VSystem();
	mFramework = new VFramework();
	mGfxSystem = new VGraphicsSystem();
	mInputSystem = new VInputSystem();
	
	mGfxSystem->setRenderEventListener(this);
	mGfxSystem->setWindowEventListener(this);
}

VEngine::~VEngine()
{
	delete mInputSystem;
	mInputSystem = VNULL;

	delete mGfxSystem;
	mGfxSystem = VNULL;

	delete mFramework;
	mFramework = VNULL;

	delete mSystem;
	mSystem = VNULL;
}

VRESULT VEngine::startup(VUINT32 initStageID, const VString &title, VINT32 width, VINT32 height, const VString &cfgFile)
{
	// 支持中文路径
	setlocale(LC_CTYPE, "");

	VSYSTEM.startup();
	
	if (mGfxSystem)
	{
		mGfxSystem->startup(title, width, height, cfgFile);
	}

	if (mInputSystem)
	{
		VINT32 wndID = mGfxSystem->getRenderWindowID();
		mInputSystem->startup(wndID);
	}

	mGfxSystem->setupDebugPanel(mInputSystem);

	if (mFramework)
	{
		mFramework->startup(initStageID);
	}

	mRunning = VTRUE;

	return VERR_NONE;
}

VRESULT VEngine::shutdown()
{
	if (mFramework)
	{
		mFramework->shutdown();
	}

	if (mInputSystem)
	{
		mInputSystem->shutdown();
	}

	if (mGfxSystem)
	{
		mGfxSystem->shutdown();
	}

	VSYSTEM.shutdown();

	mRunning = VFALSE;

	return VERR_NONE;
}

VBOOL VEngine::run()
{
	VBOOL result = VFALSE;

	if (VNULL != mGfxSystem)
	{
		result = mGfxSystem->run();
	}
	
	return result || mRunning;
}

VGraphicsSystem *VEngine::getGfxSystem()
{
	return mGfxSystem;
}

VMouse *VEngine::getMouse()
{
	if (mInputSystem != VNULL)
	{
		return mInputSystem->getMouse();
	}
	return VNULL;
}

VKeyboard *VEngine::getKeyboard()
{
	if (mInputSystem != VNULL)
	{
		return mInputSystem->getKeyboard();
	}
	return VNULL;
}

// From IRenderEventListener
void VEngine::update(const Real &time, const Real &dt)
{
	if (mInputSystem != VNULL)
	{
		mInputSystem->update(time, dt);
	}

	if (mFramework != VNULL)
	{
		mFramework->update(time, dt);
	}
}

void VEngine::preRender(const Real &time, const Real &dt)
{
}

void VEngine::postRender(const Real &time, const Real &dt)
{
}

// From IWindowEventListener
void VEngine::windowMoved(VINT32 x, VINT32 y)
{
}

void VEngine::windowResized(VUINT32 w, VUINT32 h)
{
	if (mInputSystem != VNULL)
	{
		mInputSystem->adjustMouseClipArea(w, h);
	}
}

VBOOL VEngine::windowClosing()
{
	return VTRUE;
}

void VEngine::windowClosed()
{
	mRunning = VFALSE;
}

void VEngine::windowFocusChange()
{
}

