

#include "VModel.h"
#include "VisionExpatParser.h"
#include "VModelSerializer.h"
#include "VisionUtils.h"


using namespace Vision;


/////////////////////////////////////////////////////////////////////////////////////////

class VModelAnimationControllerValue : public Ogre::ControllerValue<Ogre::Real>
{
protected:
	VModel *mModel;

public:
	VModelAnimationControllerValue(VModel *model)
		: mModel(model)
	{
	}

	Ogre::Real getValue() const
	{
		return 0;
	}

	void setValue(Ogre::Real value)
	{
		mModel->addTime(value);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////

VModel::VModel(Ogre::SceneNode *parent, const VString &resourceName, const VString &groupName)
	: mSceneMgr(VNULL)
	, mParent(parent)
	, mBoundingBox()
	, mSceneNodes()
	, mMovableObjects()
	, mMasterObjects()
	, mAnimationStates()
	, mAnimationType(AT_FULL)
	, mController(VNULL)
	, mCastShadows(VFALSE)
{
	assert(parent);
	mSceneMgr = parent->getCreator();
	_create(resourceName, groupName);
}

VModel::~VModel()
{
	_destroy();
}

void VModel::setCastShadows(VBOOL castShadows)
{
	if (mCastShadows != castShadows)
	{
		mCastShadows = castShadows;
		_updateCastShadows();
	}
}

void VModel::setAnimationType(VAnimationType animationType)
{
	if (mAnimationType != animationType)
	{
		mAnimationType = animationType;
		_updateAnimationType();
	}
}

void VModel::addTime(Ogre::Real offset)
{
	_addTime(mAnimationStates, offset);
}

VBOOL VModel::existsObject(Ogre::MovableObject *object) const
{
	return std::find(mMovableObjects.begin(), mMovableObjects.end(), object) != mMovableObjects.end();
}

void VModel::_create(const VString &resourceName, const VString &groupName)
{
	ExpatParser expatParser(VNULL);
	XMLParser *xmlParser = &expatParser;

	VModelXMLHandler handler(this, resourceName, groupName);
	xmlParser->parseXMLFile(handler, resourceName, groupName);

	_updateCastShadows();
	_updateAnimationType();
}

void VModel::_destroy()
{
	_destroyController();

	destroyObjects(mSceneMgr, mMovableObjects);
	mMovableObjects.clear();

	destroyObjects(mSceneMgr, mMasterObjects);
	mMasterObjects.clear();

	VSceneNodeList::iterator it = mSceneNodes.begin();
	while(it != mSceneNodes.end())
	{
		Ogre::SceneNode *pSceneNode = *it;
		pSceneNode->getCreator()->destroySceneNode(pSceneNode);
		pSceneNode = VNULL;
	}

	mSceneNodes.clear();

	mAnimationStates.clear();

	mBoundingBox.setNull();
}

void VModel::_updateCastShadows()
{
	std::for_each(mMovableObjects.begin(), mMovableObjects.end(),
		std::bind2nd(std::mem_fun(&Ogre::MovableObject::setCastShadows), mCastShadows));
}

void VModel::_updateAnimationType()
{
	if (!mAnimationStates.empty())
	{
		switch (mAnimationType)
		{
		case AT_FULL:
			_createController();
			_enableAnimationStates();
			break;
		case AT_FROZE:
			_destroyController();
			_enableAnimationStates();
			break;
		case AT_DISABLE:
			_destroyController();
			_disableAnimationStates();
			break;
		}
	}
}

void VModel::_enableAnimationStates()
{
	Ogre::Real timePosition = Ogre::Math::UnitRandom();
	VAnimationStateList::const_iterator itr;
	for (itr = mAnimationStates.begin(); itr != mAnimationStates.end(); ++itr)
	{
		Ogre::AnimationState *animationState = *itr;
		if (!animationState->getEnabled())
		{
			animationState->setTimePosition(timePosition * animationState->getLength());
			animationState->setEnabled(true);
		}
	}
}

void VModel::_disableAnimationStates()
{
	VAnimationStateList::const_iterator itr;
	for (itr = mAnimationStates.begin(); itr != mAnimationStates.end(); ++itr)
	{
		Ogre::AnimationState *animationState = *itr;
		if (animationState->getEnabled())
		{
			animationState->setEnabled(false);
		}
	}
}

void VModel::_createController()
{
	if (!mController)
	{
		Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
		mController = controllerManager.createFrameTimePassthroughController(
			Ogre::ControllerValueRealPtr(new VModelAnimationControllerValue(this)));
	}
}

void VModel::_destroyController()
{
	if (mController)
	{
		Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
		controllerManager.destroyController(mController);
		mController = NULL;
	}
}

void VModel::_addTime(VAnimationStateList &animationStates, Ogre::Real offset)
{
	VAnimationStateList::const_iterator itr;
	for (itr = animationStates.begin(); itr != animationStates.end(); ++itr)
	{
		Ogre::AnimationState *animationState = *itr;
		animationState->addTime(offset);
	}
}
