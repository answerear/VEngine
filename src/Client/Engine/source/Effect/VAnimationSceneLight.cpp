
#include "VAnimationSceneLight.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VAnimationSceneLight::CmdAttachTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationSceneLight*>(target)->getAttachTime());
};

void VAnimationSceneLight::CmdAttachTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationSceneLight*>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationSceneLight::CmdLastTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationSceneLight*>(target)->getLastTime());
};

void VAnimationSceneLight::CmdLastTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationSceneLight*>(target)->setLastTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationSceneLight::CmdFadeInTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationSceneLight*>(target)->getFadeInTime());
};

void VAnimationSceneLight::CmdFadeInTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationSceneLight*>(target)->setFadeInTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationSceneLight::CmdFadeOutTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationSceneLight*>(target)->getFadeOutTime());
};

void VAnimationSceneLight::CmdFadeOutTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationSceneLight*>(target)->setFadeOutTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationSceneLight::CmdDestColor::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationSceneLight*>(target)->getDestColour());
};

void VAnimationSceneLight::CmdDestColor::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationSceneLight*>(target)->setDestColor(Ogre::StringConverter::parseColourValue(val));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// VAnimationSceneLight
////////////////////////////////////////////////////////////////////////////////////////////////////

VAnimationSceneLight::CmdAttachTime  VAnimationSceneLight::msAttachTimeCmd;
VAnimationSceneLight::CmdLastTime    VAnimationSceneLight::msLastTimeCmd;
VAnimationSceneLight::CmdFadeInTime  VAnimationSceneLight::msFadeInTimeCmd;
VAnimationSceneLight::CmdFadeOutTime VAnimationSceneLight::msFadeOutTimeCmd;
VAnimationSceneLight::CmdDestColor  VAnimationSceneLight::msDestColorCmd;

VAnimationSceneLight::VAnimationSceneLight()
	: mAttachTime(0.0f)
	, mLastTime(0.0f)
	, mFadeInTime(0.0f)
	, mFadeOutTime(0.0f)
	, mDestColor(Ogre::ColourValue::Black)
{
	if (createParamDictionary("AnimationSceneLightInfo"))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();

		dict->addParameter(Ogre::ParameterDef("AttachTime", 
			"the time that starting changing the light.",
			Ogre::PT_REAL),
			&msAttachTimeCmd);	

		dict->addParameter(Ogre::ParameterDef("LastTime", 
			"the last time that changing the light.",
			Ogre::PT_REAL),
			&msLastTimeCmd);			

		dict->addParameter(Ogre::ParameterDef("FadeInTime", 
			"the fade in time.",
			Ogre::PT_REAL),
			&msFadeInTimeCmd);

		dict->addParameter(Ogre::ParameterDef("FadeOutTime", 
			"the fade out time.",
			Ogre::PT_REAL),
			&msFadeOutTimeCmd);

		dict->addParameter(Ogre::ParameterDef("DestColour", 
			"the dest colour of light.",
			Ogre::PT_COLOURVALUE),
			&msDestColorCmd);
	}
}

VAnimationSceneLight::~VAnimationSceneLight()
{

}

void VAnimationSceneLight::copyParameters(VAnimationSceneLight &other) const
{
	other.mAttachTime = mAttachTime;
	other.mLastTime = mLastTime;
	other.mFadeInTime = mFadeInTime;
	other.mFadeOutTime = mFadeOutTime;
	other.mDestColor = mDestColor;
}
