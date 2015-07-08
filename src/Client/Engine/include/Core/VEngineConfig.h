/**************************************************************************************************
 * Copyright (C) 2011. Vision Studio.
 * All rights reserved.
 *
 * @file    VEngineConfig.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011-08-24 12:52:05
 * @version v1.00
 **************************************************************************************************/


#ifndef __VISION_ENGINE_CONFIG_H__
#define __VISION_ENGINE_CONFIG_H__


#include "VConfig.h"
#include "VType.h"
#include <Ogre/Ogre.h>
#include <OIS/OIS.h>
#include "VEngineType.h"


#if defined VENGINE_EXPORTS
#define VENGINE_DLL		VEXPORT_DLL
#else
#define VENGINE_DLL		VIMPORT_DLL
#endif


#endif	/*__VISION_ENGINE_CONFIG_H__*/
