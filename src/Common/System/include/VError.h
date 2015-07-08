/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    VError.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/7/27 23:57:41
 * @version v1.00
 **************************************************************************************************/


#ifndef __VERROR_H__
#define __VERROR_H__



typedef enum tagVRESULT
{
	VERR_NONE = 0,
	VERR_NOT_INITIALIZE = -1,
	VERR_OUT_OF_MEMORY = -2,
	VERR_TIME_OUT = -3,
	VERR_INVALID_PARAM = -4,
	VERR_FAILED = -5
}VRESULT;


#define VSUCCEEDED(r)	(r >= 0)
#define VFAILED(r)		(r < 0)


#endif	/*__VERROR_H__*/
