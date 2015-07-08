

#ifndef __VISION_FLAGS_H__
#define __VISION_FLAGS_H__


#include "VEngineConfig.h"


namespace Vision
{
	enum ObjectVisibilityFlags
	{
		OVF_POSTFILTER = 1 << 0,
		OVF_INDICATORS = 1 << 1,
		OVF_GUI_ELEMENTS = 1 << 2,
		OVF_DEFAULT = ~0 << 3
	};
}


#endif	/**__VISION_FLAGS_H__*/
