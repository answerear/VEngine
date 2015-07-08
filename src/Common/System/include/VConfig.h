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


#if defined PLATFORM_WIN32

#define VEXPORT_DLL		__declspec(dllexport)
#define VIMPORT_DLL		__declspec(dllimport)

#ifdef _DEBUG
#define _V_DEBUG
#endif

#else

#define VEXPORT_DLL
#define VIMPORT_DLL

#endif


#if defined PLATFORM_WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif


#endif	/*__VCONFIG_H__*/
