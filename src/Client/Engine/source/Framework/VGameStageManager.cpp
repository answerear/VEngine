

#include "VGameStageManager.h"
#include "VGameStage.h"


template<> VGameStageManager *VSingleton<VGameStageManager>::mInstance = VNULL;


VGameStageManager::VGameStageManager()
	: mNextStageID(-1)
	, mCurrentStage(VNULL)
	, mGameStageMap()
	, mRunning(VFALSE)
{

}

VGameStageManager::~VGameStageManager()
{
	shutdown();
}


VRESULT VGameStageManager::startup(VUINT32 nStageID)
{
	mNextStageID = nStageID;

	// 创建游场景戏逻辑对象，并且放入map中管理
	mCurrentStage = createStage(nStageID);

	// 调用通知游戏逻辑对象做建立处理
	if (VNULL != mCurrentStage)
	{
		VBOOL result = mCurrentStage->enter();
		if (!result)
		{
			mNextStageID = -1;
			return VERR_FAILED;
		}
	}
	else
	{
		// 创建第一个游戏逻辑对象失败
		mNextStageID = -1;
		return VERR_INVALID_PARAM;
	}

	mRunning = VTRUE;

	return VERR_NONE;
}

VBOOL VGameStageManager::run(const Real &time, const Real &dt)
{
	if (!mRunning)
		return VFALSE;

	VBOOL result = VFALSE;

	// 游戏逻辑循环
	if (VNULL != mCurrentStage)
	{
		result = mCurrentStage->run(time, dt);
	}

	if (!result)
	{
		// 退出当前游戏逻辑
		result = mCurrentStage->exit();
		if (result)
		{
			// 需要删除游戏逻辑对象
			destroyStage(mCurrentStage);
		}

		// 创建下一个游戏逻辑对象并进入
		mCurrentStage = createStage(mNextStageID);
		if (VNULL != mCurrentStage)
		{
			result = mCurrentStage->enter();
		}
		else
		{
			result = VFALSE;
		}
	}

	return result;
}

void VGameStageManager::shutdown()
{
	while (!mGameStageMap.empty())
	{
		VStageMap::iterator itr = mGameStageMap.begin();
		destroyStage(itr->second.mGameStage);
		mGameStageMap.erase(itr);
	}

	mNextStageID = -1;
	mCurrentStage = VNULL;
	mRunning = VFALSE;
}

void VGameStageManager::registerStage(VUINT32 nStageID, VStageCreator creator)
{
	VGameStageInfo info = {creator, VNULL};
	std::pair<VUINT32, VGameStageInfo> p(nStageID, info);
	mGameStageMap.insert(p);
}

void VGameStageManager::unregisterStage(VUINT32 nStageID)
{
	VStageMap::iterator itr = mGameStageMap.find(nStageID);
	if (itr != mGameStageMap.end())
	{
		destroyStage(itr->second.mGameStage);
		mGameStageMap.erase(itr);
	}
}

VGameStage *VGameStageManager::createStage(VUINT32 nStageID)
{
	// 先检查是否存在该游戏场景逻辑对象
	VStageMap::iterator itr = mGameStageMap.find(nStageID);
	if (itr == mGameStageMap.end())
	{
		return VNULL;
	}

	if (VNULL == itr->second.mGameStage)
	{
		// 对象没创建，先创建
		itr->second.mGameStage = itr->second.mCreator();
		if (VNULL != itr->second.mGameStage)
		{
			// 通知游戏逻辑单元需要做建立初始化处理
			if (!itr->second.mGameStage->setup())
			{
				// 创建初始化失败，直接删除对象
				delete itr->second.mGameStage;
				itr->second.mGameStage = VNULL;
			}
		}
	}

	return itr->second.mGameStage;
}

void VGameStageManager::destroyStage(VGameStage *stage)
{
	if (VNULL == stage)
	{
		return;
	}

	VUINT32 nStageID = stage->getStageID();
	// 先检查是否存在该游戏场景逻辑对象
	VStageMap::iterator itr = mGameStageMap.find(nStageID);
	if (itr == mGameStageMap.end())
	{
		delete stage;
		return;
	}

	V_ASSERT(itr->second.mGameStage == stage);
	stage->destroy();
	delete itr->second.mGameStage;
	itr->second.mGameStage = VNULL;
}

void VGameStageManager::setNextStage(VUINT32 nStageID)
{
	mNextStageID = nStageID;
}

VGameStage *VGameStageManager::getCurrentStage()
{
	return mCurrentStage;
}
