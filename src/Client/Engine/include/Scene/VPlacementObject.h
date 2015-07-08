

#ifndef __VISION_PLACEMENT_OJBECT_H__
#define __VISION_PLACEMENT_OJBECT_H__


#include "VSceneObject.h"
#include "VTransform.h"


class VPlacementObject : public VSceneObject
{
	friend class VPlacementPositionCmd;
	friend class VPlacementOrientationCmd;
	friend class VPlacementOrientationXYZCmd;
	friend class VPlacementScaleCmd;
	friend class VPlacementCreateLevelCmd;

public:
	VPlacementObject();
	virtual ~VPlacementObject();

	void createRenderInstance();
	void destroyRenderInstance();

	Ogre::SceneNode *getSceneNode() const
	{
		return mSceneNode;
	}

	Ogre::Real getCreateLevel() const
	{
		return mCreateLevel;
	}

	void setCreateLevel(Ogre::Real level);

protected:
	void _addBaseProperty();
	void _updateCreateLevel();

protected:
	Ogre::SceneNode	*mSceneNode;

	Ogre::Real	mCreateLevel;
	VTransform	mTransform;
};


#endif	/*__VISION_PLACEMENT_OJBECT_H__*/
