/*
 *  IRenderEventListener.h
 *  VisionGame
 *
 *  Created by aaronwang on 12-5-18.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __IRENDER_EVENT_LISTENER_H__
#define __IRENDER_EVENT_LISTENER_H__


#include "VEngineConfig.h"


class IRenderEventListener
{
public:
	virtual void update(const Real &time, const Real &dt) = 0;	
	virtual void preRender(const Real &time, const Real &dt) = 0;
	virtual void postRender(const Real &time, const Real &dt) = 0;
};


#endif	/*__IRENDER_EVENT_LISTENER_H__*/

