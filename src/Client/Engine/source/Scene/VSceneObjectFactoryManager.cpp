

#include "VSceneObjectFactoryManager.h"


template<> VSceneObjectFactoryManager *VSingleton<VSceneObjectFactoryManager>::mInstance = VNULL;


VSceneObjectFactoryManager::VSceneObjectFactoryManager()
{

}

VSceneObjectFactoryManager::~VSceneObjectFactoryManager()
{

}

VSceneObjectPtr VSceneObjectFactoryManager::createInstance(const VString &type) const
{
	VFactory<VSceneObject> *factory = findFactory(type);
	VSceneObject *object = factory->createInstance();
	assert(object->getType() == type);
	return VSceneObjectPtr(object, VFactory<VSceneObject>::VDeleter(factory));
}

