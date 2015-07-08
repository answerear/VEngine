

#ifndef __VISION_CONSOLE_H__
#define __VISION_CONSOLE_H__


#include "VSingleton.h"


class IConsoleImp;


class VSYSTEM_DLL VConsole : public VSingleton<VConsole>
{
	friend class VSystem;

	VConsole();
	VConsole(const VConsole &);
	VConsole &operator =(const VConsole &);
	~VConsole();

	VBOOL startup(VCHAR *filename = VNULL, VBOOL isFile = VFALSE);
	void shutdown();

public:
	VINT32 debugOutput(VCHAR *fmt, ...);
	VINT32 debugOutputWarning(VCHAR *fmt, ...);
	VINT32 debugOutputError(VCHAR *fmt, ...);
	VINT32 debugOutputString(VCHAR *str);
	
	void flush();

private:
	IConsoleImp	*mConsoleImp;
};


#define VCONSOLE	(VConsole::instance())


#endif	/*__VISION_CONSOLE_H__*/

