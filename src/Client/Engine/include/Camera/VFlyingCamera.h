

#ifndef __VISION_FLYING_CAMERA_H__
#define __VISION_FLYING_CAMERA_H__


#include "VCamera.h"


/**
 * @class VFlyingCamera
 * ����ģʽ���.
 */
class VENGINE_DLL VFlyingCamera : public VCamera
{
public:
	VFlyingCamera();
	virtual ~VFlyingCamera();

private:
	VFlyingCamera(const VFlyingCamera &);
	VFlyingCamera &operator =(const VFlyingCamera &);

public:
	/** ǰ���ƶ� */
	void walk(VFLOAT step);
	/** �����ƶ� */
	void strafe(VFLOAT step);
	/** �����ƶ� */
	void fly(VFLOAT step);

	/** ��x����ת */
	void pitch(VFLOAT degree);
	/** ��y����ת */
	void yaw(VFLOAT degree);
	/** ��z����ת */
	void roll(VFLOAT degree);

	void setPosition(const Vector3 &pos);
	void setPosition(VFLOAT x, VFLOAT y, VFLOAT z);
	const Vector3 &getPosition() const;

	Vector3 getUp() const;
	Vector3 getRight() const;
	Vector3 getDirection() const;

	void lookAt(const Vector3 &target);
	void lookAt(VFLOAT x, VFLOAT y, VFLOAT z);
};


#endif	/*__VISION_FLYING_CAMERA_H__*/
