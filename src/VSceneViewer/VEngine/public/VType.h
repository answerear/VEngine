/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    VType.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/7/27 22:53:03
 * @version v1.00
 **************************************************************************************************/


#ifndef __VTYPE_H__
#define __VTYPE_H__


#include "VConfig.h"

typedef signed char			VINT8;
typedef unsigned char		VUINT8;
typedef signed short		VINT16;
typedef unsigned short		VUINT16;
typedef signed int			VINT32;
typedef unsigned int		VUINT32;
typedef signed long			VLONG;
typedef unsigned long		VULONG;

typedef float				VFLOAT;
typedef double				VDOUBLE;

typedef char				VCHAR;
typedef wchar_t				VWCHAR;
typedef unsigned char		VBYTE;
typedef unsigned short		VWORD;
typedef unsigned long		VDWORD;

typedef bool				VBOOL;

#ifndef VTRUE
#define VTRUE				true
#define VFALSE				false
#endif

#if defined WIN32	/**< For Windows */

typedef signed __int64		VINT64;
typedef unsigned __int64	VUINT64;

#else				/**< For Linux */

typedef signed long long	VINT64;
typedef unsigned long long	VUINT64;

#endif


#include "VError.h"


#endif	/*__VTYPE_H__*/
