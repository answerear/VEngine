

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

	// �����γ���Ϸ�߼����󣬲��ҷ���map�й���
	mCurrentStage = createStage(nStageID);

	// ����֪ͨ��Ϸ�߼���������������
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
		// ������һ����Ϸ�߼�����ʧ��
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

	// ��Ϸ�߼�ѭ��
	if (VNULL != mCurrentStage)
	{
		result = mCurrentStage->run(time, dt);
	}

	if (!result)
	{
		// �˳���ǰ��Ϸ�߼�
		result = mCurrentStage->exit();
		if (result)
		{
			// ��Ҫɾ����Ϸ�߼�����
			destroyStage(mCurrentStage);
		}

		// ������һ����Ϸ�߼����󲢽���
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
	// �ȼ���Ƿ���ڸ���Ϸ�����߼�����
	VStageMap::iterator itr = mGameStageMap.find(nStageID);
	if (itr == mGameStageMap.end())
	{
		return VNULL;
	}

	if (VNULL == itr->second.mGameStage)
	{
		// ����û�������ȴ���
		itr->second.mGameStage = itr->second.mCreator();
		if (VNULL != itr->second.mGameStage)
		{
			// ֪ͨ��Ϸ�߼���Ԫ��Ҫ��������ʼ������
			if (!itr->second.mGameStage->setup())
			{
				// ������ʼ��ʧ�ܣ�ֱ��ɾ������
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
	// �ȼ���Ƿ���ڸ���Ϸ�����߼�����
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
