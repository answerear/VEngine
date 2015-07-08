

#ifndef __VISION_ENGINE_TYPE_H__
#define __VISION_ENGINE_TYPE_H__


#include "VEngineConfig.h"
#include "VVariant.h"


typedef Ogre::Vector2		Vector2;
typedef Ogre::Vector3		Vector3;
typedef Ogre::Real			Real;
typedef Ogre::Matrix3		Matrix3;
typedef Ogre::Matrix4		Matrix4;
typedef Ogre::Quaternion	Quaternion;
typedef Ogre::Degree		Degree;
typedef Ogre::Radian		Radian;

typedef Ogre::ColourValue	ColorValue;

typedef OIS::KeyCode		KeyCode;
typedef OIS::MouseButtonID	MouseButtonID;
typedef OIS::Mouse			VMouse;
typedef OIS::Keyboard		VKeyboard;

enum VAnimationType
{
	AT_FULL,
	AT_FROZE,
	AT_DISABLE,
};


#endif	/*__VISION_ENGINE_TYPE_H__*/
