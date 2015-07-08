

#ifndef __VISION_MAIN_SCENE_LIGHT_H__
#define __VISION_MAIN_SCENE_LIGHT_H__


#include "VEngineConfig.h"


/// �����ƹ�仯�ļ����׶�
enum VLightModifyPhase
{
	VLMP_IDLE,       /// ���У���ʱ�����ʹ����Щ�ܿ��Ƴ����ƹ����Ч
	VLMP_UPDATE,     /// ����״̬��ֻ�������״̬�£��ƹ���Ϣ�Ż���иı�
	VLMP_FADEIN,     /// �����ƹ�������fade in״̬
	VLMP_LAST,       /// �����ƹ��������ȶ�״̬
	VLMP_FADEOUT,    /// �����ƹ�������fade out״̬
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
