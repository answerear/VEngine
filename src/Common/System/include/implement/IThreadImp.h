

#ifndef __VISION_ITHREAD_IMP_H__
#define __VISION_ITHREAD_IMP_H__


#include "VSystemConfig.h"


extern const VHANDLE V_INVALID_THREAD_ID;


class IThreadImp
{
public:
	virtual VRESULT create(VBOOL isSuspend = VFALSE) = 0;
	virtual VRESULT suspend() = 0;
	virtual VRESULT resume() = 0;
	virtual VRESULT terminate(VDWORD exitCode) = 0;
	virtual VRESULT wait(VDWORD timeout = -1) = 0;
	virtual VBOOL isRunning() const = 0;
	virtual VHANDLE getThreadID() const = 0;
};


#endif	/*__VISION_ITHREAD_IMP_H__*/
