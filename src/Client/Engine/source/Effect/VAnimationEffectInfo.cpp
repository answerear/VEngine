
#include "VAnimationEffectInfo.h"
#include "VEffectManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VAnimationEffectInfo::CmdAttachTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationEffectInfo*>(target)->getAttachTime());
}

void VAnimationEffectInfo::CmdAttachTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationEffectInfo*>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationEffectInfo::CmdAttachPoint::doGet(const void *target) const
{
	return static_cast<const VAnimationEffectInfo*>(target)->getAttachPoint();
}

void VAnimationEffectInfo::CmdAttachPoint::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationEffectInfo*>(target)->setAttachPoint(val);
}

Ogre::String VAnimationEffectInfo::CmdEffectTemplateName::doGet(const void *target) const
{
	return static_cast<const VAnimationEffectInfo*>(target)->getEffectTemplateName();
}

void VAnimationEffectInfo::CmdEffectTemplateName::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationEffectInfo*>(target)->setEffectTemplateName(val);
}

Ogre::String VAnimationEffectInfo::CmdOffsetPos::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationEffectInfo*>(target)->getOffsetPos());
}

void VAnimationEffectInfo::CmdOffsetPos::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationEffectInfo*>(target)->setOffsetPos(Ogre::StringConverter::parseVector3(val));
}

Ogre::String VAnimationEffectInfo::CmdOffsetRotation::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationEffectInfo*>(target)->getOffsetRotation());
}

void VAnimationEffectInfo::CmdOffsetRotation::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationEffectInfo*>(target)->setOffsetRotation(Ogre::StringConverter::parseQuaternion(val));
}

Ogre::String VAnimationEffectInfo::CmdAttach::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationEffectInfo*>(target)->getAttach());
}

void VAnimationEffectInfo::CmdAttach::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationEffectInfo*>(target)->setAttach(Ogre::StringConverter::parseBool(val));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// VAnimationEffectInfo
////////////////////////////////////////////////////////////////////////////////////////////////////

VAnimationEffectInfo::CmdAttachTime	VAnimationEffectInfo::msAttachTimeCmd;
VAnimationEffectInfo::CmdAttachPoint VAnimationEffectInfo::msAttachPointCmd;
VAnimationEffectInfo::CmdEffectTemplateName	VAnimationEffectInfo::msEffectTemplateNameCmd;
VAnimationEffectInfo::CmdOffsetPos VAnimationEffectInfo::msOffsetPosCmd;
VAnimationEffectInfo::CmdOffsetRotation	VAnimationEffectInfo::msOffsetRotationCmd;
VAnimationEffectInfo::CmdAttach	VAnimationEffectInfo::msAttachCmd;


VAnimationEffectInfo::VAnimationEffectInfo()
	: mEffect(VNULL)
	, mAttachTime(0.0f)
	, mAttachPoint("")
	, mEffectTemplateName("")
	, mOffsetPos(Ogre::Vector3::ZERO)
	, mOffsetRotation(Ogre::Quaternion::IDENTITY)
	, mAttach(VFALSE)
{
	_initParamDictionary();
}

VAnimationEffectInfo::~VAnimationEffectInfo()
{
	if (mEffect != VNULL)
	{
		VEffectManager::instance().removeEffect(mEffect, VFALSE, VFALSE);
		mEffect = VNULL;
	}
}

void VAnimationEffectInfo::copyParameters(VAnimationEffectInfo &other) const
{
	other.mAttachTime = mAttachTime;
	other.mAttachPoint = mAttachPoint;
	other.mEffectTemplateName = mEffectTemplateName;
	other.mOffsetPos = mOffsetPos;
	other.mOffsetRotation = mOffsetRotation;
	other.mAttach = mAttach;
}

VBOOL VAnimationEffectInfo::_initParamDictionary(void)
{
	if (createParamDictionary("AnimationEffectInfo"))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();			

		dict->addParameter(Ogre::ParameterDef("AttachTime", 
			"the time that showing the effect.",
			Ogre::PT_REAL),
			&msAttachTimeCmd);	  

		dict->addParameter(Ogre::ParameterDef("AttachPoint", 
			"the attached bone or locator.",
			Ogre::PT_STRING),
			&msAttachPointCmd);	  

		dict->addParameter(Ogre::ParameterDef("EffectTemplateName", 
			"name of effect template.",
			Ogre::PT_STRING),
			&msEffectTemplateNameCmd);	  

		dict->addParameter(Ogre::ParameterDef("OffsetPos", 
			"offset position to the attach point.",
			Ogre::PT_VECTOR3),
			&msOffsetPosCmd);	  

		dict->addParameter(Ogre::ParameterDef("OffsetRotation", 
			"offset orientation to the attach point.",
			Ogre::PT_QUATERNION),
			&msOffsetRotationCmd);	  

		dict->addParameter(Ogre::ParameterDef("Attach", 
			"whether the effect will move along with the attach point.",
			Ogre::PT_BOOL),
			&msAttachCmd);	  

		return VTRUE;
	}

	return VFALSE;
}

