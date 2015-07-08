

#ifndef __VISION_UTILITY_H__
#define __VISION_UTILITY_H__


#include "VSystemConfig.h"


#if defined PLATFORM_WIN32
#pragma warning(disable:4996)
#define snprintf	_snprintf
#endif


#define MK_DWORD(h, l)		(((h<<16)&0xFFFF0000)|(0xFFFF&l))
#define LO_WORD(x)			(x&0xFFFF)
#define HI_WORD(x)			((x>>16)&0xFFFF)
#define MK_WORD(h, l)		(((h<<8)&0xFF00)|(0xFF&l))
#define LO_BYTE(x)			(x&0xFF)
#define HI_BYTE(x)			((x&>>8)&0xFF)


VSYSTEM_DLL void VAssert(VCHAR *desc, VCHAR *filename, VLONG line);
VSYSTEM_DLL void VTrace(VCHAR *filename, VLONG line, VCHAR *fmt, ...);

VSYSTEM_DLL VDWORD VGetTickCount();
VSYSTEM_DLL void VSleep(VDWORD ms);
VSYSTEM_DLL VHANDLE VGetCurrentThreadID();
VSYSTEM_DLL VString VGetCurrentDirectory();

#endif
