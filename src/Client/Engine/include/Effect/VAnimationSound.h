

#ifndef __VISION_ANIMATION_SOUND_H__
#define __VISION_ANIMATION_SOUND_H__


#include "VEngineConfig.h"


class VAnimationSound : public Ogre::StringInterface
{
protected:
	class CmdAttachTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void*target, const Ogre::String &val);
	};

	class CmdSoundName : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void*target, const Ogre::String &val);
	};

public:
	VAnimationSound();
	~VAnimationSound();

	void setAttachTime(VFLOAT time)
	{
		mAttachTime = time;
	}
	VFLOAT getAttachTime() const
	{
		return mAttachTime;
	}

	void setSoundName(const VString &name)
	{
		mSoundName = name;
	}
	const VString &getSoundName() const
	{
		return mSoundName;
	}

	void copyParameters(VAnimationSound &other) const;

protected:
	static CmdAttachTime msAttachTimeCmd;
	static CmdSoundName msSoundNameCmd;

	VFLOAT	mAttachTime;
	VString	mSoundName;
};


#endif	/*__VISION_ANIMATION_SOUND_H__*/
