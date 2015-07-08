

#ifndef __VISION_SCENE_OBJECT_FACTORY_MANAGER_H__
#define __VISION_SCENE_OBJECT_FACTORY_MANAGER_H__


#include "VSingleton.h"
#include "VSceneObject.h"
#include "VFactoryManager.h"


class VSceneObjectFactoryManager
	: public VFactoryManager<VSceneObject>
	, public VSingleton<VSceneObjectFactoryManager>
{
public:
	VSceneObjectFactoryManager();
	~VSceneObjectFactoryManager();

	VSceneObjectPtr createInstance(const VString &type) const;
};


#endif	/*__VISION_SCENE_OBJECT_FACTORY_MANAGER_H__*/
