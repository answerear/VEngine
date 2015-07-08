

#ifndef __VISION_THREAD_IMP_H__
#define __VISION_THREAD_IMP_H__


#include "IThreadImp.h"


class IThreadObserver;


class VThreadImp : public IThreadImp
{
public:
	VThreadImp(IThreadObserver *observer);
	virtual ~VThreadImp();

private:
	VThreadImp(const VThreadImp &);
	VThreadImp &operator =(const VThreadImp &);

public:
	VRESULT create(VBOOL isSuspend = VFALSE);
	VRESULT suspend();
	VRESULT resume();
	VRESULT terminate(VDWORD exitCode);
	VRESULT wait(VDWORD timeout = -1);
	VBOOL isRunning() const;
	VHANDLE getThreadID() const;

protected:
	static VDWORD __stdcall threadProc(void *param);

protected:
	VHANDLE			mHandle;
	VDWORD			mThreadID;
	IThreadObserver	*mObserver;
	VBOOL			mIsRunning;
};


#endif	/*__VISION_THREAD_IMP_H__*/
