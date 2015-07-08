

#include "VActor.h"
#include "VResource.h"
#include "VLogicModelManager.h"
#include "VLogicModel.h"


using namespace Vision;


VActor::VActor(const VString &name, const VString &modelName, Ogre::SceneNode *node)
	: mModel(VNULL)
{
	VString resourceName, groupName;
	splitResourceName(name, resourceName, groupName);

	mModel = VLOGIC_MODEL_MANAGER.createLogicModel(name, modelName);
	mModel->setParentSceneNode(node);
}

VActor::~VActor()
{
	VLOGIC_MODEL_MANAGER.destroyLogicModel(mModel);
	mModel = VNULL;
}

Ogre::SceneNode *VActor::getSceneNode() const
{
	assert(mModel);
	assert(mModel->getSceneNode());
	return mModel->getSceneNode();
}

Ogre::AxisAlignedBox VActor::getBoundingBox() const
{
	assert(mModel);
	return mModel->getBoundingBox();
}
