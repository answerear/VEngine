

#include "VEnvironmentObject.h"
#include "VEngine.h"
#include "VPropertyHelper.h"
#include "VGraphicsSystem.h"


const VString VEnvironmentObject::msType = "Enviroment";
const VString VEnvironmentObject::msCategory = "Enviroment";


/**************************************************************************************************
 *									class VEnvironmentObjectFactory
 *************************************************************************************************/

class VEnvironmentObjectFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VEnvironmentObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VEnvironmentObject();
	}
};


/**************************************************************************************************
 *								Property Command of VEnvironmentObject
 *************************************************************************************************/

static const VConstant gsFogModeConstants[] =
{
	{"exp",    Ogre::FOG_EXP},
	{"exp2",   Ogre::FOG_EXP2},
    {"linear", Ogre::FOG_LINEAR},
    {"none",   Ogre::FOG_NONE},
    {VNULL, }
};

class VFogModeCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VEnvironmentObject *object = static_cast<const VEnvironmentObject*>(source);
		return object->mFogMode;
	}

	VString doGetAsString(const void *source) const
	{
		const VEnvironmentObject *object = static_cast<const VEnvironmentObject*>(source);
		VString result;
		if (!mapEnumLabel(gsFogModeConstants, object->mFogMode, result))
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
				"Unknown fog mode: " + Ogre::StringConverter::toString(object->mFogMode),
				"FogModeCmd::doGet");
		}

		return result;
	}

	void doSet(void *target, const Variant &value)
	{
		VEnvironmentObject* object = static_cast<VEnvironmentObject*>(target);
		object->mFogMode = VariantCast<Ogre::FogMode>(value);
		object->updateRenderInstance();
	}

	void doSetAsString(void *target, const VString &value)
	{
		VEnvironmentObject* object = static_cast<VEnvironmentObject*>(target);
		int fogMode;
		if (!mapEnumValue(gsFogModeConstants, value, fogMode, false))
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
				"Unknown fog mode: " + value,
				"FogModeCmd::doSet");
		}

		object->mFogMode = static_cast<Ogre::FogMode>(fogMode);
		object->updateRenderInstance();
	}
};


template <ColorValue VEnvironmentObject::*PtrToMember>
class VEnvColorRGBMemberCommand : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VEnvironmentObject *object = static_cast<const VEnvironmentObject*>(source);
		return object->*PtrToMember;
	}

	VString doGetAsString(const void *source) const
	{
		const VEnvironmentObject *object = static_cast<const VEnvironmentObject*>(source);
		return ColourRGB2String(object->*PtrToMember);
	}

	void doSet(void *target, const Variant &value)
	{
		VEnvironmentObject *object = static_cast<VEnvironmentObject*>(target);
		object->*PtrToMember = VariantCast<ColorValue>(value);
		object->updateRenderInstance();
	}

	void doSetAsString(void *target, const VString &value)
	{
		VEnvironmentObject *object = static_cast<VEnvironmentObject*>(target);
		object->*PtrToMember = String2ColourRGB(value);
		object->updateRenderInstance();
	}
};


class VRenderWeatherCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VEnvironmentObject *object = static_cast<const VEnvironmentObject*>(source);
		return object->mRenderWeather;
	}

	VString doGetAsString(const void *source) const
	{
		const VEnvironmentObject *object = static_cast<const VEnvironmentObject*>(source);
		return Ogre::StringConverter::toString(object->mRenderWeather);
	}

	void doSet(void *target, const Variant &value)
	{
		VEnvironmentObject *object = static_cast<VEnvironmentObject*>(target);
		object->enableRenderWeather(VariantCast<bool>(value));
	}

	void doSetAsString(void *target, const VString &value)
	{
		VEnvironmentObject *object = static_cast<VEnvironmentObject*>(target);

		object->enableRenderWeather(Ogre::StringConverter::parseBool(value));
	}
};


/**************************************************************************************************
 *									class VEnvironmentObject
 *************************************************************************************************/

VEnvironmentObject::VEnvironmentObject()
	: VSceneObject()
	, mSceneMgr(VNULL)
	, mViewport(VNULL)
	, mWeatherParticleSystemName("")
	, mAmbientLight(Ogre::ColourValue::Black)
	, mBackgroundColor(Ogre::ColourValue::Black)
	, mFogColor(Ogre::ColourValue::White)
	, mFogMode(Ogre::FOG_NONE)
	, mFogExpDensity(0.001f)
	, mFogLinearStart(0.0f)
	, mFogLinearEnd(1.0f)
	, mRenderWeather(VFALSE)
{
	typedef VEnvColorRGBMemberCommand<&VEnvironmentObject::mAmbientLight> VAmbientLightCmd;
	typedef VEnvColorRGBMemberCommand<&VEnvironmentObject::mBackgroundColor> VBackgroundColorCmd;
	typedef VEnvColorRGBMemberCommand<&VEnvironmentObject::mFogColor> VFogColorCmd;

	typedef VObjectMemberPropertyCommand<VEnvironmentObject,Ogre::Real,&VEnvironmentObject::mFogExpDensity> VFogExpDensityCmd;    
	typedef VObjectMemberPropertyCommand<VEnvironmentObject,Ogre::Real,&VEnvironmentObject::mFogLinearStart> VFogLinearStartCmd;
	typedef VObjectMemberPropertyCommand<VEnvironmentObject,Ogre::Real,&VEnvironmentObject::mFogLinearEnd> VFogLinearEndCmd;

	typedef VObjectMemberPropertyCommand<VEnvironmentObject,VString,&VEnvironmentObject::mWeatherParticleSystemName> VWeatherParticleSystemNameCmd;

	if (createPropertyDictionary("Environment"))
	{
		static VAmbientLightCmd ambientColorCmd;
		defineProperty(
			"ambient",
			"The ambient light level to be used for the scene",
			"ColorRGB",
			&ambientColorCmd,
			0);

		static VBackgroundColorCmd backgroundColorCmd;
		defineProperty(
			"background",
			"The background color of the scene",
			"ColorRGB",
			&backgroundColorCmd,
			0);

		static VFogModeCmd fogModeCmd;
		defineProperty(
			"fog.mode",
			"The fog mode",
			getEnumTypeString("FogMode", gsFogModeConstants),
			&fogModeCmd,
			0);

		static VFogColorCmd fogColorCmd;
		defineProperty(
			"fog.colour",
			"The color of the fog.",
			"ColorRGB",
			&fogColorCmd,
			0);     

		static VFogExpDensityCmd expDensityCmd;
		defineProperty(
			"fog.exp density",
			"The density of the fog in exp or exp2 mode.",
			"Real",
			&expDensityCmd,
			0);    

		static VFogLinearStartCmd linearStartCmd;
		defineProperty(
			"fog.linear start",
			"Distance in world units at which linear fog starts to encroach.",
			"Real",
			&linearStartCmd,
			0);    

		static VFogLinearEndCmd linearEndCmd;
		defineProperty(
			"fog.linear end",
			"Distance in world units at which linear fog becomes completely opaque.",
			"Real",
			&linearEndCmd,
			0);    

		static VRenderWeatherCmd renderWeatherCmd;
		defineProperty(
			"weather.render weather",
			"if this option is true, the weather particle system will be rendered.",
			"Bool",
			&renderWeatherCmd,
			0);

		static VWeatherParticleSystemNameCmd weatherCmd;
		defineProperty(
			"weather.particle system name",
			"The name of the particle system that used for scene weather.",
			"String",
			&weatherCmd,
			0);
	}
}

VEnvironmentObject::~VEnvironmentObject()
{
	destroyRenderInstance();
}

const VString &VEnvironmentObject::getType() const
{
	return msType;
}

const VString &VEnvironmentObject::getCategory() const
{
	return msCategory;
}



void VEnvironmentObject::createRenderInstance()
{
	VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();

	if (VNULL == mSceneMgr)
	{
		mSceneMgr = pGfxSystem->getSceneManager();
	}

	if (VNULL == mViewport)
	{
		mViewport = pGfxSystem->getViewport();
	}

	_updateRenderInstanceImp();
}

void VEnvironmentObject::destroyRenderInstance()
{
	if (mSceneMgr)
	{
		mSceneMgr->setFog(Ogre::FOG_NONE);
		mSceneMgr->setAmbientLight(Ogre::ColourValue::Black);
		mSceneMgr = VNULL;
	}

	if (mViewport)
	{
		mViewport->setBackgroundColour(Ogre::ColourValue::Black);
		mViewport = VNULL;
	}
}

void VEnvironmentObject::updateRenderInstance()
{
	if (mSceneMgr && mViewport)
	{
		_updateRenderInstanceImp();
	}
}

void VEnvironmentObject::enableRenderWeather(VBOOL renderWeather)
{
	mRenderWeather = renderWeather;
}

void VEnvironmentObject::_updateRenderInstanceImp()
{
	assert(mSceneMgr);
	assert(mViewport);

	mSceneMgr->setAmbientLight(mAmbientLight);
	mSceneMgr->setFog(mFogMode, mFogColor, mFogExpDensity, mFogLinearStart, mFogLinearEnd);
	mViewport->setBackgroundColour(mFogMode == Ogre::FOG_NONE ? mBackgroundColor : mFogColor);
}

VSceneObjectFactory *VEnvironmentObject::getFactory()
{
	static VEnvironmentObjectFactory factory;
	return &factory;
}
