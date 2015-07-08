

#include "VEffectElement.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VEffectElement::CmdLifeTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VEffectElement*>(target)->getLifeTime());
}

void VEffectElement::CmdLifeTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VEffectElement*>(target)->setLifeTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VEffectElement::CmdStartTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VEffectElement*>(target)->getStartTime());
}

void VEffectElement::CmdStartTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VEffectElement*>(target)->setStartTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VEffectElement::CmdRelPos::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VEffectElement*>(target)->getRelPos());
}

void VEffectElement::CmdRelPos::doSet(void *target, const Ogre::String &val)
{
	static_cast<VEffectElement*>(target)->setRelPos(Ogre::StringConverter::parseVector3(val));
}

Ogre::String VEffectElement::CmdRelOrientation::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VEffectElement*>(target)->getRelOrientation());
}

void VEffectElement::CmdRelOrientation::doSet(void *target, const Ogre::String &val)
{
	static_cast<VEffectElement*>(target)->setRelOrientation(Ogre::StringConverter::parseQuaternion(val));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// class VEffectElement
////////////////////////////////////////////////////////////////////////////////////////////////////

VEffectElement::CmdLifeTime VEffectElement::msLifeTimeCmd;
VEffectElement::CmdStartTime VEffectElement::msStartTimeCmd;
VEffectElement::CmdRelPos VEffectElement::msRelPosCmd;
VEffectElement::CmdRelOrientation VEffectElement::msRelOrientationCmd;

VEffectElement::VEffectElement(const VString &type)
	: VEffectBase()
	, mElementType(type)
	, mParentEffect(VNULL)
	, mLifeTime(0.0f)
	, mStartTime(0.0f)
	, mElapseLifeTime(0.0f)
	, mElapseStartTime(0.0f)
	, mRelPos(Ogre::Vector3::ZERO)
	, mRelOrientation(Ogre::Quaternion::IDENTITY)
	, mIsActive(VTRUE)
	, mIsRenderInstanceCreated(VFALSE)
	, mIsDestroy(VFALSE)
{

}

VEffectElement::~VEffectElement()
{

}

void VEffectElement::updateRenderInstance(const Real &dt, const VTransform &xform)
{

}

void VEffectElement::destroyRenderInstance()
{
	mIsRenderInstanceCreated = VFALSE;
}

void VEffectElement::setTransform(const VTransform &xform)
{
	if (mBaseNode != VNULL)
	{
		Ogre::Matrix3 mat(Ogre::Matrix3::IDENTITY);
		xform.orientation.ToRotationMatrix(mat);
		mBaseNode->setPosition(xform.position + mat * mRelPos);
		mBaseNode->setOrientation(xform.orientation * mRelOrientation);
	}
}

void VEffectElement::createSceneNode(Ogre::SceneNode *parentNode)
{
	assert(parentNode);
	assert(VNULL == mBaseNode);
	mBaseNode = parentNode->createChildSceneNode(mRelPos, mRelOrientation);
	assert(VNULL != mBaseNode);
}

void VEffectElement::update(const Real &dt, VTransform &xform)
{
	Ogre::Real elapseStartTime = mElapseStartTime + dt;
	if (elapseStartTime < mStartTime)
	{
		// 没有到粒子开始时间，更新开始时间，并且返回
		mElapseStartTime = elapseStartTime;
		return;
	}

	if (!mIsRenderInstanceCreated)
	{
		// 没有创建渲染实例，先创建
		if (createRenderInstance())
		{
			mIsRenderInstanceCreated = VTRUE;
		}
	}

	Ogre::Real currentTime = mElapseLifeTime + dt;
	if (currentTime > mLifeTime && mLifeTime > 0.0f)
	{
		// 超出生命周期，销毁自己
		destroy();
	}
	else
	{
		// 生命周期内，更新渲染实例
		mElapseLifeTime = currentTime;
		updateRenderInstance(dt, xform);
	}
}

void VEffectElement::destroy()
{
	mIsDestroy = VTRUE;
}

void VEffectElement::shutdown()
{
	// do nothing
}

void VEffectElement::updateExtraTransforms(const VTransforms &xforms)
{
	// do nothing
}

void VEffectElement::sleep()
{
	mElapseLifeTime = 0.0f;
	mElapseStartTime = 0.0f;

	mIsDestroy = VFALSE;
	mIsActive = VFALSE;

	setVisible(VFALSE);
}

void VEffectElement::active()
{
	mIsActive = VTRUE;
}

void VEffectElement::setRelPos(const Vector3 &pos)
{
	mRelPos = pos;
	if (mBaseNode != VNULL)
	{
		mBaseNode->setPosition(mRelPos);
	}
}

void VEffectElement::setRelOrientation(const Quaternion &orientation)
{
	mRelOrientation = orientation;
	if (mBaseNode != VNULL)
	{
		mBaseNode->setOrientation(orientation);
	}
}

VBOOL VEffectElement::_initDefaults(const VString &type)
{
	if (createParamDictionary(type + "EffectElement"))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();

		dict->addParameter(Ogre::ParameterDef("StartTime", 
			"the start time of this element.",
			Ogre::PT_REAL),
			&msStartTimeCmd);

		dict->addParameter(Ogre::ParameterDef("LifeTime", 
			"the life time of this element.",
			Ogre::PT_REAL),
			&msLifeTimeCmd);

		dict->addParameter(Ogre::ParameterDef("Position", 
			"the relative pos from parent.",
			Ogre::PT_VECTOR3),
			&msRelPosCmd);

		dict->addParameter(Ogre::ParameterDef("Orientation", 
			"the relative orientation from parent.",
			Ogre::PT_QUATERNION),
			&msRelOrientationCmd);

		return VTRUE;

	}

	return VFALSE;
}


