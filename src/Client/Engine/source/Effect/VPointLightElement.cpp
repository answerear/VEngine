

#include "VEngine.h"
#include "VGraphicsSystem.h"
#include "VPointLightElement.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VPointLightElement::CmdDiffuse::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VPointLightElement*>(target)->getDiffuse());
}

void VPointLightElement::CmdDiffuse::doSet(void *target, const Ogre::String &val)
{
	static_cast<VPointLightElement*>(target)->setDiffuse(Ogre::StringConverter::parseColourValue(val));
}

Ogre::String VPointLightElement::CmdSpecular::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VPointLightElement*>(target)->getSpecular());
}

void VPointLightElement::CmdSpecular::doSet(void *target, const Ogre::String &val)
{
	static_cast<VPointLightElement*>(target)->setSpecular(Ogre::StringConverter::parseColourValue(val));
}

Ogre::String VPointLightElement::CmdRange::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VPointLightElement*>(target)->getRange());
}

void VPointLightElement::CmdRange::doSet(void *target, const Ogre::String &val)
{
	static_cast<VPointLightElement*>(target)->setRange(Ogre::StringConverter::parseReal(val));
}

Ogre::String VPointLightElement::CmdAttenuationConstant::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VPointLightElement*>(target)->getAttenuationConstant());
}

void VPointLightElement::CmdAttenuationConstant::doSet(void* target, const Ogre::String &val)
{
	static_cast<VPointLightElement*>(target)->setAttenuationConstant(Ogre::StringConverter::parseReal(val));
}

Ogre::String VPointLightElement::CmdAttenuationLinear::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VPointLightElement*>(target)->getAttenuationLinear());
}

void VPointLightElement::CmdAttenuationLinear::doSet(void* target, const Ogre::String &val)
{
	static_cast<VPointLightElement*>(target)->setAttenuationLinear(Ogre::StringConverter::parseReal(val));
}

Ogre::String VPointLightElement::CmdAttenuationQuadric::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VPointLightElement*>(target)->getAttenuationQuadric());
}

void VPointLightElement::CmdAttenuationQuadric::doSet(void *target, const Ogre::String &val)
{
	static_cast<VPointLightElement*>(target)->setAttenuationQuadric(Ogre::StringConverter::parseReal(val));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// class VPointLightElement
////////////////////////////////////////////////////////////////////////////////////////////////////

VPointLightElement::CmdDiffuse VPointLightElement::msDiffuseCmd;
VPointLightElement::CmdSpecular VPointLightElement::msSpecularCmd;
VPointLightElement::CmdRange VPointLightElement::msRangeCmd;
VPointLightElement::CmdAttenuationConstant VPointLightElement::msAttenuationConstantCmd;
VPointLightElement::CmdAttenuationLinear VPointLightElement::msAttenuationLinearCmd;
VPointLightElement::CmdAttenuationQuadric VPointLightElement::msAttenuationQuadricCmd;


VPointLightElement::VPointLightElement(const VString &type)
	: VEffectElement(type)
	, mLight(VNULL)
	, mDiffuse(Ogre::ColourValue::White)
	, mSpecular(Ogre::ColourValue::Black)
	, mRange(1000.0f)
	, mAttenuationConstant(1.0f)
	, mAttenuationLinear(0.0f)
	, mAttenuationQuadric(0.0f)
{
	if (_initDefaults(mElementType))
	{
		Ogre::ParamDictionary *dict = getParamDictionary();   

		dict->addParameter(Ogre::ParameterDef("Diffuse", 
			"diffuse colour of point light.",
			Ogre::PT_COLOURVALUE),
			&msDiffuseCmd);

		dict->addParameter(Ogre::ParameterDef("Specular", 
			"specular colour of point light.",
			Ogre::PT_COLOURVALUE),
			&msSpecularCmd);

		dict->addParameter(Ogre::ParameterDef("Range", 
			"range of point light.",
			Ogre::PT_REAL),
			&msRangeCmd);

		dict->addParameter(Ogre::ParameterDef("AttenuationConstant", 
			"attenuation constant of point light.",
			Ogre::PT_REAL),
			&msAttenuationConstantCmd);

		dict->addParameter(Ogre::ParameterDef("AttenuationLinear", 
			"attenuation linear of point light.",
			Ogre::PT_REAL),
			&msAttenuationLinearCmd);

		dict->addParameter(Ogre::ParameterDef("AttenuationQuadric", 
			"attenuation quadric of point light.",
			Ogre::PT_REAL),
			&msAttenuationQuadricCmd);
	}
}

VPointLightElement::~VPointLightElement()
{
	destroyRenderInstance();
}

VBOOL VPointLightElement::createRenderInstance()
{
	return _createLight();
}

void VPointLightElement::destroyRenderInstance()
{
	_destroyLight();
	VEffectElement::destroyRenderInstance();
}

void VPointLightElement::setVisibilityFlags(VUINT32 flags)
{
	if (mLight != VNULL)
		mLight->setVisibilityFlags(flags);
}

void VPointLightElement::setDiffuse(const Ogre::ColourValue &diffuse)
{
	mDiffuse = diffuse;

	if (mLight != VNULL)
		mLight->setDiffuseColour(diffuse);
}

void VPointLightElement::setSpecular(const Ogre::ColourValue &specular)
{
	mSpecular = specular;

	if (mLight != VNULL)
		mLight->setSpecularColour(specular);
}

void VPointLightElement::setRange(const Ogre::Real &range)
{
	mRange = range;

	if (mLight != VNULL)
		mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
}

void VPointLightElement::setAttenuationConstant(const Ogre::Real &constant)
{
	mAttenuationConstant = constant;

	if (mLight != VNULL)
		mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
}

void VPointLightElement::setAttenuationLinear(const Ogre::Real &linear)
{
	mAttenuationLinear = linear;

	if (mLight != VNULL)
		mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
}

void VPointLightElement::setAttenuationQuadric(const Ogre::Real &quadric)
{
	mAttenuationQuadric = quadric;

	if (mLight != VNULL)
		mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
}

VBOOL VPointLightElement::_createLight()
{
	assert(VNULL == mLight);

	VBOOL result = VFALSE;

	if (mBaseNode != VNULL)
	{
		Ogre::SceneManager *sceneMgr = VENGINE.getGfxSystem()->getSceneManager();
		mLight = sceneMgr->createLight(mBaseNode->getName());

		// 初始化光源
		mLight->setDiffuseColour(mDiffuse);
		mLight->setSpecularColour(mSpecular);
		mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
		mLight->setCastShadows(VFALSE);
		mLight->setVisible(mVisible);

		mBaseNode->attachObject(mLight);
		mBaseNode->setPosition(mRelPos);
		mBaseNode->setOrientation(mRelOrientation);

		result = VTRUE;
	}

	return result;
}

void VPointLightElement::_destroyLight()
{
	if (mLight != VNULL)
	{
		assert(mLight->getParentSceneNode());
		assert(mLight->getParentSceneNode()->getCreator());
		mLight->getParentSceneNode()->getCreator()->destroyLight(mLight->getName());
		mLight = VNULL;
	}
}
