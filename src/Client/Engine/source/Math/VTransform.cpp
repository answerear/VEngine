

#include "VTransform.h"




#include "VTransform.h"


VTransform::VTransform()
	: position(Ogre::Vector3::ZERO)
	, orientation(Ogre::Quaternion::IDENTITY)
	, scale(Ogre::Vector3::UNIT_SCALE)
{

}

VTransform::VTransform(const Vector3 &pos, const Quaternion &ori, const Vector3 &s)
	: position(pos)
	, orientation(ori)
	, scale(s)
{

}
