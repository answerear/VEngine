/**************************************************************************************************
 * Copyright (C) 2011. Vision Studio.
 * All rights reserved.
 *
 * @file    VGameStageManager.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/8/28 21:14:44
 * @version v1.00
 **************************************************************************************************/


#ifndef __VISION_GAME_STAGE_MANAGER_H__
#define __VISION_GAME_STAGE_MANAGER_H__


#include <map>
#include "VEngineConfig.h"
#include "VSingleton.h"


class VGameStage;


typedef VGameStage* (STDCALL* VStageCreator)();


/**
 * @class VGameStageManager
 * @brief 游戏逻辑场景管理类.
 */
class VENGINE_DLL VGameStageManager : public VSingleton<VGameStageManager>
{
	friend class VFramework;

private:
	/**
	 * @brief Constructor for VGameStageManager.
	 */
	VGameStageManager();

	/**
	 * @brief Destructor for VGameStageManager.
	 */
	~VGameStageManager();

	VGameStageManager(const VGameStageManager &);
	VGameStageManager &operator =(const VGameStageManager &);

public:
	VRESULT startup(VUINT32 nStageID);
	VBOOL run(const Real &time, const Real &dt);
	void shutdown();

	void registerStage(VUINT32 nStageID, VStageCreator creator);
	void unregisterStage(VUINT32 nStageID);

	VGameStage *createStage(VUINT32 nStageID);
	void destroyStage(VGameStage *stage);

	void setNextStage(VUINT32 nStageID);
	VGameStage *getCurrentStage();

private:
	struct VGameStageInfo
	{
		VStageCreator	mCreator;
		VGameStage		*mGameStage;
	};

	typedef std::map<VUINT32, VGameStageInfo> VStageMap;

	VUINT32		mNextStageID;
	VGameStage	*mCurrentStage;
	VStageMap	mGameStageMap;
	VBOOL		mRunning;
};


#define VGAME_STAGE_MANAGER		VGameStageManager::instance()


#endif	/*__VISION_GAME_STAGE_MANAGER_H__*/
