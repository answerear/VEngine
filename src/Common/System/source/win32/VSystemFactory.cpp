

#include <windows.h>

#include "VSystemFactory.h"

#include "VTextCodecImp.h"
#include "VConsoleImp.h"
#include "VThreadImp.h"
#include "VFileAccess.h"

#include "VTimerImp.h"
#include "VCriticalSectionImp.h"
#include "VMutexImp.h"
#include "VSemaphoreImp.h"
// #include "VCondition.h"


ITextCodecImp *VSystemFactory::createTextCodec()
{
	return new VTextCodecImp();
}

IConsoleImp *VSystemFactory::createConsole()
{
	return new VConsoleImp();
}

IThreadImp *VSystemFactory::createThread(IThreadObserver *observer)
{
	return new VThreadImp(observer);
}

ITimerImp *VSystemFactory::createTimer()
{
	return new VTimerImp();
}

IFileAccess *VSystemFactory::createFileAccess()
{
	return new VFileAccess();
}


ICriticalSectionImp *VSystemFactory::createCriticalSection()
{
	return new VCriticalSectionImp();
}

IMutexImp *VSystemFactory::createMutex(VBOOL recursive /* = VTRUE */, VCHAR *name /* = 0 */)
{
	return new VMutexImp(recursive, name);
}

ISemaphoreImp *VSystemFactory::createSemaphore(VLONG count /* = 1 */, VCHAR *name /* = 0 */)
{
	return new VSemaphoreImp(count, name);
}

IConditionImp *VSystemFactory::createCondition()
{
	return VNULL;
}

VBOOL VSystemFactory::createDirectory(const VCHAR *dir)
{
	return (::CreateDirectory(dir, NULL) == TRUE);
}

VBOOL VSystemFactory::createFile(const VCHAR *filePath)
{
	VFileAccess file;

	if (!file.open(filePath, VFileAccess::modeCreate))
		return VFALSE;

	file.close();

	return VTRUE;
}

VBOOL VSystemFactory::deleteFile(const VCHAR *filePath)
{
	return (::DeleteFile(filePath) == TRUE);
}

VBOOL VSystemFactory::deleteDirectory(const VCHAR *dir)
{
	return (::RemoveDirectory(dir) == TRUE);
}

VBOOL VSystemFactory::moveFile(const VCHAR *existFile, const VCHAR *newFile)
{
	return (::MoveFile(existFile, newFile) == TRUE);
}

VBOOL VSystemFactory::copyFile(const VCHAR *existFile, const VCHAR *newFile)
{
	return (::CopyFile(existFile, newFile, FALSE) == TRUE);
}
