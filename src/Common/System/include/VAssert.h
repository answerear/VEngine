

#ifndef __VISION_ASSERT_H__
#define __VISION_ASSERT_H__


#include "VUtility.h"


#ifdef _V_DEBUG
#define V_ASSERT(x)		((x) ? 0 : VAssert(#x, __FILE__, __LINE__))
#else
#define V_ASSERT(x)		
#endif


#endif

