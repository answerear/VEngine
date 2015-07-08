

#ifndef __VISION_TRACE_H__
#define __VISION_TRACE_H__


#include "VUtility.h"


#ifdef _V_DEBUG
#define V_TRACE(fmt, ...)		VTrace(__FILE__, __LINE__, fmt, __VA_ARGS__)
#else
#define V_TRACE(fmt, ...)		
#endif


#endif	/*__VISION_TRACE_H__*/
