
#ifndef __VISION_THREAD_H__
#define __VISION_THREAD_H__


#include "IThreadObserver.h"


class IThreadImp;


class VSYSTEM_DLL VThread : public IThreadObserver
{
public:
	VThread();
	virtual ~VThread();

public:
	VRESULT create(VBOOL isSuspend = VFALSE);
	VRESULT suspend();
	VRESULT resume();
	VRESULT terminate(VDWORD exitCode);
	VRESULT wait(VDWORD timeout = -1);
	VBOOL isRunning() const;
	VHANDLE getThreadID() const;

protected:
	virtual VINT32 run() = 0;

private:
	IThreadImp	*mHandle;
};


#endif	/*__VISION_THREAD_H__*/

