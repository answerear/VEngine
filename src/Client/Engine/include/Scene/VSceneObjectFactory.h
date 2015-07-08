

#ifndef __VISION_SCENE_OBJECT_FACTORY_H__
#define __VISION_SCENE_OBJECT_FACTORY_H__


#include "VFactory.h"


class VSceneObject;


class VSceneObjectFactory : public VFactory<VSceneObject>
{
public:
	void destroyInstance(VSceneObject *object);
};



#endif	/*__VISION_SCENE_OBJECT_FACTORY_H__*/
