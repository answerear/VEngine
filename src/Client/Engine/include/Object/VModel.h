

#ifndef __VISION_MODEL_H__
#define __VISION_MODEL_H__


#include "VEngineConfig.h"


class VENGINE_DLL VModel
{
	friend class VModelXMLHandler;

	typedef std::list<Ogre::SceneNode*> VSceneNodeList;
	typedef std::list<Ogre::MovableObject*> VObjectList;
	typedef std::list<Ogre::AnimationState*> VAnimationStateList;

public:
	VModel(Ogre::SceneNode *parent, const VString &resourceName, const VString &groupName);
	virtual ~VModel();
	
	const Ogre::AxisAlignedBox &getBoundingBox() const
	{
		return mBoundingBox;
	}

	void setCastShadows(VBOOL castShadows);
	VBOOL getCastShadows() const
	{
		return mCastShadows;
	}

	void setAnimationType(VAnimationType animationType);
	VAnimationType getAnimationType() const
	{
		return mAnimationType;
	}

	void addTime(Ogre::Real offset);
	VBOOL existsObject(Ogre::MovableObject *object) const;

	const std::list<Ogre::MovableObject*> &getObjects() const
	{
		return mMovableObjects;
	}

protected:
	void _create(const VString &resourceName, const VString &groupName);
	void _destroy();

	void _updateCastShadows();
	void _updateAnimationType();
	void _enableAnimationStates();
	void _disableAnimationStates();
	void _createController();
	void _destroyController();

	void _addTime(VAnimationStateList &animationStates, Ogre::Real offset);

protected:
	Ogre::SceneManager	*mSceneMgr;
	Ogre::SceneNode		*mParent;

	Ogre::AxisAlignedBox	mBoundingBox;

	VSceneNodeList	mSceneNodes;
	VObjectList		mMovableObjects;
	VObjectList		mMasterObjects;
	VAnimationStateList	mAnimationStates;

	VAnimationType	mAnimationType;

	Ogre::Controller<Ogre::Real> *mController;

	VBOOL	mCastShadows;
};


#endif	/*__VISION_MODEL_H__*/
