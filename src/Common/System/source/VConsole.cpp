
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "VConsole.h"
#include "IConsoleImp.h"
#include "VSystem.h"
#include "VSystemFactory.h"

#include "VUtility.h"


template<> VConsole *VSingleton<VConsole>::mInstance = VNULL;


VConsole::VConsole()
	: mConsoleImp(VNULL)
{
	mConsoleImp = VSYSTEM_FACTORY.createConsole();
}

VConsole::~VConsole()
{
	delete mConsoleImp;
	mConsoleImp = VNULL;
}

VBOOL VConsole::startup(VCHAR *filename /* = VNULL */, VBOOL isFile /* = VFALSE */)
{
	VBOOL result = VFALSE;

	if (VNULL != mConsoleImp)
	{
		result = mConsoleImp->startup(filename, isFile);
	}
	
	return result;
}

void VConsole::shutdown()
{
	if (VNULL != mConsoleImp)
	{
		mConsoleImp->shutdown();
	}
}

VINT32 VConsole::debugOutput(VCHAR *fmt, ...)
{
	VINT32 length = 0;

	if (VNULL != mConsoleImp)
	{
		VCHAR buffer[MAX_BUFFER_SIZE];
		memset(buffer, 0, sizeof(buffer));

		va_list vl;
		va_start(vl, fmt);
		vsnprintf(buffer, MAX_BUFFER_SIZE, fmt, vl);
		va_end(vl);

		length = mConsoleImp->debugOutputString(buffer);
	}

	return length;
}

VINT32 VConsole::debugOutputError(VCHAR *fmt, ...)
{
	VINT32 length = 0;

	if (VNULL != mConsoleImp)
	{
		VCHAR msg[MAX_BUFFER_SIZE];
		snprintf(msg, MAX_BUFFER_SIZE, "[EROOR]");

		VCHAR buffer[MAX_BUFFER_SIZE];
		memset(buffer, 0, sizeof(buffer));

		va_list vl;
		va_start(vl, fmt);
		vsnprintf(buffer, MAX_BUFFER_SIZE, fmt, vl);
		va_end(vl);

		strcat(msg, buffer);

		length = mConsoleImp->debugOutputString(buffer);
	}

	return length;
}

VINT32 VConsole::debugOutputWarning(VCHAR *fmt, ...)
{
	VINT32 length = 0;

	if (VNULL != mConsoleImp)
	{
		VCHAR msg[MAX_BUFFER_SIZE];
		snprintf(msg, MAX_BUFFER_SIZE, "[WARNING]");

		VCHAR buffer[MAX_BUFFER_SIZE];
		memset(buffer, 0, sizeof(buffer));

		va_list vl;
		va_start(vl, fmt);
		vsnprintf(buffer, MAX_BUFFER_SIZE, fmt, vl);
		va_end(vl);

		strcat(msg, buffer);

		length = mConsoleImp->debugOutputString(buffer);
	}

	return length;
}

VINT32 VConsole::debugOutputString(VCHAR *str)
{
	VINT32 length = 0;

	if (VNULL != mConsoleImp)
	{
		length = mConsoleImp->debugOutputString(str);
	}

	return length;
}

void VConsole::flush()
{
	if (VNULL != mConsoleImp)
	{
		mConsoleImp->flush();
	}
}
