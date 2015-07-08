
#ifndef __IntermediateTutorial1_h__
#define __IntermediateTutorial1_h__

#include "BaseApplication.h"
#include <deque>


class IntermediateTutorial1 : public BaseApplication
{
public:
	IntermediateTutorial1(void);
	virtual ~IntermediateTutorial1(void);

protected:
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual bool nextLocation(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	void rotateRobotToDirection(void);

	Ogre::Real		mDistance;
	Ogre::Vector3	mDirection;
	Ogre::Vector3	mDestination;

	Ogre::AnimationState	*mAnimationState;

	Ogre::Entity	*mEntity;
	Ogre::SceneNode	*mNode;

	std::deque<Ogre::Vector3>	mWalkList;

	Ogre::Real	mWalkSpeed;
};

#endif	// #ifndef __IntermediateTutorial1_h__
