

#include <windows.h>
#include "VThreadImp.h"
#include "IThreadObserver.h"

const VHANDLE V_INVALID_THREAD_ID = INVALID_HANDLE_VALUE;


VThreadImp::VThreadImp(IThreadObserver *observer)
	: mHandle(INVALID_HANDLE_VALUE)
	, mThreadID(0)
	, mObserver(observer)
	, mIsRunning(VFALSE)
{

}

VThreadImp::~VThreadImp()
{
	::TerminateThread(mHandle, -1);
	::CloseHandle(mHandle);
	mHandle = INVALID_HANDLE_VALUE;
	mThreadID = 0;
	mObserver = VNULL;
	mIsRunning = VFALSE;
}

VRESULT VThreadImp::create(VBOOL isSuspend /* = VFALSE */)
{
	DWORD dwFlags = 0;

	if (isSuspend)
	{
		dwFlags = CREATE_SUSPENDED;
	}

	VRESULT result = VERR_NONE;

	VHANDLE handle = ::CreateThread(NULL, 0, VThreadImp::threadProc, this, dwFlags, &mThreadID);
	if (handle != INVALID_HANDLE_VALUE && mThreadID != 0)
	{
		mHandle = handle;
		result = VERR_NONE;
	}
	else
	{
		result = VERR_FAILED;
	}

	return result;
}

VRESULT VThreadImp::suspend()
{
	VRESULT result = VERR_NONE;
	if (::SuspendThread(mHandle) == -1)
	{
		result = VERR_FAILED;
	}

	return result;
}

VRESULT VThreadImp::resume()
{
	VRESULT result = VERR_NONE;
	if (::ResumeThread(mHandle) == -1)
	{
		result = VERR_FAILED;
	}

	return result;
}

VRESULT VThreadImp::terminate(VDWORD exitCode)
{
	VRESULT result = VERR_NONE;
	if (::TerminateThread(mHandle, exitCode) == -1)
	{
		result = VERR_FAILED;
	}
	else
	{
		mIsRunning = VFALSE;
	}

	return result;
}

VRESULT VThreadImp::wait(VDWORD timeout /* = -1 */)
{
	DWORD ms = INFINITE;
	if (timeout != -1)
	{
		ms = timeout;
	}

	VRESULT result = VERR_TIME_OUT;

	DWORD dwResult = ::WaitForSingleObject(mHandle, ms);
	if (WAIT_OBJECT_0 == dwResult)
	{
		result = VERR_NONE;
	}
	else if (WAIT_TIMEOUT == dwResult)
	{
		result = VERR_TIME_OUT;
	}
	else
	{
		result = VERR_FAILED;
	}

	return result;
}

VBOOL VThreadImp::isRunning() const
{
	return mIsRunning;
}

VHANDLE VThreadImp::getThreadID() const
{
	return mHandle;
}

VDWORD VThreadImp::threadProc(void *param)
{
	VThreadImp *thread = (VThreadImp *)param;
	thread->mIsRunning = VTRUE;
	VINT32 result = thread->mObserver->run();
	thread->mIsRunning = VFALSE;
	return result;
}

