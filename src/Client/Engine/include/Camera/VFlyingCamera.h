

#ifndef __VISION_FLYING_CAMERA_H__
#define __VISION_FLYING_CAMERA_H__


#include "VCamera.h"


/**
 * @class VFlyingCamera
 * 飞行模式相机.
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
	/** 前后移动 */
	void walk(VFLOAT step);
	/** 左右移动 */
	void strafe(VFLOAT step);
	/** 上下移动 */
	void fly(VFLOAT step);

	/** 绕x轴旋转 */
	void pitch(VFLOAT degree);
	/** 绕y轴旋转 */
	void yaw(VFLOAT degree);
	/** 绕z轴旋转 */
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
