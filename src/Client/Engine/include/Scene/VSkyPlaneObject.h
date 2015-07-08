

#ifndef __VISION_SKY_PLANE_OBJECT_H__
#define __VISION_SKY_PLANE_OBJECT_H__


#include "VSceneObject.h"
#include "VSceneObjectFactory.h"


class VSkyPlaneObject : public VSceneObject
{
	friend class VSkyPlaneFactory;

public:
	VSkyPlaneObject();
	virtual ~VSkyPlaneObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	void updateRenderInstance();

	static VSceneObjectFactory *getFactory();

protected:
	void _updateRenderInstanceImp();

protected:
	static const VString msType;
	static const VString msCategory;

	Ogre::SceneManager	*mSceneMgr;

	Ogre::Plane	mPlane;
	VString		mMaterialName;
	Ogre::Real	mScale;
	Ogre::Real	mTiling;
	VBOOL		mDrawFirst;
	Ogre::Real	mBow;
	VINT32		mXsegments;
	VINT32		mYsegments;
};


#endif	/*__VISION_SKY_PLANE_OBJECT_H__*/
