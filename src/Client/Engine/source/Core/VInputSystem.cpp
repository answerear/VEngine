

#include "VInputSystem.h"
#include "VDateTime.h"
#include "VTrace.h"


#define MAX_KEY				256
#define MAX_MOUSE_BUTTON	3


VInputSystem::VInputSystem()
	: mInputManager(VNULL)
	, mMouse(VNULL)
	, mKeyboard(VNULL)
{
}

VInputSystem::~VInputSystem()
{
	if (mKeyboard)
	{
		mInputManager->destroyInputObject(mKeyboard);
		mKeyboard = VNULL;
	}

	if (mMouse)
	{
		mInputManager->destroyInputObject(mMouse);
		mMouse = VNULL;
	}

	if (mInputManager)
	{
		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = VNULL;
	}
}


void VInputSystem::startup(VINT32 wndID)
{
	OIS::ParamList pl;
	std::ostringstream windowHndStr;
	windowHndStr << wndID;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));

	mInputManager = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));

	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);
}

void VInputSystem::shutdown()
{
	if (mKeyboard)
	{
		mInputManager->destroyInputObject(mKeyboard);
		mKeyboard = VNULL;
	}

	if (mMouse)
	{
		mInputManager->destroyInputObject(mMouse);
		mMouse = VNULL;
	}

	if (mInputManager)
	{
		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = VNULL;
	}
}

void VInputSystem::update(const Real &time, const Real &dt)
{
	if (mMouse != VNULL)
	{
		mMouse->capture();
	}

	if (mKeyboard != VNULL)
	{
		mKeyboard->capture();
	}
}

void VInputSystem::adjustMouseClipArea(VUINT32 w, VUINT32 h)
{
	if (mMouse != VNULL)
	{
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = w;
		ms.height = h;
	}
}

bool VInputSystem::keyPressed(const OIS::KeyEvent &arg)
{
	return true;
}

bool VInputSystem::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}

bool VInputSystem::mouseMoved(const OIS::MouseEvent &arg)
{
	return true;
}

bool VInputSystem::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return true;
}

bool VInputSystem::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return true;
}
