

#include "VModelObject.h"
#include "VGraphicsSystem.h"
#include "VEngine.h"
#include "VResource.h"


using namespace Vision;


const VString VModelObject::msType = "Model";
const VString VModelObject::msCategory = "Model";


/**************************************************************************************************
 *									class VEffectObjectFactory
 *************************************************************************************************/

class VModelObjectFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VModelObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VModelObject();
	}
};


/**************************************************************************************************
 *								Property Command of VModelObject
 *************************************************************************************************/
class VModelNameCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VModelObject *object = static_cast<const VModelObject*>(source);
		return object->getModelName();
	}

	VString doGetAsString(const void *source) const
	{
		const VModelObject *object = static_cast<const VModelObject*>(source);
		return object->getModelName();
	}

	void doSet(void *target, const Variant &value)
	{
		VModelObject *object = static_cast<VModelObject*>(target);
		VString modelName = VariantCast<VString>(value);
		object->setModelName(modelName);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VModelObject *object = static_cast<VModelObject*>(target);
		object->setModelName(value);
	}
};

class VModelAnimationLevelCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VModelObject *object = static_cast<const VModelObject*>(source);
		return object->getAnimationLevel();
	}

	VString doGetAsString(const void *source) const
	{
		const VModelObject *object = static_cast<const VModelObject*>(source);
		return Ogre::StringConverter::toString(object->getAnimationLevel());
	}

	void doSet(void *target, const Variant &value)
	{
		VModelObject *object = static_cast<VModelObject*>(target);
		Ogre::Real animationLevel = VariantCast<Ogre::Real>(value);
		object->setAnimationLevel(animationLevel);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VModelObject *object = static_cast<VModelObject*>(target);
		Ogre::Real animationLevel = Ogre::StringConverter::parseReal(value);
		object->setAnimationLevel(animationLevel);
	}
};

class VModelCastShadowsCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VModelObject *object = static_cast<const VModelObject*>(source);
		return object->getCastShadows();
	}

	VString doGetAsString(const void *source) const
	{
		const VModelObject *object = static_cast<const VModelObject*>(source);
		return Ogre::StringConverter::toString(object->getCastShadows());
	}

	void doSet(void *target, const Variant &value)
	{
		VModelObject *object = static_cast<VModelObject*>(target);
		VBOOL castShadows = VariantCast<VBOOL>(value);
		object->setCastShadows(castShadows);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VModelObject *object = static_cast<VModelObject*>(target);
		VBOOL castShadows = Ogre::StringConverter::parseBool(value);
		object->setCastShadows(castShadows);
	}
};


/**************************************************************************************************
 *									class VModelObject
 *************************************************************************************************/

VModelObject::VModelObject()
	: VPlacementObject()
	, mSceneMgr(VNULL)
	, mModel(VNULL)
	, mModelName()
	, mAnimationLevel(50)
	, mCastShadows(VFALSE)
{
	// TODO: Add properties definition.
	if (createPropertyDictionary("Model"))
	{
		static VModelNameCmd modelNameCmd;
		defineProperty(
			"model name",
			"The model filename use to create this model.",
			"ModelName",
			&modelNameCmd,
			PF_PRIMARY);

		_addBaseProperty();

		static VModelAnimationLevelCmd animationLevelCmd;
		defineProperty(
			"animation level",
			"The animation level of this object.",
			"Ogre::Real",
			&animationLevelCmd,
			0);

		static VModelCastShadowsCmd castShadowsCmd;
		defineProperty(
			"cast shadows",
			"Does this object cast shadows?",
			"Bool",
			&castShadowsCmd,
			0);
	}
}

VModelObject::~VModelObject()
{
	destroyRenderInstance();
}

const VString &VModelObject::getType() const
{
	return msType;
}

const VString &VModelObject::getCategory() const
{
	return msCategory;
}

void VModelObject::createRenderInstance()
{
	assert(mModel == VNULL);

	VPlacementObject::createRenderInstance();

	if (!mModelName.empty())
	{
		_createModel();
	}
}

void VModelObject::destroyRenderInstance()
{
	if (mModel)
	{
		_destroyModel();
	}

	VPlacementObject::destroyRenderInstance();
}

void VModelObject::setModelName(const VString &modelName)
{
	if (mModelName != modelName)
	{
		if (mModel)
		{
			_destroyModel();
		}

		mModelName = modelName;

		if (getSceneNode() && !mModelName.empty())
		{
			_createModel();
		}
	}
}

void VModelObject::setAnimationLevel(Ogre::Real level)
{
	mAnimationLevel = level;
	if (mModel)
	{
		VGraphicsSystem *system = VENGINE.getGfxSystem();
		mModel->setAnimationType(system->determineAnimationType(mAnimationLevel));
	}
}

void VModelObject::setCastShadows(VBOOL castShadows)
{
	mCastShadows = castShadows;
	if (mModel)
	{
		mModel->setCastShadows(mCastShadows);
	}
}

void VModelObject::_createModel()
{
	assert(!mModel);
	assert(!mModelName.empty());

	Ogre::SceneNode* parent = getSceneNode();
	assert(parent);

	mModel = new VModel(parent, mModelName, DEFAULT_RESOURCE_GROUP_NAME);

	VGraphicsSystem *system = VENGINE.getGfxSystem();
	mModel->setAnimationType(system->determineAnimationType(mAnimationLevel));
	mModel->setCastShadows(mCastShadows);
}

void VModelObject::_destroyModel()
{
	assert(mModel);

	delete mModel;
	mModel = NULL;
}

VSceneObjectFactory *VModelObject::getFactory()
{
	static VModelObjectFactory factory;
	return &factory;
}

