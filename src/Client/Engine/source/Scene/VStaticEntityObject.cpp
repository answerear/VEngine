
#include "VStaticEntityObject.h"
#include "VisionUtils.h"
#include "VResource.h"
#include "OgreAutoAnimationEntity.h"


using namespace Vision;


const VString VStaticEntityObject::msType = "StaticEntity";
const VString VStaticEntityObject::msCategory = "StaticObject";


/**************************************************************************************************
 *									class VStaticEntityObjectFactory
 *************************************************************************************************/

class VStaticEntityObjectFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VStaticEntityObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VStaticEntityObject();
	}
};


/**************************************************************************************************
 *								Property Command of VStaticEntityObject
 *************************************************************************************************/

class VEntityMeshNameCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return object->getMeshName();
	}

	VString doGetAsString(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return object->getMeshName();
	}

	void doSet(void *target, const Variant &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		Ogre::String meshName = VariantCast<VString>(value);
		object->setMeshName(meshName);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		object->setMeshName(value);
	}
};

class VEntityAnimationLevelCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return object->getAnimationLevel();
	}

	VString doGetAsString(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return Ogre::StringConverter::toString(object->getAnimationLevel());
	}

	void doSet(void *target, const Variant &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		Ogre::Real animationLevel = VariantCast<Ogre::Real>(value);
		object->setAnimationLevel(animationLevel);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		Ogre::Real animationLevel = Ogre::StringConverter::parseReal(value);
		object->setAnimationLevel(animationLevel);
	}
};

class VEntityTransparencyCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return object->getTransparency();
	}

	VString doGetAsString(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return Ogre::StringConverter::toString(object->getTransparency());
	}

	void doSet(void *target, const Variant &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		Ogre::Real transparency = VariantCast<Ogre::Real>(value);
		object->setTransparency(transparency);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		Ogre::Real transparency = Ogre::StringConverter::parseReal(value);
		object->setTransparency(transparency);
	}
};

class VEntityCastShadowsCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return object->getCastShadows();
	}

	VString doGetAsString(const void *source) const
	{
		const VStaticEntityObject* object = static_cast<const VStaticEntityObject*>(source);
		return Ogre::StringConverter::toString(object->getCastShadows());
	}

	void doSet(void *target, const Variant &value)
	{
		VStaticEntityObject* object = static_cast<VStaticEntityObject*>(target);
		VBOOL castShadows = VariantCast<VBOOL>(value);
		object->setCastShadows(castShadows);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		VBOOL castShadows = Ogre::StringConverter::parseBool(value);
		object->setCastShadows(castShadows);
	}
};

class VEntityReceiveShadowsCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return object->getReceiveShadows();
	}

	VString doGetAsString(const void *source) const
	{
		const VStaticEntityObject* object = static_cast<const VStaticEntityObject*>(source);
		return Ogre::StringConverter::toString(object->getReceiveShadows());
	}

	void doSet(void *target, const Variant &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		bool castShadows = VariantCast<bool>(value);
		object->setReceiveShadows(castShadows);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		bool castShadows = Ogre::StringConverter::parseBool(value);
		object->setReceiveShadows(castShadows);
	}
};

class VEntityReceiveDecalsCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return object->getReceiveDecals();
	}

	VString doGetAsString(const void *source) const
	{
		const VStaticEntityObject *object = static_cast<const VStaticEntityObject*>(source);
		return Ogre::StringConverter::toString(object->getReceiveDecals());
	}

	void doSet(void *target, const Variant &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		VBOOL receiveDecals = VariantCast<VBOOL>(value);
		object->setReceiveDecals(receiveDecals);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VStaticEntityObject *object = static_cast<VStaticEntityObject*>(target);
		VBOOL receiveDecals = Ogre::StringConverter::parseBool(value);
		object->setReceiveDecals(receiveDecals);
	}
};

/**************************************************************************************************
 *									class VStaticEntityObject
 *************************************************************************************************/

VStaticEntityObject::VStaticEntityObject()
	: VPlacementObject()
	, mEntity(VNULL)
	, mMeshName()
	, mAnimationLevel(50)
	, mTransparency()
	, mCastShadows(VFALSE)
	, mReceiveShadows(VFALSE)
	, mReceiveDecals(VFALSE)
{
	// TODO: Add properties definition.
	if (createPropertyDictionary("StaticEntity"))
	{
		static VEntityMeshNameCmd meshNameCmd;
		defineProperty(
			"mesh name",
			"The mesh filename use to create this static entity.",
			"MeshName",
			&meshNameCmd,
			PF_PRIMARY);

		_addBaseProperty();

		static VEntityAnimationLevelCmd animationLevelCmd;
		defineProperty(
			"animation level",
			"The animation level of this object.",
			"Ogre::Real",
			&animationLevelCmd,
			0);

		static VEntityTransparencyCmd transparencyCmd;
		defineProperty(
			"transparency",
			"Amount transparent of this object.",
			"Ogre::Real",
			&transparencyCmd,
			0);

		static VEntityCastShadowsCmd castShadowsCmd;
		defineProperty(
			"cast shadows",
			"Does this object cast shadows?",
			"Bool",
			&castShadowsCmd,
			0);

		static VEntityReceiveShadowsCmd receiveShadowsCmd;
		defineProperty(
			"receive shadows",
			"Does this object receive shadows?",
			"Bool",
			&receiveShadowsCmd,
			0);

		static VEntityReceiveDecalsCmd receiveDecalsCmd;
		defineProperty(
			"receive decals",
			"Does this object receive decals?",
			"Bool",
			&receiveDecalsCmd,
			0);
	}
}

VStaticEntityObject::~VStaticEntityObject()
{
	destroyRenderInstance();
}

const VString &VStaticEntityObject::getType() const
{
	return msType;
}

const VString &VStaticEntityObject::getCategory() const
{
	return msCategory;
}

void VStaticEntityObject::createRenderInstance()
{
	assert(!mEntity);

	VPlacementObject::createRenderInstance();
	
	if (!mMeshName.empty())
	{
		_createEntity();
	}
}

void VStaticEntityObject::destroyRenderInstance()
{
	if (mEntity)
	{
		_destroyEntity();
	}
	
	VPlacementObject::destroyRenderInstance();
}

void VStaticEntityObject::queryBakeableEntities(VEntityList &entities)
{
	assert(mEntity);

	if (!getAnimationStateSafely(mEntity, AUTO_ANIMATION_NAME))
	{
		entities.push_back(mEntity);
	}
}

void VStaticEntityObject::bakeStaticGeometry(Ogre::StaticGeometry *staticGeometry, const VEntityList &entities)
{
	assert(mEntity);
	assert(!entities.empty());
	assert(entities.front() == mEntity);
	assert(!getAnimationStateSafely(mEntity, AUTO_ANIMATION_NAME));

	staticGeometry->addSceneNode(mEntity->getParentSceneNode());
	destroyRenderInstance();
}

void VStaticEntityObject::setMeshName(const VString &meshName)
{
	if (mMeshName != meshName)
	{
		if (mEntity)
		{
			_destroyEntity();
		}

		mMeshName = meshName;

		if (getSceneNode() && !mMeshName.empty())
		{
			_createEntity();
		}
	}
}

void VStaticEntityObject::setAnimationLevel(Real level)
{
	mAnimationLevel = level;
}

void VStaticEntityObject::setTransparency(Real transparency)
{
	if (transparency < 0)
		transparency = 0;
	else if (transparency > 1)
		transparency = 1;

	mTransparency = transparency;
}

void VStaticEntityObject::setCastShadows(VBOOL castShadows)
{
	if (mCastShadows != castShadows)
	{
		mCastShadows = castShadows;

		if (mEntity)
		{
			mEntity->setCastShadows(castShadows);
		}
	}
}

void VStaticEntityObject::setReceiveShadows(VBOOL receiveShadows)
{
	if (mReceiveShadows != receiveShadows)
	{
		mReceiveShadows = receiveShadows;

		if (mEntity)
		{
			if (receiveShadows)
			{
				mEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
			}
			else
			{
				mEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAIN);
			}
		}
	}
}

void VStaticEntityObject::setReceiveDecals(VBOOL receiveDecals)
{
	mReceiveDecals = receiveDecals;

	if (mEntity)
	{
		if (receiveDecals)
		{
// 			mEntity->addQueryFlags(OQF_DECAL_TARGET);
		}
		else
		{
// 			mEntity->removeQueryFlags(OQF_DECAL_TARGET);
		}
	}
}

void VStaticEntityObject::_createEntity()
{
	Ogre::SceneNode *parent = getSceneNode();
	assert(parent);

	Ogre::SceneManager *creator = parent->getCreator();
	assert(creator);

	Ogre::MeshPtr mesh = loadMesh(mMeshName, Ogre::StringUtil::BLANK, Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);

	if (getAnimationSafely(mesh, AUTO_ANIMATION_NAME))
	{
		Ogre::NameValuePairList params;
		params["mesh"] = mesh->getName();
		Ogre::AutoAnimationEntity* autoAnimationEntity = static_cast<Ogre::AutoAnimationEntity*>(
			creator->createMovableObject(parent->getName(), Ogre::AutoAnimationEntityFactory::FACTORY_TYPE_NAME, &params));
		mEntity = autoAnimationEntity;
	}
	else
	{
		mEntity = creator->createEntity(parent->getName(), mesh->getName());
	}

	parent->attachObject(mEntity);
	mEntity->setCastShadows(mCastShadows);
	if (mReceiveDecals)
	{
// 		mEntity->addQueryFlags(OQF_DECAL_TARGET);
	}
}

void VStaticEntityObject::_destroyEntity()
{
	Ogre::SceneNode* parent = getSceneNode();
	parent->getCreator()->destroyEntity(mEntity);
	mEntity = VNULL;
}

VSceneObjectFactory *VStaticEntityObject::getFactory()
{
	static VStaticEntityObjectFactory factory;
	return &factory;
}


