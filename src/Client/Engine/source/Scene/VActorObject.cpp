

#include "VActorObject.h"
#include "VActor.h"


const VString VActorObject::msType = "Actor";
const VString VActorObject::msCategory = "Actor";


/**************************************************************************************************
 *									class VActorObjectFactory
 *************************************************************************************************/

class VActorObjectFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VActorObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VActorObject();
	}
};


/**************************************************************************************************
 *								Property Command of VActorObject
 *************************************************************************************************/

class VActorNameCmd : public VPropertyCommand
{
	Variant doGet(const void *source) const
	{
		const VActorObject *object = static_cast<const VActorObject*>(source);
		return object->getActorName();
	}

	VString doGetAsString(const void *source) const
	{
		const VActorObject *object = static_cast<const VActorObject*>(source);
		return object->getActorName();
	}

	void doSet(void *target, const Variant &value)
	{
		VActorObject *object = static_cast<VActorObject*>(target);
		VString actorName = VariantCast<VString>(value);
		object->setActorName(actorName);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VActorObject *object = static_cast<VActorObject*>(target);
		object->setActorName(value);
	}
};

/**************************************************************************************************
 *									class VActorObject
 *************************************************************************************************/

VActorObject::VActorObject()
	: VPlacementObject()
	, mActor(VNULL)
	, mActorName()
{
	// TODO: Add properties definition.
	if (createPropertyDictionary("Actor"))
	{
		static VActorNameCmd actorNameCmd;
		defineProperty(
			"actor name",
			"The actor filename use to create this actor.",
			"ActorName",
			&actorNameCmd,
			PF_PRIMARY);

		_addBaseProperty();
	}
}

VActorObject::~VActorObject()
{
	destroyRenderInstance();
}

const VString &VActorObject::getType() const
{
	return msType;
}

const VString &VActorObject::getCategory() const
{
	return msCategory;
}

void VActorObject::createRenderInstance()
{
	VPlacementObject::createRenderInstance();

	if (!mActorName.empty())
	{
		_createActor();
	}
}

void VActorObject::destroyRenderInstance()
{
	if (mActor != VNULL)
	{
		_destroyActor();
	}

	VPlacementObject::destroyRenderInstance();
}

void VActorObject::setActorName(const VString &actorName)
{
	if (mActorName != actorName)
	{
		if (mActor != VNULL)
		{
			_destroyActor();
		}

		mActorName = actorName;

		if (getSceneNode() != VNULL && !mActorName.empty())
		{
			_createActor();
		}
	}
}

Variant VActorObject::getProperty(const VString &name) const
{
	if (name == "bounding box" && mActor != VNULL)
	{
		return mActor->getBoundingBox();
	}

	return VPlacementObject::getProperty(name);
}

VString VActorObject::getPropertyAsString(const VString &name) const
{
	if (name == "bounding box" && mActor != VNULL)
	{
		const Ogre::AxisAlignedBox& aabb = mActor->getBoundingBox();
		return Ogre::StringConverter::toString(aabb.getMinimum()) + ", " + 
			Ogre::StringConverter::toString(aabb.getMaximum());
	}

	return VPlacementObject::getPropertyAsString(name);
}

void VActorObject::_createActor()
{
	assert(mActor == VNULL);
	assert(!mActorName.empty());

	Ogre::SceneNode *parent = getSceneNode();
	assert(parent != VNULL);
	Ogre::SceneManager *creator = parent->getCreator();
	assert(creator != VNULL);
	
	mActor = new VActor(parent->getName(), mActorName, parent);
}

void VActorObject::_destroyActor()
{
	delete mActor;
	mActor = VNULL;
}

VSceneObjectFactory *VActorObject::getFactory()
{
	static VActorObjectFactory factory;
	return &factory;
}

