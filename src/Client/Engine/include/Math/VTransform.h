

#ifndef __VISION_TRANSFORM_H__
#define __VISION_TRANSFORM_H__


#include "VEngineConfig.h"


class VTransform
{
public:
	VTransform();
	VTransform(const Vector3 &pos, const Quaternion &ori, const Vector3 &s);

	Vector3		position;
	Quaternion	orientation;
	Vector3		scale;
};


#endif	/*__VISION_TRANSFORM_H__*/
