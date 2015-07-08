

#ifndef __VISION_SCENE_INFO_H__
#define __VISION_SCENE_INFO_H__


#include "VEngineConfig.h"


class VSceneInfo
{
	friend class VScene;
	friend class VSceneXMLHandler;

public:
	VSceneInfo();
	virtual ~VSceneInfo();

	virtual void reset();

protected:
	Ogre::String	mName;
	Ogre::String	mAuthor;
	Ogre::String	mCopyright;
	Ogre::String	mDescription;
	Ogre::String	mTerrainFilename;
	Ogre::Real		mTerrainMinHeight;
	Ogre::Real		mTerrainMaxHeight;

};


#endif	/*__VISION_SCENE_INFO_H__*/
