

#include "VisionUtils.h"

using namespace Ogre;

namespace Vision
{
	////////////////////////////////////////////////////////////////////////////////////////

	AnimationState *getAnimationStateSafely(AnimationStateSet *animationStateSet, const VString &name)
	{
		if (animationStateSet)
		{
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
			if (animationStateSet->hasAnimationState(name))
				return animationStateSet->getAnimationState(name);
#else
			AnimationStateSet::iterator it = animationStateSet->find(name);
			if (it != animationStateSet->end())
				return &it->second;
#endif
		}

		return VNULL;
	}

	AnimationState *getAnimationStateSafely(Entity *entity, const VString &name)
	{
		assert(entity);
		return getAnimationStateSafely(entity->getAllAnimationStates(), name);
	}

	Animation *getAnimationSafely(const Skeleton *skeleton, const VString &name)
	{
		return skeleton ? skeleton->_getAnimationImpl(name) : 0;
	}

	Animation *getAnimationSafely(const SkeletonPtr &skeleton, const VString &name)
	{
		return getAnimationSafely(skeleton.get(), name);
	}

	Animation *getAnimationSafely(const Mesh *mesh, const VString &name)
	{
		return mesh ? getAnimationSafely(mesh->getSkeleton(), name) : 0;
	}

	Animation *getAnimationSafely(const MeshPtr &mesh, const VString &name)
	{
		return getAnimationSafely(mesh.get(), name);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	static void addAttachedObject(MovableObject *object, VObjectList &objects);
	
	template <class Iterator>
	static inline void addAttachedObjects(Iterator it, VObjectList &objects)
	{
		while (it.hasMoreElements())
		{
			MovableObject *object = it.getNext();
			addAttachedObject(object, objects);
		}
	}
	
	static void addAttachedObject(MovableObject *object, VObjectList &objects)
	{
		objects.push_back(object);
		
		if (object->getMovableType() == "Entity")
		{
			Entity *entity = static_cast<Entity*>(object);
			addAttachedObjects(entity->getAttachedObjectIterator(), objects);
		}
	}

	void findAllAttachedObjects(SceneNode *sn, VObjectList &objects)
	{
		addAttachedObjects(sn->getAttachedObjectIterator(), objects);

		SceneNode::ChildNodeIterator itNodes = sn->getChildIterator();
		while (itNodes.hasMoreElements())
		{
			Node *node = itNodes.getNext();
			SceneNode *child = static_cast<SceneNode*>(node);
			findAllAttachedObjects(child, objects);
		}
	}

	void destroyObject(SceneManager *creator, MovableObject *object)
	{
		const Ogre::String &type = object->getMovableType();
		const Ogre::String &name = object->getName();

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
		if (type == "Camera")
		{
			creator->destroyCamera(name);
		}
		else if (type == "StaticGeometry")
		{
			creator->destroyStaticGeometry(name);
		}
		else
		{
			creator->destroyMovableObject(object);
		}
#else
		if (type == "Entity")
		{
			creator->destroyEntity(name);
		}
		else if (type == "Camera")
		{
			creator->destroyCamera(name);
		}
		else if (type == "Light")
		{
			creator->destroyLight(name);
		}
		else if (type == "BillboardSet")
		{
			creator->removeBillboardSet(name);
		}
		else if (type == "StaticGeometry")
		{
			creator->destroyStaticGeometry(name);
		}
		else if (type == "ParticleSystem")
		{
			ParticleSystemManager::getSingleton().destroySystem(name);
		}
		else
		{
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
				"Unknown object type " + type,
				"destroyObject");
		}
#endif
	}

	void destroyObjects(SceneManager *creator, const VObjectList &objects)
	{
		for (VObjectList::const_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			destroyObject(creator, *it);
		}
	}

	void cleanupSceneNode(SceneNode *sn)
	{
		VObjectList objects;
		findAllAttachedObjects(sn, objects);
		sn->removeAndDestroyAllChildren();

		destroyObjects(sn->getCreator(), objects);
	}

	void cleanupAndDestroySceneNode(SceneNode *sn)
	{
		cleanupSceneNode(sn);
		sn->getCreator()->destroySceneNode(sn);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	static AxisAlignedBox _getSkeletonBoundingBox(const Skeleton *_skeleton, bool deltaOnly)
	{
		Skeleton *skeleton = const_cast<Skeleton*>(_skeleton);
		assert(skeleton);

		AxisAlignedBox boundingBox;

		// calculate bone initial positions
		std::map<Bone*, Ogre::Vector3> boneInitialPositions;

		skeleton->reset();

		Skeleton::BoneIterator itBone = skeleton->getBoneIterator();
		while (itBone.hasMoreElements())
		{
			Bone *bone = itBone.getNext();
			const Ogre::Vector3 &pos = bone->_getDerivedPosition();
			if (deltaOnly)
			{
				boneInitialPositions[bone] = pos;
			}
			else
			{
				boundingBox.merge(pos);
			}
		}

		// create animation state set
		AnimationStateSet animationStateSet;
		skeleton->_initAnimationState(&animationStateSet);

		// for each animation
		AnimationStateIterator asi = animationStateSet.getAnimationStateIterator();
		while (asi.hasMoreElements())
		{
			AnimationState *as = asi.getNext();

			typedef std::set<Ogre::Real> RealSet;
			typedef Ogre::ConstVectorIterator<RealSet> ConstRealSetIterator;

			// collect key frame times
			RealSet keyFrameTimes;
			Animation *animation = skeleton->getAnimation(as->getAnimationName());
			Animation::NodeTrackIterator nti = animation->getNodeTrackIterator();
			while (nti.hasMoreElements())
			{
				AnimationTrack *nt = nti.getNext();
				for (ushort i = 0; i < nt->getNumKeyFrames(); ++i)
				{
					KeyFrame *tkf = nt->getKeyFrame(i);
					keyFrameTimes.insert(tkf->getTime());
				}
			}

			// collect bounds of the animation
			as->setEnabled(true);
			ConstRealSetIterator kfti(keyFrameTimes.begin(), keyFrameTimes.end());
			while (kfti.hasMoreElements())
			{
				// Applies the time position
				as->setTimePosition(kfti.getNext());
				skeleton->setAnimationState(animationStateSet);

				Skeleton::BoneIterator itBone = skeleton->getBoneIterator();
				while (itBone.hasMoreElements())
				{
					Bone *bone = itBone.getNext();
					const Ogre::Vector3 &pos = bone->_getDerivedPosition();
					if (deltaOnly)
					{
						boundingBox.merge(pos - boneInitialPositions[bone]);
					}
					else
					{
						boundingBox.merge(pos);
					}
				}
			}
			as->setEnabled(false);
		}

		// reset skeleton
		skeleton->reset();

		return boundingBox;
	}

	AxisAlignedBox skinningTransformBoundingBox(const AxisAlignedBox &boundingBox, const Skeleton *skeleton)
	{
		if (boundingBox.isNull())
			return boundingBox;

		AxisAlignedBox deltaBoundingBox = _getSkeletonBoundingBox(skeleton, true);
		if (deltaBoundingBox.isNull())
			return boundingBox;

		AxisAlignedBox newBoundingBox(boundingBox);
		newBoundingBox.merge(boundingBox.getMinimum() + deltaBoundingBox.getMinimum());
		newBoundingBox.merge(boundingBox.getMaximum() + deltaBoundingBox.getMaximum());
		return newBoundingBox;
	}

	AxisAlignedBox skinningTransformBoundingBox(const AxisAlignedBox &boundingBox, const SkeletonPtr &skeleton)
	{
		return skinningTransformBoundingBox(boundingBox, skeleton.get());
	}

	AxisAlignedBox getSkeletonBoundingBox(const Skeleton *skeleton)
	{
		return _getSkeletonBoundingBox(skeleton, false);
	}

	AxisAlignedBox getSkeletonBoundingBox(const SkeletonPtr &skeleton)
	{
		return getSkeletonBoundingBox(skeleton.get());
	}

	/////////////////////////////////////////////////////////////////////////////////////

	static class NullSkeletonMeshLoader : public ManualResourceLoader
	{
	public:
		void loadResource(Resource *resource)
		{
			Mesh *mesh = static_cast<Mesh*>(resource);
			mesh->setAutoBuildEdgeLists(false);
			mesh->setSkeletonName(mesh->getName());
			mesh->_setBounds(getSkeletonBoundingBox(mesh->getSkeleton()));
		}
	} gsNullSkeletonMeshLoader;

	MeshPtr createNullMeshForSkeleton(const VString &meshName, const SkeletonPtr &skeleton)
	{
		MeshManager &mm = MeshManager::getSingleton();

		MeshPtr mesh = mm.getByName(meshName);
		if (mesh.isNull())
		{
			if (meshName == skeleton->getName())
			{
				mesh = mm.createManual(meshName, skeleton->getGroup(), &gsNullSkeletonMeshLoader);
			}
			else
			{
				mesh = mm.createManual(meshName, skeleton->getGroup());
				mesh->setAutoBuildEdgeLists(false);
				mesh->_notifySkeleton(const_cast<SkeletonPtr&>(skeleton));
				mesh->_setBounds(getSkeletonBoundingBox(skeleton));
			}

			mesh->load();
		}

		return mesh;
	}

	MeshPtr createNullMeshForSkeleton(const SkeletonPtr &skeleton)
	{
		return createNullMeshForSkeleton(skeleton->getName(), skeleton);
	}
}
