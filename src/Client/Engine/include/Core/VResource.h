

#ifndef __VISION_RESOURCE_H__
#define __VISION_RESOURCE_H__


#include "VEngineConfig.h"


namespace Vision
{
	extern const VString DEFAULT_RESOURCE_GROUP_NAME;
	extern const VString AUTO_ANIMATION_NAME;

	VBOOL splitResourceName(const Ogre::String& name,Ogre::String& resourceName,Ogre::String& groupName);

	VBOOL findCorrelativeResource(VString& resourceName, VString& groupName,
		const VString& baseResourceName, const VString& baseGroupName);

	Ogre::ResourcePtr loadCorrelativeResource(const Ogre::String& resourceName, const Ogre::String& groupName,
		const Ogre::String& baseResourceName, const Ogre::String& baseGroupName,
		Ogre::ResourceManager& resourceManager);

	Ogre::MeshPtr loadMesh(const Ogre::String& meshName, const Ogre::String& groupName,
		const Ogre::String& baseResourceName, const Ogre::String& baseGroupName);
}


#endif	/*__VISION_RESOURCE_H__*/
