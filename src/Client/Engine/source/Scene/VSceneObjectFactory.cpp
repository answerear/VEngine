

#include "VSceneObjectFactory.h"
#include "VSceneObject.h"


void VSceneObjectFactory::destroyInstance(VSceneObject *object)
{
	delete object;
}
