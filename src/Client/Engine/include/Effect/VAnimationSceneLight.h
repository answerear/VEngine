

#ifndef __VISION_ANIMATION_SCENE_LIGHT_H__
#define __VISION_ANIMATION_SCENE_LIGHT_H__


#include "VEngineConfig.h"


class VAnimationSceneLight : public Ogre::StringInterface
{
protected:
	class CmdAttachTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdLastTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdFadeInTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdFadeOutTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdDestColor : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VAnimationSceneLight();
	~VAnimationSceneLight();

	void setAttachTime(VFLOAT time)
	{
		mAttachTime = time;
	}
	VFLOAT getAttachTime() const
	{
		return mAttachTime;
	}

	void setLastTime(VFLOAT time)
	{
		mLastTime = time;
	}
	VFLOAT getLastTime() const
	{
		return mLastTime;
	}

	void setFadeInTime(VFLOAT time)
	{
		mFadeInTime = time;
	}
	VFLOAT getFadeInTime() const
	{
		return mFadeInTime;
	}

	void setFadeOutTime(VFLOAT time)
	{
		mFadeOutTime = time;
	}
	VFLOAT getFadeOutTime() const
	{
		return mFadeOutTime;
	}

	void setDestColor(const ColorValue& color)
	{
		mDestColor = color;
	}
	const ColorValue& getDestColour(void) const
	{
		return mDestColor;
	}

	void copyParameters(VAnimationSceneLight &other) const;

protected:
	static CmdAttachTime msAttachTimeCmd;
	static CmdLastTime msLastTimeCmd;
	static CmdFadeInTime msFadeInTimeCmd;
	static CmdFadeOutTime msFadeOutTimeCmd;
	static CmdDestColor msDestColorCmd;

	VFLOAT	mAttachTime;
	VFLOAT	mLastTime;
	VFLOAT	mFadeInTime;
	VFLOAT	mFadeOutTime;

	Ogre::ColourValue	mDestColor;
};


#endif	/*__VISION_ANIMATION_SCENE_LIGHT_H__*/

