

#include "VAnimationSound.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VAnimationSound::CmdAttachTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationSound*>(target)->getAttachTime());
}

void VAnimationSound::CmdAttachTime::doSet(void*target, const Ogre::String &val)
{
	static_cast<VAnimationSound*>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationSound::CmdSoundName::doGet(const void *target) const
{
	return static_cast<const VAnimationSound*>(target)->getSoundName();
}

void VAnimationSound::CmdSoundName::doSet(void*target, const Ogre::String &val)
{
	static_cast<VAnimationSound*>(target)->setSoundName(val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// VAnimationSound
////////////////////////////////////////////////////////////////////////////////////////////////////

VAnimationSound::CmdAttachTime VAnimationSound::msAttachTimeCmd;
VAnimationSound::CmdSoundName VAnimationSound::msSoundNameCmd;


VAnimationSound::VAnimationSound()
	: mAttachTime(0.0f)
	, mSoundName("")
{
	if (createParamDictionary("AnimationSound"))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();

		dict->addParameter(Ogre::ParameterDef("SoundName", 
			"the file name of sound.",
			Ogre::PT_STRING),
			&msSoundNameCmd);	

		dict->addParameter(Ogre::ParameterDef("AttachTime", 
			"the time that play sound.",
			Ogre::PT_REAL),
			&msAttachTimeCmd);			
	}
}

VAnimationSound::~VAnimationSound()
{

}

void VAnimationSound::copyParameters(VAnimationSound &other) const
{
	other.mAttachTime = mAttachTime;
	other.mSoundName = mSoundName;
}

