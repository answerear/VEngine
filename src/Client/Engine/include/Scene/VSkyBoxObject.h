

#ifndef __VISION_SKY_BOX_OBJECT_H__
#define __VISION_SKY_BOX_OBJECT_H__


#include "VSceneObject.h"
#include "VSceneObjectFactory.h"


class VSkyBoxObject : public VSceneObject
{
	friend class VSkyBoxFactory;

public:
	VSkyBoxObject();
	virtual ~VSkyBoxObject();

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

	VString				mMaterialName;
	Ogre::Real			mDistance;
	VBOOL				mDrawFirst;
	Ogre::Quaternion	mOrientation;
};


#endif	/*__VISION_SKY_BOX_OBJECT_H__*/
