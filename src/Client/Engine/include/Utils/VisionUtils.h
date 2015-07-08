

#ifndef __VISION_ENGINE_UTILS_H__
#define __VISION_ENGINE_UTILS_H__


#include "VEngineConfig.h"


namespace Vision
{
	////////////////////////////////////////////////////////

	Ogre::AnimationState *getAnimationStateSafely(Ogre::AnimationStateSet *animationStateSet, const VString &name);

	Ogre::AnimationState *getAnimationStateSafely(Ogre::Entity *entity, const VString &name);

	Ogre::Animation *getAnimationSafely(const Ogre::Skeleton *skeleton, const VString &name);

	Ogre::Animation *getAnimationSafely(const Ogre::SkeletonPtr &skeleton, const VString &name);

	Ogre::Animation *getAnimationSafely(const Ogre::Mesh *mesh, const VString &name);

	Ogre::Animation *getAnimationSafely(const Ogre::MeshPtr &mesh, const VString &name);

	////////////////////////////////////////////////////////

	typedef std::list<Ogre::MovableObject*> VObjectList;

	void findAllAttachedObjects(Ogre::SceneNode *sn, VObjectList &objects);

	void destroyObject(Ogre::SceneManager *creator, Ogre::MovableObject *object);

	void destroyObjects(Ogre::SceneManager *creator, const VObjectList &objects);

	void cleanupSceneNode(Ogre::SceneNode *sn);

	void cleanupAndDestroySceneNode(Ogre::SceneNode *sn);

	/////////////////////////////////////////////////////////////////////////////////////

	// Returns transformed bounding box which will skinning by the given skeleton.
	Ogre::AxisAlignedBox skinningTransformBoundingBox(const Ogre::AxisAlignedBox &boundingBox, const Ogre::Skeleton *skeleton);

	// Returns transformed bounding box which will skinning by the given skeleton.
	Ogre::AxisAlignedBox skinningTransformBoundingBox(const Ogre::AxisAlignedBox &boundingBox, const Ogre::SkeletonPtr &skeleton);

	// Returns a bounding box that includes all possible positions of bones in the given skeleton.
	Ogre::AxisAlignedBox getSkeletonBoundingBox(const Ogre::Skeleton *skeleton);

	// Returns a bounding box that includes all possible positions of bones in the given skeleton.
	Ogre::AxisAlignedBox getSkeletonBoundingBox(const Ogre::SkeletonPtr &skeleton);

	/////////////////////////////////////////////////////////////////////////////////////

	Ogre::MeshPtr createNullMeshForSkeleton(const VString &meshName, const Ogre::SkeletonPtr &skeleton);

	Ogre::MeshPtr createNullMeshForSkeleton(const Ogre::SkeletonPtr &skeleton);
}


#endif	/*__VISION_ENGINE_UTILS_H__*/
