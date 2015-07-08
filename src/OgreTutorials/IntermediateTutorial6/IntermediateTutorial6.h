
#ifndef __IntermediateTutorial6_h__
#define __IntermediateTutorial6_h__


#include "BaseApplication.h"


class IntermediateTutorial6 : public BaseApplication
{
public:
	IntermediateTutorial6(void);
	virtual ~IntermediateTutorial6(void);

protected:
	virtual void createScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	virtual void createProjector(void);
	virtual void makeMaterialReceiveDecal(const Ogre::String &matName);

	Ogre::SceneNode	*mProjectorNode;
	Ogre::Frustum	*mDecalFrustum;
	Ogre::Frustum	*mFilterFrustum;
	float			mAnim;
};


#endif	// #ifndef __IntermediateTutorial6_h__