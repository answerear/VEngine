

#ifndef __VISION_SYSTEM_FACTORY_H__
#define __VISION_SYSTEM_FACTORY_H__


#include "ISystemFactory.h"


class VSystemFactory : public ISystemFactory
{
public:
	VSystemFactory();

	virtual ITextCodecImp		*createTextCodec();
	virtual IConsoleImp			*createConsole();
	virtual IThreadImp			*createThread(IThreadObserver *observer);
	virtual IFileAccess			*createFileAccess();

	virtual ITimerImp			*createTimer();
	virtual ICriticalSectionImp	*createCriticalSection();
	virtual IMutexImp			*createMutex(VBOOL recursive = VTRUE, VCHAR *name = 0);
	virtual ISemaphoreImp		*createSemaphore(VLONG count = 1, VCHAR *name = 0);
	virtual IConditionImp		*createCondition();

	virtual VBOOL createDirectory(const VCHAR *dir);
	virtual VBOOL createFile(const VCHAR *filePath);
	virtual VBOOL deleteFile(const VCHAR *filePath);
	virtual VBOOL deleteDirectory(const VCHAR *dir);
	virtual VBOOL moveFile(const VCHAR *existFile, const VCHAR *newFile);
	virtual VBOOL copyFile(const VCHAR *existFile, const VCHAR *newFile);
};


#endif	/*__VISION_SYSTEM_FACTORY_H__*/

