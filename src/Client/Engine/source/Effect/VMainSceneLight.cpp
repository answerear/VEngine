
#include "VMainSceneLight.h"
#include "VLightObject.h"


VMainSceneLight::VMainSceneLight()
	: mMainSceneLightOne(VNULL)
	, mMainSceneLightTwo(VNULL)
	, mLastTime(0.0f)
	, mFadeInTime(0.0f)
	, mCurrentFadeInTime(0.0f)
	, mFadeOutTime(0.0f)
	, mCurrentFadeOutTime(0.0f)
	, mCurrentTime(0.0f)
	, mOriginOneLightColor(Ogre::ColourValue::Black)
	, mOriginTwoLightColor(Ogre::ColourValue::Black)
	, mOriginAmbientColor(Ogre::ColourValue::Black)
	, mDestColor(Ogre::ColourValue::Black)
	, mLightModifyPhase(VLMP_IDLE)
{
	
}

VMainSceneLight::~VMainSceneLight()
{

}

void VMainSceneLight::updateLightInfo()
{
	
}

void VMainSceneLight::updateLight(VFLOAT dt)
{

}
