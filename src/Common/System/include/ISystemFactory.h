

#ifndef __VISION_ISYSTEM_FACTORY_H__
#define __VISION_ISYSTEM_FACTORY_H__


#include "VSystemConfig.h"


class IThreadObserver;

class ITextCodecImp;
class IConsoleImp;
class IThreadImp;
class IFileAccess;

class ITimerImp;
class ICriticalSectionImp;
class IMutexImp;
class ISemaphoreImp;
class IConditionImp;
class IDateTimeImp;


class ISystemFactory
{
public:
	virtual ITextCodecImp		*createTextCodec() = 0;
	virtual IConsoleImp			*createConsole() = 0;
	virtual ITimerImp			*createTimer() = 0;
	virtual IThreadImp			*createThread(IThreadObserver *observer) = 0;
	virtual IFileAccess			*createFileAccess() = 0;
	virtual ICriticalSectionImp	*createCriticalSection() = 0;
	virtual IMutexImp			*createMutex(VBOOL recursive = VTRUE, VCHAR *name = 0) = 0;
	virtual ISemaphoreImp		*createSemaphore(VLONG count = 1, VCHAR *name = 0) = 0;
	virtual IConditionImp		*createCondition() = 0;

	virtual VBOOL createDirectory(const VCHAR *dir) = 0;
	virtual VBOOL deleteDirectory(const VCHAR *dir) = 0;
	virtual VBOOL createFile(const VCHAR *filePath) = 0;
	virtual VBOOL deleteFile(const VCHAR *filePath) = 0;
	virtual VBOOL moveFile(const VCHAR *existFile, const VCHAR *newFile) = 0;
	virtual VBOOL copyFile(const VCHAR *existFile, const VCHAR *newFile) = 0;
};


#endif	/*__VISION_ISYSTEM_FACTORY_H__*/
