/**************************************************************************************************
 * Copyright (C) 2011. Vision Studio.
 * All rights reserved.
 *
 * @file    VGameStage.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/8/28 21:13:17
 * @version v1.00
 **************************************************************************************************/


#ifndef __VISION_GAME_STAGE_H__
#define __VISION_GAME_STAGE_H__


#include "VEngineConfig.h"


#define V_DECLARE_GAMESTAGE() \
	public: \
		static VGameStage* STDCALL createStage();

#define V_IMPLEMENT_GAMESTAGE(class_name) \
	VGameStage* class_name::createStage() \
	{ return new class_name(); }


class VENGINE_DLL VGameStage
{
public:
	virtual VUINT32 getStageID() const = 0;

	virtual VBOOL setup() = 0;
	virtual VBOOL enter() = 0;
	virtual VBOOL run(const Real &time, const Real &dt) = 0;
	virtual VBOOL exit() = 0;
	virtual void destroy() = 0;
};


#endif	/*__VISION_GAME_STAGE_H__*/
