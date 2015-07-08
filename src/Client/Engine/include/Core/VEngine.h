/**************************************************************************************************
 * Copyright (C) 2011. Vision Studio.
 * All rights reserved.
 *
 * @file    VEngine.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/7/27 23:52:05
 * @version v1.00
 **************************************************************************************************/


#ifndef __VISION_ENGINE_H__
#define __VISION_ENGINE_H__


#include "VEngineConfig.h"
#include "IRenderEventListener.h"
#include "IWindowEventListener.h"


class VSystem;
class VGraphicsSystem;
class VInputSystem;
class VFramework;


class VENGINE_DLL VEngine 
	: public IRenderEventListener
	, public IWindowEventListener
{
private:
	VEngine();

public:
	~VEngine();

private:
	VEngine(const VEngine &other);
	VEngine &operator =(const VEngine &other);

public:
	static VEngine &instance()
	{
		return mInstance;
	}

	VRESULT startup(VUINT32 initStageID, const VString &title, VINT32 width, VINT32 height, const VString &cfgFile);
	VRESULT shutdown();
	VBOOL run();

	VGraphicsSystem *getGfxSystem();

	VMouse		*getMouse();
	VKeyboard	*getKeyboard();

protected:
	// From IRenderEventListener
	virtual void update(const Real &time, const Real &dt);
	virtual void preRender(const Real &time, const Real &dt);
	virtual void postRender(const Real &time, const Real &dt);
	
	// From IWindowEventListener
	virtual void windowMoved(VINT32 x, VINT32 y);
	virtual void windowResized(VUINT32 w, VUINT32 h);
	virtual VBOOL windowClosing();
	virtual void windowClosed();
	virtual void windowFocusChange();
	
protected:
	static VEngine mInstance;

	VSystem			*mSystem;		/**< 系统层对象 */
	VGraphicsSystem	*mGfxSystem;	/**< 图形系统对象 */
	VInputSystem	*mInputSystem;	/**< 输入系统对象 */
	VFramework		*mFramework;	/**< 框架系统对象 */

	VBOOL	mRunning;
};


#define VENGINE		VEngine::instance()


#endif	/*__VISION_ENGINE_H__*/

