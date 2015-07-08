

#include "VProjectorElement.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VProjectorElement::CmdMaterialName::doGet(const void *target) const
{
	return static_cast<const VProjectorElement*>(target)->getMaterialName();
}

void VProjectorElement::CmdMaterialName::doSet(void *target, const Ogre::String &val)
{
	static_cast<VProjectorElement*>(target)->setMaterialName(val);
}

Ogre::String VProjectorElement::CmdProjectionSize::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VProjectorElement*>(target)->getProjectionSize());
}

void VProjectorElement::CmdProjectionSize::doSet(void *target, const Ogre::String &val)
{
	static_cast<VProjectorElement*>(target)->setProjectionSize(Ogre::StringConverter::parseReal(val));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// VProjectorElement
////////////////////////////////////////////////////////////////////////////////////////////////////

VProjectorElement::CmdMaterialName VProjectorElement::msMaterialNameCmd;
VProjectorElement::CmdProjectionSize VProjectorElement::msProjectionSizeCmd;


VProjectorElement::VProjectorElement(const VString &type)
	: VEffectElement(type)
{
	if (_initDefaults(type))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();

		dict->addParameter(Ogre::ParameterDef("Material", 
			"the name of material of the projector.",
			Ogre::PT_STRING),
			&msMaterialNameCmd);      

		dict->addParameter(Ogre::ParameterDef("ProjectionSize", 
			"the size of projected terrain or object.",
			Ogre::PT_REAL),
			&msProjectionSizeCmd);  

	}
}

VProjectorElement::~VProjectorElement()
{

}

VBOOL VProjectorElement::createRenderInstance()
{
	return VTRUE;
}

void VProjectorElement::updateRenderInstance(const Real &dt, const VTransform &xform)
{

}

void VProjectorElement::destroyRenderInstance()
{

}

void VProjectorElement::setVisibilityFlags(VUINT32 flags)
{

}

void VProjectorElement::setMaterialName(const VString &name)
{

}

void VProjectorElement::setProjectionSize(const Real &size)
{

}
