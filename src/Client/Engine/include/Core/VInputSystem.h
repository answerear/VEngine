

#ifndef __VISION_INPUT_SYSTEM_H__
#define __VISION_INPUT_SYSTEM_H__


#include "VEngineConfig.h"


class VENGINE_DLL VInputSystem
	: public OIS::KeyListener
	, public OIS::MouseListener
{
	friend class VGraphicsSystem;

public:
	VInputSystem();
	virtual ~VInputSystem();

private:
	VInputSystem(const VInputSystem &);
	VInputSystem &operator =(const VInputSystem &);

public:
	void startup(VINT32 wndID);
	void shutdown();
	void update(const Real &time, const Real &dt);

	void adjustMouseClipArea(VUINT32 w, VUINT32 h);

	VMouse		*getMouse()		{ return mMouse; }
	VKeyboard	*getKeyboard()	{ return mKeyboard; }

protected:
	// OIS::KeyListener
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	// OIS::MouseListener
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

protected:
	OIS::InputManager	*mInputManager;
	OIS::Mouse			*mMouse;
	OIS::Keyboard		*mKeyboard;
};


#endif	/*__VISION_INPUT_SYSTEM_H__*/
