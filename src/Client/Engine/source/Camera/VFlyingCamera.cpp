

#include "VFlyingCamera.h"


VFlyingCamera::VFlyingCamera()
	: VCamera()
{

}

VFlyingCamera::~VFlyingCamera()
{

}


void VFlyingCamera::walk(VFLOAT step)
{
	if (mNativeCamera)
	{
		Vector3 offset(0.0f, 0.0f, step);
		mNativeCamera->moveRelative(offset);
	}
}

void VFlyingCamera::strafe(VFLOAT step)
{
	if (mNativeCamera)
	{
		Vector3 offset(step, 0.0f, 0.0f);
		mNativeCamera->moveRelative(offset);
	}
}

void VFlyingCamera::fly(VFLOAT step)
{
	if (mNativeCamera)
	{
		Vector3 offset(0.0f, step, 0.0f);
		mNativeCamera->moveRelative(offset);
	}
}

void VFlyingCamera::pitch(VFLOAT degree)
{
	if (mNativeCamera)
	{
		Radian angle(degree);
		mNativeCamera->pitch(angle);
	}
}

void VFlyingCamera::yaw(VFLOAT degree)
{
	if (mNativeCamera)
	{
		Radian angle(degree);
		mNativeCamera->yaw(angle);
	}
}

void VFlyingCamera::roll(VFLOAT degree)
{
	if (mNativeCamera)
	{
		Radian angle(degree);
		mNativeCamera->roll(angle);
	}
}

void VFlyingCamera::setPosition(const Vector3 &pos)
{
	if (mNativeCamera)
	{
		mNativeCamera->setPosition(pos);
	}
}

void VFlyingCamera::setPosition(VFLOAT x, VFLOAT y, VFLOAT z)
{
	if (mNativeCamera)
	{
		mNativeCamera->setPosition(x, y, z);
	}
}

const Vector3 &VFlyingCamera::getPosition() const
{
	if (mNativeCamera)
	{
		return mNativeCamera->getPosition();
	}
	return Vector3::ZERO;
}

Vector3 VFlyingCamera::getUp() const
{
	if (mNativeCamera)
	{
		return mNativeCamera->getRealUp();
	}
	return Vector3::UNIT_Y;
}

Vector3 VFlyingCamera::getRight() const
{
	if (mNativeCamera)
	{
		return mNativeCamera->getRealRight();
	}
	return Vector3::UNIT_X;
}

Vector3 VFlyingCamera::getDirection() const
{
	if (mNativeCamera)
	{
		return mNativeCamera->getRealDirection();
	}
	return Vector3::UNIT_Z;
}

void VFlyingCamera::lookAt(const Vector3 &target)
{
	if (mNativeCamera)
	{
		mNativeCamera->lookAt(target);
	}
}

void VFlyingCamera::lookAt(VFLOAT x, VFLOAT y, VFLOAT z)
{
	if (mNativeCamera)
	{
		mNativeCamera->lookAt(x, y, z);
	}
}
