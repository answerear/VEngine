/*
 *  IWindowEventListener.h
 *  VisionGame
 *
 *  Created by aaronwang on 12-5-18.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __IWINDOW_EVENT_LISTENER_H__
#define __IWINDOW_EVENT_LISTENER_H__


#include "VEngineConfig.h"


class IWindowEventListener
{
public:
	virtual void windowMoved(VINT32 x, VINT32 y) = 0;
	virtual void windowResized(VUINT32 w, VUINT32 h) = 0;
	virtual VBOOL windowClosing() = 0;
	virtual void windowClosed() = 0;
	virtual void windowFocusChange() = 0;
};


#endif	/*__IWINDOW_EVENT_LISTENER_H__*/

