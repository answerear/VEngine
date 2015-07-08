

#include "VEffectObject.h"
#include "VEffect.h"
#include "VEffectManager.h"
#include "VPropertyHelper.h"


const VString VEffectObject::msType = "Effect";
const VString VEffectObject::msCategory = "StaticEntity";


/**************************************************************************************************
 *									class VEffectObjectFactory
 *************************************************************************************************/

class VEffectObjectFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VEffectObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VEffectObject();
	}
};


/**************************************************************************************************
 *								Property Command of VEffectObject
 *************************************************************************************************/
class VEffectNameCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VEffectObject *object = static_cast<const VEffectObject*>(source);
		return object->getEffectName();
	}

	VString doGetAsString(const void *source) const
	{
		const VEffectObject *object = static_cast<const VEffectObject*>(source);
		return object->getEffectName();
	}

	void doSet(void *target, const Variant &value)
	{
		VEffectObject *object = static_cast<VEffectObject*>(target);
		VString effectName = VariantCast<VString>(value);
		object->setEffectName(effectName);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VEffectObject *object = static_cast<VEffectObject*>(target);
		object->setEffectName(value);
	}
};

/**************************************************************************************************
 *									class VEffectObject
 *************************************************************************************************/

VEffectObject::VEffectObject()
	: VPlacementObject()
	, mSceneMgr(VNULL)
	, mEffect(VNULL)
	, mEffectName("")
{
	// TODO: Add properties definition.
	if (createPropertyDictionary("Effect"))
	{
		static VEffectNameCmd effectNameCmd;
		defineProperty(
			"effect name",
			"The effect template name use to create this effect object.",
			"effect template name",
			&effectNameCmd,
			PF_PRIMARY);

		_addBaseProperty();
	}
}

VEffectObject::~VEffectObject()
{
	destroyRenderInstance();
}

const VString &VEffectObject::getType() const
{
	return msType;
}

const VString &VEffectObject::getCategory() const
{
	return msCategory;
}

void VEffectObject::createRenderInstance()
{
	VPlacementObject::createRenderInstance();

	if (!mEffectName.empty())
	{
		_createEffect();
	}
}

void VEffectObject::destroyRenderInstance()
{
	if (mEffect != VNULL)
	{
		_destroyEffect();
	}

	VPlacementObject::destroyRenderInstance();
}

void VEffectObject::setEffectName(const VString &name)
{
	if (mEffectName != name)
	{
		if (mEffect != VNULL)
		{
			_destroyEffect();
		}

		mEffectName = name;

		if (!mEffectName.empty())
		{
			_createEffect();
		}
	}
}

void VEffectObject::_createEffect()
{
	assert(mEffect == VNULL);
	assert(!mEffectName.empty());

	Ogre::SceneNode* parent = getSceneNode();

	if (parent)
	{
		mEffect = VEFFECT_MANAGER.createEffect(mEffectName);
		assert(mEffect);

		VTransforms tempTransformInfos;
		VTransform tempTransformInfo(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, Ogre::Vector3::UNIT_SCALE);
		tempTransformInfos.push_back(tempTransformInfo);
		mEffect->setExtraTransforms(tempTransformInfos);

		// the scene node of placement object will be the parent scene node of
		// effect object, so the position of the scene node will be the position of
		// the effect.
		mEffect->createSceneNode(parent);	
	}
}

void VEffectObject::_destroyEffect()
{
	assert(mEffect != VNULL);
	VEFFECT_MANAGER.destroyEffect(mEffect);
	mEffect = VNULL;
}

VSceneObjectFactory *VEffectObject::getFactory()
{
	static VEffectObjectFactory factory;
	return &factory;
}

