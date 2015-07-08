

#ifndef __VISION_SYSTEM_CONFI_H__
#define __VISION_SYSTEM_CONFI_H__


#include "VConfig.h"
#include "VType.h"


#if defined VSYSTEM_EXPORTS
#define VSYSTEM_DLL		VEXPORT_DLL
#else
#define VSYSTEM_DLL		VIMPORT_DLL
#endif


#endif	/*__VISION_SYSTEM_CONFI_H__*/
