

#ifndef __VISION_CAMERA_H__
#define __VISION_CAMERA_H__


#include "VEngineConfig.h"


class VENGINE_DLL VCamera
{
public:
	VCamera(VBOOL bAutoCreated = VFALSE, const VString &name = "");
	virtual ~VCamera();

private:
	VCamera(const VCamera &);
	VCamera &operator =(const VCamera &);

public:
	const VString &getName() const	{ return mNativeCamera->getName(); }
	VBOOL isAutoCreated() { return mAutoCreated; }

	Ogre::Camera *getNativeCamera() { return mNativeCamera; }

protected:
	Ogre::Camera	*mNativeCamera;
	VBOOL			mAutoCreated;
};


#endif	/*__VISION_CAMERA_H__*/
