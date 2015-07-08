
#include "VType.h"
#include "VSystem.h"
#include "VConsole.h"
#include "VLogManager.h"
#include "VTextCodec.h"
#include "VSystemFactory.h"


template<> VSystem *VSingleton<VSystem>::mInstance = VNULL;


VSystem::VSystem()
	: mSystemFactory(VNULL)
	, mConsole(VNULL)
	, mLogMgr(VNULL)
{
	mSystemFactory = new VSystemFactory();
	mConsole = new VConsole();
	mLogMgr = new VLogManager();
	mTextCodec = new VTextCodec();
}

VSystem::~VSystem()
{
	delete mTextCodec;
	mTextCodec = VNULL;

	delete mLogMgr;
	mLogMgr = VNULL;

	delete mConsole;
	mConsole = VNULL;

	delete mSystemFactory;
	mSystemFactory = VNULL;
}

VRESULT VSystem::startup(VCHAR *filename /* = VNULL */, VBOOL isFile /* = VFALSE */)
{
	if (!mConsole->startup(filename, isFile))
	{
		return VERR_FAILED;
	}

	if (!mLogMgr->startup())
	{
		return VERR_FAILED;
	}

	return VERR_NONE;
}

VRESULT VSystem::shutdown()
{
	mLogMgr->shutdown();
	mConsole->shutdown();

	return VERR_NONE;
}

