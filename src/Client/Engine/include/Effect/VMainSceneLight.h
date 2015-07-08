

#ifndef __VISION_MAIN_SCENE_LIGHT_H__
#define __VISION_MAIN_SCENE_LIGHT_H__


#include "VEngineConfig.h"


/// 场景灯光变化的几个阶段
enum VLightModifyPhase
{
	VLMP_IDLE,       /// 空闲，这时候可以使用那些能控制场景灯光的特效
	VLMP_UPDATE,     /// 更新状态，只有在这个状态下，灯光信息才会进行改变
	VLMP_FADEIN,     /// 场景灯光正出于fade in状态
	VLMP_LAST,       /// 场景灯光正处于稳定状态
	VLMP_FADEOUT,    /// 场景灯光正出于fade out状态
};

class VMainSceneLight
{
public:
	VMainSceneLight();
	virtual ~VMainSceneLight();

	void updateLightInfo();
	void updateLight(VFLOAT dt);

	void addLastTime(VFLOAT time)
	{
		mLastTime += time;
	}
	void setLastTime(VFLOAT time)
	{
		mLastTime = time;
	}

	void setColorValue(const Ogre::ColourValue &color)
	{
		mDestColor = color;
	}

	void setFadeTime(VFLOAT fadeInTime, VFLOAT fadeOutTime)
	{
		mFadeInTime = fadeInTime;
		mFadeOutTime = fadeOutTime;
		mCurrentFadeInTime = 0.0f;
		mCurrentFadeOutTime = 0.0f;
		mLightModifyPhase = VLMP_UPDATE;
	}

	VLightModifyPhase getLightModifyPhase() const
	{
		return mLightModifyPhase;
	}

protected:
	Ogre::Light	*mMainSceneLightOne;
	Ogre::Light	*mMainSceneLightTwo;

	VFLOAT	mLastTime;
	VFLOAT	mFadeInTime;
	VFLOAT	mCurrentFadeInTime;
	VFLOAT	mFadeOutTime;
	VFLOAT	mCurrentFadeOutTime;

	VFLOAT	mCurrentTime;

	Ogre::ColourValue	mOriginOneLightColor;
	Ogre::ColourValue	mOriginTwoLightColor;
	Ogre::ColourValue	mOriginAmbientColor;
	Ogre::ColourValue	mDestColor;

	VLightModifyPhase	mLightModifyPhase;
};


#endif	/*__VISION_MAIN_SCENE_LIGHT_H__*/
