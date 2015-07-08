

#ifndef __VISION_ICONSOLE_IMP_H__
#define __VISION_ICONSOLE_IMP_H__


#include "VSystemConfig.h"


#define MAX_BUFFER_SIZE		1024


class IConsoleImp
{
public:
	virtual VBOOL startup(VCHAR *filename = VNULL, VBOOL isFile = VFALSE) = 0;
	virtual void shutdown() = 0;

	virtual VINT32 debugOutputString(VCHAR *str) = 0;
	virtual void flush() = 0;
};


#endif	/*__VISION_ICONSOLE_IMP_H__*/

