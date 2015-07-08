

#include "VSkill.h"
#include "VEffect.h"
#include "VAnimationEffectInfo.h"
#include "VAnimationRibbon.h"
#include "VAnimationSound.h"
#include "VAnimationSceneLight.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////
Ogre::String VSkill::CmdAnimationName::doGet(const void *target) const
{
	return static_cast<const VSkill*>(target)->getAnimationName();
}

void VSkill::CmdAnimationName::doSet(void *target, const Ogre::String &val)
{
	static_cast<VSkill*>(target)->setAnimationName(val);
}

Ogre::String VSkill::CmdBreakTime::doGet(const void *target) const
{
	return static_cast<const VSkill*>(target)->getBreakTimeString();
}

void VSkill::CmdBreakTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VSkill*>(target)->setBreakTimeByString(val);
}

Ogre::String VSkill::CmdHitTime::doGet(const void *target) const
{
	return static_cast<const VSkill*>(target)->getHitTimeString();
}

void VSkill::CmdHitTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VSkill*>(target)->setHitTimeByString(val);
}

Ogre::String VSkill::CmdShakeTime::doGet(const void *target) const
{
	return static_cast<const VSkill*>(target)->getShakeTimeString();
}

void VSkill::CmdShakeTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VSkill*>(target)->setShakeTimeByString(val);
}

Ogre::String VSkill::CmdRepeatEffect::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VSkill*>(target)->getRepeatEffect());
}

void VSkill::CmdRepeatEffect::doSet(void *target, const Ogre::String &val)
{
	static_cast<VSkill*>(target)->setRepeatEffect(Ogre::StringConverter::parseBool(val));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// VSkill
////////////////////////////////////////////////////////////////////////////////////////////////////

VSkill::CmdAnimationName VSkill::msAnimationNameCmd;
VSkill::CmdBreakTime VSkill::msBreakTimeCmd;
VSkill::CmdHitTime VSkill::msHitTimeCmd;
VSkill::CmdShakeTime VSkill::msShakeTimeCmd;
VSkill::CmdRepeatEffect VSkill::msRepeatEffectCmd;

VSkill::VSkill(const VString &skillName)
	: mSkillName(skillName)
	, mAnimName("")
	, mBreakTimeArray()
	, mHitTimeArray()
	, mShakeTimeArray()
	, mRepeatEffect(VFALSE)
	, mAnimationEffectInfos()
	, mAnimationRibbons()
	, mAnimationSceneLights()
	, mAnimationSounds()
{
	mBreakTimeArray.push_back(-1.0f);
	mHitTimeArray.push_back(-1.0f);
	mShakeTimeArray.push_back(-1.0f);

	_initParamDictionary();
}

VSkill::~VSkill()
{
	removeAllAnimationEffectInfos();
	removeAllAnimationRibbons();
	removeAllAnimationSounds();
	removeAllAnimationSceneLights();
}

VSkill::VSkill(const VSkill &other)
{
	_clone(other);
}

VSkill &VSkill::operator =(const VSkill &other)
{
	_clone(other);
	return *this;
}

void VSkill::shutdown()
{
	VAnimationEffectInfos::iterator itr = mAnimationEffectInfos.begin();

	while (itr != mAnimationEffectInfos.end())
	{
		VAnimationEffectInfo *info = *itr;
		VEffect* effect = info->getEffect();

		if (effect != VNULL)
		{
			effect->shutdown();
		}

		++itr;
	}
}

void VSkill::setBreakTimeByString(const VString &time)
{
	mBreakTimeArray.clear();

	Ogre::StringVector breakTimes = Ogre::StringUtil::split(time);

	for (size_t i = 0; i < breakTimes.size(); ++i)
	{
		mBreakTimeArray.push_back( Ogre::StringConverter::parseReal(breakTimes[i]) );
	}
}

VString VSkill::getBreakTimeString() const
{
	Ogre::String str("");

	for (size_t i = 0; i < mBreakTimeArray.size(); ++i)
	{
		str += Ogre::StringConverter::toString(mBreakTimeArray[i]);
		str += " ";
	}

	return str;
}

void VSkill::setHitTimeByString(const VString &time)
{
	mHitTimeArray.clear();

	Ogre::StringVector hitTimes = Ogre::StringUtil::split(time);

	for (size_t i = 0; i < hitTimes.size(); ++i)
	{
		mHitTimeArray.push_back(Ogre::StringConverter::parseReal(hitTimes[i]));
	}
}

VString VSkill::getHitTimeString() const
{
	Ogre::String str("");

	for (size_t i = 0; i < mHitTimeArray.size(); ++i)
	{
		str += Ogre::StringConverter::toString(mHitTimeArray[i]);
		str += " ";
	}

	return str;
}

void VSkill::setShakeTimeByString(const VString &time)
{
	mShakeTimeArray.clear();

	Ogre::StringVector shakeTimes = Ogre::StringUtil::split(time);

	for (size_t i = 0; i < shakeTimes.size(); ++i)
	{
		mShakeTimeArray.push_back( Ogre::StringConverter::parseReal(shakeTimes[i]) );
	}
}

VString VSkill::getShakeTimeString() const
{
	Ogre::String str("");

	for (size_t i = 0; i < mShakeTimeArray.size(); ++i)
	{
		str += Ogre::StringConverter::toString(mShakeTimeArray[i]);
		str += " ";
	}

	return str;
}

void VSkill::copyParameters(VSkill *other) const
{
	other->mAnimName = mAnimName;
	other->mBreakTimeArray = mBreakTimeArray;
	other->mHitTimeArray = mHitTimeArray;
	other->mShakeTimeArray = mShakeTimeArray;
	other->mRepeatEffect = mRepeatEffect;
}

VAnimationEffectInfo *VSkill::addAnimationEffectInfo()
{
	VAnimationEffectInfo *effectInfo = new VAnimationEffectInfo();
	assert (effectInfo != VNULL);

	mAnimationEffectInfos.push_back(effectInfo);

	return effectInfo;
}

void VSkill::removeAnimationEffectInfo(VINT32 index)
{
	assert(index < (VINT32)mAnimationEffectInfos.size() && "Animation Effect Info index out of bounds!");
	VAnimationEffectInfos::iterator itr = mAnimationEffectInfos.begin() + index;

	if (*itr)
	{
		delete *itr;
		*itr = NULL;
	}

	mAnimationEffectInfos.erase(itr);
}

void VSkill::removeAllAnimationEffectInfos()
{
	VAnimationEffectInfos::iterator itr = mAnimationEffectInfos.begin();
	while (itr != mAnimationEffectInfos.end())
	{
		if (*itr)
		{
			delete *itr;
			*itr = VNULL;
		}

		++itr;
	}

	mAnimationEffectInfos.clear();
}

VAnimationEffectInfo *VSkill::getAnimationEffectInfo(VINT32 index) const
{
	assert(index < (VINT32)mAnimationEffectInfos.size() && "Animation Effect Info index out of bounds!");
	return mAnimationEffectInfos[index];
}

VINT32 VSkill::getAnimationEffectInfoCount() const
{
	return static_cast<VINT32>(mAnimationEffectInfos.size());
}

VAnimationRibbon *VSkill::addAnimationRibbon()
{
	VAnimationRibbon *ribbon = new VAnimationRibbon();
	assert(ribbon != VNULL);

	mAnimationRibbons.push_back(ribbon);

	return ribbon;
}

void VSkill::removeAnimationRibbon(VINT32 index)
{
	assert(index < (VINT32)mAnimationRibbons.size() && "Animation ribbon index out of bounds!");
	VAnimationRibbons::iterator itr = mAnimationRibbons.begin() + index;

	if (*itr)
	{
		delete *itr;
		*itr = VNULL;
	}

	mAnimationRibbons.erase(itr);
}

void VSkill::removeAllAnimationRibbons()
{
	VAnimationRibbons::iterator itr = mAnimationRibbons.begin();

	while (itr != mAnimationRibbons.end())
	{
		if (*itr)
		{
			delete *itr;
			*itr = VNULL;
		}

		++itr;
	}

	mAnimationRibbons.clear();
}

VAnimationRibbon *VSkill::getAnimationRibbon(VINT32 index) const
{
	assert(index < (VINT32)mAnimationRibbons.size() && "Animation ribbon index out of bounds!");
	return mAnimationRibbons[index];
}

VINT32 VSkill::getAnimationRibbonCount() const
{
	return static_cast<VINT32>(mAnimationRibbons.size());
}

VAnimationSceneLight *VSkill::addAnimationSceneLight()
{
	VAnimationSceneLight *sceneLight = new VAnimationSceneLight();
	assert (sceneLight != VNULL);

	mAnimationSceneLights.push_back(sceneLight);

	return sceneLight;
}

void VSkill::removeAnimationSceneLight(VINT32 index)
{
	assert(index < (VINT32)mAnimationSceneLights.size() && "Animation scene light index out of bounds!");
	VAnimationSceneLights::iterator itr = mAnimationSceneLights.begin() + index;

	if (*itr)
	{
		delete *itr;
		*itr = NULL;
	}

	mAnimationSceneLights.erase(itr);
}

void VSkill::removeAllAnimationSceneLights()
{
	VAnimationSceneLights::iterator itr = mAnimationSceneLights.begin();

	while (itr != mAnimationSceneLights.end())
	{
		if (*itr)
		{
			delete *itr;
			*itr = VNULL;
		}

		++itr;
	}

	mAnimationSceneLights.clear();
}

VAnimationSceneLight *VSkill::getAnimationSceneLight(VINT32 index) const
{
	assert(index < (VINT32)mAnimationSceneLights.size() && "Animation scene light index out of bounds!");
	return mAnimationSceneLights[index];
}

VINT32 VSkill::getAnimationSceneLightCount() const
{
	return static_cast<VINT32>(mAnimationSceneLights.size());
}

VAnimationSound *VSkill::addAnimationSound()
{
	VAnimationSound *sound = new VAnimationSound();
	assert (sound != VNULL);

	mAnimationSounds.push_back(sound);

	return sound;
}

void VSkill::removeAnimationSound(VINT32 index)
{
	assert(index < (VINT32)mAnimationSounds.size() && "Animation sound index out of bounds!");
	VAnimationSounds::iterator itr = mAnimationSounds.begin() + index;

	if (*itr)
	{
		delete *itr;
		*itr = NULL;
	}

	mAnimationSounds.erase(itr);
}

void VSkill::removeAllAnimationSounds()
{
	VAnimationSounds::iterator itr = mAnimationSounds.begin();

	while (itr != mAnimationSounds.end())
	{
		if (*itr)
		{
			delete *itr;
			*itr = VNULL;
		}

		++itr;
	}

	mAnimationSounds.clear();
}

VAnimationSound *VSkill::getAnimationSound(VINT32 index) const
{
	assert(index < (VINT32)mAnimationSounds.size() && "Animation sound index out of bounds!");
	return mAnimationSounds[index];
}

VINT32 VSkill::getAnimationSoundCount() const
{
	return static_cast<VINT32>(mAnimationSounds.size());
}


VBOOL VSkill::_initParamDictionary()
{
	if (createParamDictionary("Skill"))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();			

		dict->addParameter(Ogre::ParameterDef("Animation", 
			"name of animation.",
			Ogre::PT_STRING),
			&msAnimationNameCmd);	  

		dict->addParameter(Ogre::ParameterDef("BreakTime", 
			"the break time of skill.",
			Ogre::PT_STRING),
			&msBreakTimeCmd);	  

		dict->addParameter(Ogre::ParameterDef("HitTime", 
			"the hit time of skill.",
			Ogre::PT_STRING),
			&msHitTimeCmd);	  

		dict->addParameter(Ogre::ParameterDef("ShakeTime", 
			"the shake time of skill.",
			Ogre::PT_STRING),
			&msShakeTimeCmd);	 

		dict->addParameter(Ogre::ParameterDef("RepeatEffect", 
			"whether recreate effect when the animation is loop.",
			Ogre::PT_BOOL),
			&msRepeatEffectCmd);	  

		return VTRUE;
	}

	return VFALSE;
}

void VSkill::_clone(const VSkill &other)
{
	removeAllAnimationEffectInfos();

	// Copy effect infos
	VINT32 i;
	for (i = 0; i < other.getAnimationEffectInfoCount(); ++i)
	{
		VAnimationEffectInfo *rhsAEI = other.getAnimationEffectInfo(i);
		VAnimationEffectInfo *newAEI = addAnimationEffectInfo();

		assert ( rhsAEI && newAEI );

		//	rhsAEI->copyParametersTo(newAEI);
		rhsAEI->copyParameters(*newAEI);
	}

	removeAllAnimationRibbons();

	// copy ribbons
	for (i = 0; i < other.getAnimationRibbonCount(); ++i)
	{
		VAnimationRibbon *rhsAEI = other.getAnimationRibbon(i);
		VAnimationRibbon *newAEI = addAnimationRibbon();

		assert ( rhsAEI && newAEI );

		//	rhsAEI->copyParametersTo(newAEI);
		rhsAEI->copyParameters(*newAEI);
	}

	removeAllAnimationSounds();

	// copy sounds
	for (i = 0; i < other.getAnimationSoundCount(); ++i)
	{
		VAnimationSound *rhsAEI = other.getAnimationSound(i);
		VAnimationSound *newAEI = addAnimationSound();

		assert ( rhsAEI && newAEI );

		//rhsAEI->copyParametersTo(newAEI);
		rhsAEI->copyParameters(*newAEI);
	}

	removeAllAnimationSceneLights();

	// Copy effect infos
	for(i = 0; i < other.getAnimationSceneLightCount(); ++i)
	{
		VAnimationSceneLight *rhsAEI = other.getAnimationSceneLight(i);
		VAnimationSceneLight *newAEI = addAnimationSceneLight();

		assert ( rhsAEI && newAEI );

		//  rhsAEI->copyParametersTo(newAEI);
		rhsAEI->copyParameters(*newAEI);
	}

	// copy skill parameter
	//	rhs.copyParametersTo(this);
	other.copyParameters(this);
}

