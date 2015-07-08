/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    VConfig.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/7/27 22:55:33
 * @version v1.00
 **************************************************************************************************/


#ifndef __VCONFIG_H__
#define __VCONFIG_H__


/**
 * 平台宏，控制编译平台
 */
#ifdef WIN32
#define PLATFORM_WIN32
#else
#define PLATFORM_LINUX
#endif


#ifdef PLATFORM_WIN32
	#if defined _AFXDLL || defined _USRDLL
		#define VENGINE_DLL		__declspec(dllexport)
	#else
		#define VENGINE_DLL		__declspec(dllimport)
	#endif
#else
	#define VENGINE_DLL	
#endif


#endif	/*__VCONFIG_H__*/
