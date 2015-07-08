

#include "VSystem.h"
#include "ISystemFactory.h"
#include "VThread.h"
#include "IThreadImp.h"


VThread::VThread()
	: mHandle(VNULL)
{
	mHandle = VSYSTEM_FACTORY.createThread(this);
}

VThread::~VThread()
{
	delete mHandle;
	mHandle = VNULL;
}

VRESULT VThread::create(VBOOL isSuspend /* = VFALSE */)
{
	if (VNULL == mHandle)
	{
		return VERR_NOT_INITIALIZE;
	}

	return mHandle->create(isSuspend);
}

VRESULT VThread::suspend()
{
	if (VNULL == mHandle)
	{
		return VERR_NOT_INITIALIZE;
	}

	return mHandle->suspend();
}

VRESULT VThread::resume()
{
	if (VNULL == mHandle)
	{
		return VERR_NOT_INITIALIZE;
	}

	return mHandle->resume();
}

VRESULT VThread::terminate(VDWORD exitCode)
{
	if (VNULL == mHandle)
	{
		return VERR_NOT_INITIALIZE;
	}

	return mHandle->terminate(exitCode);
}

VRESULT VThread::wait(VDWORD timeout /* = -1 */)
{
	if (VNULL == mHandle)
	{
		return VERR_NOT_INITIALIZE;
	}

	return mHandle->wait(timeout);
}

VBOOL VThread::isRunning() const
{
	if (VNULL == mHandle)
	{
		return VFALSE;
	}

	return mHandle->isRunning();
}

VHANDLE VThread::getThreadID() const
{
	if (VNULL == mHandle)
	{
		return 0;
	}

	return mHandle->getThreadID();
}

