

#ifndef __VISION_ACTOR_H__
#define __VISION_ACTOR_H__


#include "VEngineConfig.h"


class VLogicModel;


class VActor
{
public:
	VActor(const VString &name, const VString &modelName, Ogre::SceneNode *node);
	virtual ~VActor();
	
	Ogre::SceneNode *getSceneNode() const;
	Ogre::AxisAlignedBox getBoundingBox() const;
	
protected:
	VLogicModel	*mModel;
};


#endif	/*__VISION_ACTOR_H__*/

