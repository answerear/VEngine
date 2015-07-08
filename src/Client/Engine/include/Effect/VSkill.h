

#ifndef __VISION_SKILL_H__
#define __VISION_SKILL_H__


#include "VEngineConfig.h"

class VAnimationEffectInfo;
class VAnimationRibbon;
class VAnimationSceneLight;
class VAnimationSound;

class VSkill : public Ogre::StringInterface
{
protected:
	typedef std::vector<Ogre::Real> VTimeArray;

	typedef std::vector<VAnimationEffectInfo*> VAnimationEffectInfos;
	typedef std::vector<VAnimationRibbon*> VAnimationRibbons;
	typedef std::vector<VAnimationSceneLight*> VAnimationSceneLights;
	typedef std::vector<VAnimationSound*> VAnimationSounds;

	class CmdAnimationName : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdBreakTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdHitTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdShakeTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdRepeatEffect : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VSkill(const VString &skillName);
	~VSkill();

	VSkill(const VSkill &other);
	VSkill &operator =(const VSkill &other);

	/** 进行一些skill中各种元素的清除操作 */
	void shutdown();

	/**
	 * @brief 获取技能名称
	 */
	const VString &getSkillName() const
	{
		return mSkillName;
	}

	/**
	 * @brief 获取技能动画名称
	 */
	const VString &getAnimationName() const
	{
		return mAnimName;
	}
	/** 
	 * @brief 设置技能动画名称
	 */
	void setAnimationName(const VString &name)
	{
		mAnimName = name;
	}

	void setBreakTime(const Real &time, VWORD index)
	{
		assert(index < mBreakTimeArray.size());
		mBreakTimeArray[index] = time;
	}
	const Real &getBreakTime(VWORD index) const
	{
		assert(index < mBreakTimeArray.size());
		return mBreakTimeArray[index];
	}

	void setBreakTimeByString(const VString &time);
	VString getBreakTimeString() const;

	VWORD getNumBreakTime() const
	{
		return static_cast<VWORD>(mBreakTimeArray.size());
	}

	void setHitTime(const Real &time, VWORD index)
	{
		assert(index < mHitTimeArray.size());
		mHitTimeArray[index] = time;
	}
	const Real &getHitTime(VWORD index) const
	{
		assert(index < mHitTimeArray.size());
		return mHitTimeArray[index];
	}

	void setHitTimeByString(const VString &time);
	VString getHitTimeString() const;

	VWORD getNumHitTime() const
	{
		return static_cast<VWORD>(mHitTimeArray.size());
	}

	void setShakeTime(const Real &time, VWORD index)
	{
		assert(index < mShakeTimeArray.size());
		mShakeTimeArray[index] = time;
	}
	const Real &getShakeTime(VWORD index) const
	{
		assert(index < mShakeTimeArray.size());
		return mShakeTimeArray[index];
	}

	void setShakeTimeByString(const VString &time);
	VString getShakeTimeString() const;

	VWORD getNumShakeTime() const
	{
		return static_cast<VWORD>(mShakeTimeArray.size());
	}

	void setRepeatEffect(VBOOL repeat)
	{
		mRepeatEffect = repeat;
	}
	VBOOL getRepeatEffect() const
	{
		return mRepeatEffect;
	}

	void copyParameters(VSkill *other) const;

	VAnimationEffectInfo *addAnimationEffectInfo();
	void removeAnimationEffectInfo(VINT32 index);
	void removeAllAnimationEffectInfos();
	VAnimationEffectInfo *getAnimationEffectInfo(VINT32 index) const;
	VINT32 getAnimationEffectInfoCount() const;

	VAnimationRibbon *addAnimationRibbon();
	void removeAnimationRibbon(VINT32 index);
	void removeAllAnimationRibbons();
	VAnimationRibbon *getAnimationRibbon(VINT32 index) const;
	VINT32 getAnimationRibbonCount() const;

	VAnimationSceneLight *addAnimationSceneLight();
	void removeAnimationSceneLight(VINT32 index);
	void removeAllAnimationSceneLights();
	VAnimationSceneLight *getAnimationSceneLight(VINT32 index) const;
	VINT32 getAnimationSceneLightCount() const;

	VAnimationSound *addAnimationSound();
	void removeAnimationSound(VINT32 index);
	void removeAllAnimationSounds();
	VAnimationSound *getAnimationSound(VINT32 index) const;
	VINT32 getAnimationSoundCount() const;

protected:
	VBOOL _initParamDictionary();
	void _clone(const VSkill &other);

protected:
	static CmdAnimationName	msAnimationNameCmd;
	static CmdBreakTime		msBreakTimeCmd;
	static CmdHitTime		msHitTimeCmd;
	static CmdShakeTime		msShakeTimeCmd;
	static CmdRepeatEffect	msRepeatEffectCmd;

	VString		mSkillName;	/**< 技能名称 */
	VString		mAnimName;	/**< 动画名称 */

	VTimeArray	mBreakTimeArray;
	VTimeArray	mHitTimeArray;
	VTimeArray	mShakeTimeArray;

	VBOOL		mRepeatEffect;

	VAnimationEffectInfos mAnimationEffectInfos;
	VAnimationRibbons mAnimationRibbons;
	VAnimationSceneLights mAnimationSceneLights;
	VAnimationSounds mAnimationSounds;
};

#endif	/*__VISION_SKILL_H__*/
