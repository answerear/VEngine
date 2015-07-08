

#ifndef __VISION_SKY_DOME_OBJECT_H__
#define __VISION_SKY_DOME_OBJECT_H__


#include "VSceneObject.h"
#include "VSceneObjectFactory.h"


class VSkyDomeObject : public VSceneObject
{
	friend class VSkyDomeMaterialNameCmd;
	friend class VSkyDomeFactory;

public:
	VSkyDomeObject();
	virtual ~VSkyDomeObject();

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
	Ogre::Real			mCurvature;
	Ogre::Real			mTiling;
	Ogre::Real			mDistance;
	VBOOL				mDrawFirst;
	Ogre::Quaternion	mOrientation;
	VINT32				mXsegments;
	VINT32				mYsegments;
	VINT32				mYsegments_keep;
};


#endif	/*__VISION_SKY_DOME_OBJECT_H__*/
