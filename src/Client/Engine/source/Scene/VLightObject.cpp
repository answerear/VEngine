

#include "VLightObject.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"
#include "VPropertyHelper.h"


const VString VLightObject::msType = "Light";
const VString VLightObject::msCategory = "Light";


/**************************************************************************************************
 *									class VLightObjectFactory
 *************************************************************************************************/

class VLightObjectFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VLightObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VLightObject();
	}
};


/**************************************************************************************************
 *								Property Command of VLightObject
 *************************************************************************************************/

static const VConstant gsLightTypeConstants[] =
{
	{"point",        Ogre::Light::LT_POINT},
    {"directional",  Ogre::Light::LT_DIRECTIONAL},
    {"spotlight",    Ogre::Light::LT_SPOTLIGHT},
    {VNULL}
};

class VLightTypeCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VLightObject *object = static_cast<const VLightObject*>(source);
		return object->mLightType;
	}

	VString doGetAsString(const void *source) const
	{
		VString result;

		const VLightObject *object = static_cast<const VLightObject*>(source);
		if (!mapEnumLabel(gsLightTypeConstants, object->mLightType, result))
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
				"Unknown light type: " + Ogre::StringConverter::toString(object->mLightType),
				"VLightTypeCmd::doGet");
		}

		return result;
	}

	void doSet(void *target, const Variant &value)
	{
		VLightObject *object = static_cast<VLightObject*>(target);
		object->mLightType = VariantCast<Ogre::Light::LightTypes>(value);
		object->updateRenderInstance();
	}

	void doSetAsString(void *target, const VString &value)
	{
		VLightObject *object = static_cast<VLightObject*>(target);
		VINT32 lightType;
		if (!mapEnumValue(gsLightTypeConstants, value, lightType, false))
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
				"Unknown light type: " + value,
				"LightTypeCmd::doSet");
		}

		object->mLightType = static_cast<Ogre::Light::LightTypes>(lightType);
		object->updateRenderInstance();
	}
};

template <Ogre::Vector3 VLightObject::*PtrToMember>
class VLightVector3MemberCommand : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VLightObject *object = static_cast<const VLightObject*>(source);
		return object->*PtrToMember;
	}

	VString doGetAsString(const void *source) const
	{
		const VLightObject *object = static_cast<const VLightObject*>(source);
		return Ogre::StringConverter::toString(object->*PtrToMember);
	}

	void doSet(void *target, const Variant &value)
	{
		VLightObject *object = static_cast<VLightObject*>(target);
		object->*PtrToMember = VariantCast<Ogre::Vector3>(value);
		object->updateRenderInstance();
	}

	void doSetAsString(void *target, const VString &value)
	{
		VLightObject *object = static_cast<VLightObject*>(target);
		object->*PtrToMember = Ogre::StringConverter::parseVector3(value);
		object->updateRenderInstance();
	}
};

template <Ogre::ColourValue VLightObject::*PtrToMember>
class VLightColorRGBMemberCommand : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VLightObject *object = static_cast<const VLightObject*>(source);
		return object->*PtrToMember;
	}

	VString doGetAsString(const void *source) const
	{
		const VLightObject* object = static_cast<const VLightObject*>(source);
		return ColourRGB2String(object->*PtrToMember);
	}

	void doSet(void *target, const Variant &value)
	{
		VLightObject *object = static_cast<VLightObject*>(target);
		object->*PtrToMember = VariantCast<Ogre::ColourValue>(value);
		object->updateRenderInstance();
	}

	void doSetAsString(void *target, const VString &value)
	{
		VLightObject *object = static_cast<VLightObject*>(target);
		object->*PtrToMember = String2ColourRGB(value);
		object->updateRenderInstance();
	}
};


/**************************************************************************************************
 *									class VLightObject
 *************************************************************************************************/

VLightObject::VLightObject()
	: VSceneObject()
	, mBaseSceneNode(VNULL)
	, mSceneNode(VNULL)
	, mLight(VNULL)
	, mLightType(Ogre::Light::LT_POINT)
	, mDiffuse(Ogre::ColourValue::White)
	, mSpecular(Ogre::ColourValue::Black)
	, mPosition(Ogre::Vector3::ZERO)
	, mDirection(Ogre::Vector3::UNIT_Z)
	, mRange(100.0f)
	, mAttenuationConstant(1.0f)
	, mAttenuationLinear(0.0f)
	, mAttenuationQuadric(0.0f)
	, mSpotOuterAngle(40.0f)
	, mSpotInnerAngle(30.0f)
	, mSpotFalloff(1.0f)
	, mCastShadows(VFALSE)
	, mStaticLight(VFALSE)
{
	// TODO: Add properties definition.

	typedef VLightColorRGBMemberCommand<&VLightObject::mDiffuse> VDiffuseCmd;
	typedef VLightColorRGBMemberCommand<&VLightObject::mSpecular> VSpecularCmd;

	typedef VLightVector3MemberCommand<&VLightObject::mPosition> VPositionCmd;
	typedef VLightVector3MemberCommand<&VLightObject::mDirection> VDirectionCmd;

	typedef VObjectMemberPropertyCommand<VLightObject, Ogre::Real, &VLightObject::mRange> VRangeCmd;
	typedef VObjectMemberPropertyCommand<VLightObject, Ogre::Real, &VLightObject::mAttenuationConstant> VAttenuationConstantCmd;
	typedef VObjectMemberPropertyCommand<VLightObject, Ogre::Real, &VLightObject::mAttenuationLinear> VAttenuationLinearCmd;
	typedef VObjectMemberPropertyCommand<VLightObject, Ogre::Real, &VLightObject::mAttenuationQuadric> VAttenuationQuadricCmd;
	typedef VObjectMemberPropertyCommand<VLightObject, Ogre::Real, &VLightObject::mSpotOuterAngle> VSpotOuterAngleCmd;
	typedef VObjectMemberPropertyCommand<VLightObject, Ogre::Real, &VLightObject::mSpotInnerAngle> VSpotInnerAngleCmd;
	typedef VObjectMemberPropertyCommand<VLightObject, Ogre::Real, &VLightObject::mSpotFalloff> VSpotFalloffCmd;
	typedef VObjectMemberPropertyCommand<VLightObject, VBOOL, &VLightObject::mCastShadows> VCastShadowsCmd;
	typedef VObjectMemberPropertyCommand<VLightObject, VBOOL, &VLightObject::mStaticLight> VStaticLightCmd;

	if (createPropertyDictionary("Light"))
	{
		static VLightTypeCmd lightTypeCmd;
		defineProperty(
			"type",
			"The type of light.",
			getEnumTypeString("LightType", gsLightTypeConstants),
			&lightTypeCmd,
			0);

		static VDiffuseCmd diffuseCmd;
		defineProperty(
			"diffuse",
			"The color of the diffuse light given off by this light.",
			"ColorRGB",
			&diffuseCmd,
			0);

		static VSpecularCmd specularCmd;
		defineProperty(
			"specular",
			"The color of the specular light given off by this light.",
			"ColorRGB",
			&specularCmd,
			0);

		static VPositionCmd positionCmd;
		defineProperty(
			"position",
			"The position of the light.\n"
			"Applicable to point lights and spotlights only.",
			"Position",
			&positionCmd,
			0);

		static VDirectionCmd directionCmd;
		defineProperty(
			"direction",
			"The direction of the light.\n"
			"Applicable to directional lights and spotlights only.",
			"Direction",
			&directionCmd,
			0);

		static VRangeCmd rangeCmd;
		defineProperty(
			"range",
			"The absolute upper range of the light.\n"
			"Applicable to point lights and spotlights only.",
			"Ogre::Real",
			&rangeCmd,
			0);

		static VAttenuationConstantCmd attenuationConstantCmd;
		defineProperty(
			"attenuation.constant",
			"The constant factor in the attenuation formula.\n"
			"Applicable to point lights and spotlights only.",
			"Ogre::Real",
			&attenuationConstantCmd,
			0);

		static VAttenuationLinearCmd attenuationLinearCmd;
		defineProperty(
			"attenuation.linear",
			"The linear factor in the attenuation formula.\n"
			"Applicable to point lights and spotlights only.",
			"Ogre::Real",
			&attenuationLinearCmd,
			0);

		static VAttenuationQuadricCmd attenuationQuadricCmd;
		defineProperty(
			"attenuation.quadric",
			"The quadric factor in the attenuation formula.\n"
			"Applicable to point lights and spotlights only.",
			"Ogre::Real",
			&attenuationQuadricCmd,
			0);

		static VSpotOuterAngleCmd spotOuterAngleCmd;
		defineProperty(
			"spotlight.outer",
			"The angle covered by the spotlights outer cone.\n"
			"Applicable to spotlights only.",
			"Ogre::Real",
			&spotOuterAngleCmd,
			0);

		static VSpotInnerAngleCmd spotInnerAngleCmd;
		defineProperty(
			"spotlight.inner",
			"The angle covered by the spotlights inner cone.\n"
			"Applicable to spotlights only.",
			"Ogre::Real",
			&spotInnerAngleCmd,
			0);

		static VSpotFalloffCmd spotFalloffCmd;
		defineProperty(
			"spotlight.falloff",
			"The falloff between the inner and outer cones of the spotlight.\n"
			"Applicable to spotlights only.",
			"Ogre::Real",
			&spotFalloffCmd,
			0);

		static VCastShadowsCmd castShadowsCmd;
		defineProperty(
			"cast shadows",
			"Does this light cast shadows?",
			"Bool",
			&castShadowsCmd,
			0);

		static VStaticLightCmd staticLightCmd;
		defineProperty(
			"static light",
			"This light is use as static, which will be use for calculate lightmap only.",
			"Bool",
			&staticLightCmd,
			0);
	}
}

VLightObject::~VLightObject()
{
	destroyRenderInstance();
}

const VString &VLightObject::getType() const
{
	return msType;
}

const VString &VLightObject::getCategory() const
{
	return msCategory;
}

void VLightObject::createRenderInstance()
{
	assert(VNULL == mBaseSceneNode);

	VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();
	mBaseSceneNode = pGfxSystem->getBaseSceneNode();

	updateRenderInstance();
}

void VLightObject::destroyRenderInstance()
{
	_destroyLight();
	mBaseSceneNode = VNULL;
}

void VLightObject::updateRenderInstance()
{
	if (mBaseSceneNode)
	{
		if (mStaticLight)
		{
			_destroyLight();
		}
		else
		{
			if (VNULL == mLight)
			{
				_createLight();
			}

			_updateRenderInstanceImp();
		}
	}
}

void VLightObject::_createLight()
{
	assert(VNULL == mLight);
	assert(VNULL == mSceneNode);

	mSceneNode = mBaseSceneNode->createChildSceneNode();
	mLight = mSceneNode->getCreator()->createLight(mSceneNode->getName());
	mSceneNode->attachObject(mLight);
}

void VLightObject::_destroyLight()
{
	if (mSceneNode)
	{
		if (mLight)
		{
			mSceneNode->getCreator()->destroyLight(mLight);
			mLight = VNULL;
		}

		mSceneNode->getCreator()->destroySceneNode(mSceneNode);
		mSceneNode = VNULL;
	}
}

void VLightObject::_updateRenderInstanceImp()
{
	assert(mLight);

	mLight->setType(mLightType);
	mLight->setDiffuseColour(mDiffuse);
	mLight->setSpecularColour(mSpecular);

	if (Ogre::Light::LT_DIRECTIONAL != mLightType)
	{
		mLight->setPosition(mPosition);
		mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
	}

	if (Ogre::Light::LT_POINT != mLightType)
	{
		mLight->setDirection(mDirection.normalisedCopy());
	}

	if (Ogre::Light::LT_SPOTLIGHT == mLightType)
	{
		mLight->setSpotlightRange(Ogre::Degree(mSpotInnerAngle), Ogre::Degree(mSpotOuterAngle), mSpotFalloff);
	}

	mLight->setCastShadows(mCastShadows);
}

VSceneObjectFactory *VLightObject::getFactory()
{
	static VLightObjectFactory factory;
	return &factory;
}

