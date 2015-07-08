

#include "VModelSerializer.h"
#include "VisionXMLAttributes.h"
#include "VModel.h"
#include "VisionUtils.h"
#include "VResource.h"
#include <OgreTagPoint.h>


using namespace Vision;


VModelXMLHandler::VModelXMLHandler(VModel *model, const VString &resourceName, const VString &groupName)
	: mModel(model)
	, mMasterEntity(VNULL)
	, mLastMovable(VNULL)
	, mLastAttachNode(VNULL)
	, mModelResourceName(resourceName)
	, mModelGroupName(groupName)
{

}

VModelXMLHandler::~VModelXMLHandler()
{

}

void VModelXMLHandler::startElement(const VString &element, const XMLAttributes &attributes)
{
	if (element == "entity")
	{
		if (mLastMovable || mLastAttachNode)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate model resource " + mModelResourceName,
				"Model::create");
		}

		// Determinate which node to attach to
		const VString *boneName = VNULL;
		if (attributes.exists("node"))
		{
			if (!mMasterEntity || !mMasterEntity->hasSkeleton())
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
					"Invalidate model resource " + mModelResourceName,
					"Model::create");
			}

			boneName = &attributes.getValue("node");
			mMasterEntity->getSkeleton()->getBone(*boneName);
		}

		// Determinate should be normalise normal
		VBOOL normalise = VFALSE;
		if (attributes.exists("normalise"))
		{
			normalise = attributes.getValueAs<VBOOL>("normalise");
		}

		// Determinate it's decal target
		VBOOL receive_decals = VFALSE;
		if (attributes.exists("receive_decals"))
		{
			receive_decals = attributes.getValueAs<VBOOL>("receive_decals");
		}

		// Get the mesh name
		const VString &meshName = attributes.getValue("mesh");

		// Load the mesh
		Ogre::MeshPtr mesh = loadMesh(meshName, Ogre::StringUtil::BLANK, mModelResourceName, mModelGroupName);

		// Create the entity
		Ogre::Entity* entity = mModel->mSceneMgr->createEntity(
			mModel->mParent->getName() + "/" + mesh->getName(),
			mesh->getName());
		mModel->mMovableObjects.push_back(entity);

// 		if (receive_decals)
// 		{
// 			entity->addQueryFlags(OQF_DECAL_TARGET);
// 			// If attaching to master entity, should enable master entity query flags too,
// 			// otherwise will failed to query.
// 			if (mMasterEntity)
// 			{
// 				mMasterEntity->addQueryFlags(OQF_DECAL_TARGET);
// 			}
// 		}

		if (boneName)
		{
			mLastAttachNode = mMasterEntity->attachObjectToBone(*boneName, entity);
		}
		else
		{
			mModel->mParent->attachObject(entity);
		}

		mLastMovable = entity;

		// Compute bounding box
		if (mesh->hasSkeleton())
		{
			// Merge skinned bounding box
			mModel->mBoundingBox.merge(skinningTransformBoundingBox(mesh->getBounds(), mesh->getSkeleton()));
		}
		else
		{
			// Merge normal bounding box
			mModel->mBoundingBox.merge(mesh->getBounds());
		}

		// Add predefined animations
		Ogre::AnimationState* animationState = getAnimationStateSafely(entity, AUTO_ANIMATION_NAME);
		if (animationState)
		{
// 			convertEntityToHardwareSkinning(entity);
			mModel->mAnimationStates.push_back(animationState);
		}
	}
	else if (element == "offset")
	{
		if (!mLastMovable)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate model resource " + mModelResourceName,
				"Model::create");
		}

		if (!mLastAttachNode)
		{
			if (mLastMovable->getParentSceneNode() != mModel->mParent)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
					"Invalidate model resource " + mModelResourceName,
					"Model::create");
			}

			// Detach form parent and attach to new offset scene node
			Ogre::SceneNode* attachNode = mLastMovable->getParentSceneNode()->createChildSceneNode();
			mModel->mSceneNodes.push_back(attachNode);
			mLastMovable->getParentSceneNode()->detachObject(mLastMovable->getName());
			attachNode->attachObject(mLastMovable);
			mLastAttachNode = attachNode;
		}
	}
	else if (element == "translate")
	{
		if (!mLastAttachNode)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate model resource " + mModelResourceName,
				"Model::create");
		}

		mLastAttachNode->setPosition(
			attributes.getValueAs<Ogre::Real>("x"),
			attributes.getValueAs<Ogre::Real>("y"),
			attributes.getValueAs<Ogre::Real>("z"));
		mLastAttachNode->setInitialState();
	}
	else if (element == "rotate")
	{
		if (!mLastAttachNode)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate model resource " + mModelResourceName,
				"Model::create");
		}

		mLastAttachNode->setOrientation(
			attributes.getValueAs<Ogre::Real>("w"),
			attributes.getValueAs<Ogre::Real>("x"),
			attributes.getValueAs<Ogre::Real>("y"),
			attributes.getValueAs<Ogre::Real>("z"));
		mLastAttachNode->setInitialState();
	}
	else if (element == "scale")
	{
		if (!mLastAttachNode)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate model resource " + mModelResourceName,
				"Model::create");
		}

		mLastAttachNode->setScale(
			attributes.getValueAs<Ogre::Real>("x"),
			attributes.getValueAs<Ogre::Real>("y"),
			attributes.getValueAs<Ogre::Real>("z"));
		mLastAttachNode->setInitialState();
	}
	else if (element == "frame")
	{
		if (mMasterEntity)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate model resource " + mModelResourceName,
				"Model::create");
		}

		const Ogre::String& frameName = attributes.getValue("name");

		// Load the skeleton
		Ogre::SkeletonPtr skeleton = loadCorrelativeResource(
			frameName, Ogre::StringUtil::BLANK,
			mModelResourceName, mModelGroupName,
			Ogre::SkeletonManager::getSingleton());

		if (skeleton.isNull())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
				"Unable to load frame " + frameName +
				" for Model " + mModelResourceName +
				". This Model will not be animated. " +
				"You can ignore this message if you are using an offline tool.",
				"Model::create");
		}
		else if (!skeleton->getNumBones())
		{
			// Null skeleton, don't know why this happen
			static int i;
			++i;
		}
		else
		{
			// Construct fake mesh and entity
			Ogre::MeshPtr mesh = createNullMeshForSkeleton(skeleton);
			mMasterEntity = mModel->mSceneMgr->createEntity(
				mModel->mParent->getName() + "/" + mesh->getName(),
				mesh->getName());
			mModel->mMasterObjects.push_back(mMasterEntity);

			mModel->mParent->attachObject(mMasterEntity);

			// Merge frame structure bounding box
			mModel->mBoundingBox.merge(mMasterEntity->getBoundingBox());

			// Add predefined animations
			Ogre::AnimationState* animationState = getAnimationStateSafely(mMasterEntity, AUTO_ANIMATION_NAME);
			if (animationState)
			{
				mModel->mAnimationStates.push_back(animationState);
			}
		}
	}
	else if (element == "model")
	{

	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
			"Unexpected data was found while parsing the model file: '" + element + "' is unknown.",
			"VModel::create");
	}
}

void VModelXMLHandler::endElement(const VString &element)
{
	if (element == "entity")
	{
		mLastMovable = VNULL;
		mLastAttachNode = VNULL;
	}
	else if (element == "offset")
	{
		assert(mLastMovable);
		assert(mLastAttachNode);
		mLastAttachNode = VNULL;
	}
	else if (element == "translate")
	{
		assert(mLastAttachNode);
	}
	else if (element == "rotate")
	{
		assert(mLastAttachNode);
	}
	else if (element == "scale")
	{
		assert(mLastAttachNode);
	}
	else if (element == "frame")
	{
	}
	else if (element == "model")
	{
	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
			"Unexpected data was found while parsing the model file: '" + element + "' is unknown.",
			"Model::create");
	}
}
