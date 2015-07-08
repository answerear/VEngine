

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#if defined PLATFORM_WIN32
#include <windows.h>

#pragma warning(disable:4996)
#elif defined PLATFORM_LINUX
#elif defined PLATFORM_MAC
#endif

#include "VUtility.h"
#include "VConsole.h"


void VAssert(VCHAR *desc, VCHAR *filename, VLONG line)
{
#if defined PLATFORM_WIN32
#if _DEBUG
	MSG msg;
	BOOL bQuit = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
	BOOL bResult = _CrtDbgReport(_CRT_ASSERT, filename, line, desc, NULL);
	if (bQuit)
		PostQuitMessage((int)msg.wParam);
#endif
#elif defined PLATFORM_LINUX
#elif defined PLATFORM_MAC
#endif
}

void VTrace(VCHAR *filename, VLONG line, VCHAR *fmt, ...)
{	
	VCHAR msg[1024];
	snprintf(msg, 1024, "[%s] [%l]", filename, line);

	VCHAR buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	va_list vl;
	va_start(vl, fmt);
	vsnprintf(buffer, 1024, fmt, vl);
	va_end(vl);

	strcat(msg, buffer);
	VCONSOLE.debugOutputString(msg);
}

VDWORD VGetTickCount()
{
	VDWORD tick = 0;

#if defined PLATFORM_WIN32
	tick = GetTickCount();
#elif defined PLATFORM_LINUX
#elif defined PLATFORM_MAC
#endif

	return tick;
}

void VSleep(VDWORD ms)
{
#if defined PLATFORM_WIN32
	Sleep(ms);
#elif defined PLATFORM_LINUX
#elif defined PLATFORM_MAC
#endif
}

VHANDLE VGetCurrentThreadID()
{
	VHANDLE threadID = 0;

#if defined PLATFORM_WIN32
	threadID = (VHANDLE)GetCurrentThread();
#elif defined PLATFORM_LINUX
#elif defined PLATFORM_MAC
#endif

	return threadID;
}

VString VGetCurrentDirectory()
{
#if defined PLATFORM_WIN32
	VCHAR szPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szPath);
	VString dir = VString(szPath) + "/";
#elif defined PLATFORM_LINUX
	VString dir;
#elif defined PLATFORM_MAC
	VString dir;
#endif

	return dir;
}

