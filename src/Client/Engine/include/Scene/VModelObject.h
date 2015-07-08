

#ifndef __VISION_MODEL_OBJECT_H__
#define __VISION_MODEL_OBJECT_H__


#include "VPlacementObject.h"
#include "VSceneObjectFactory.h"
#include "VModel.h"


class VModelObject : public VPlacementObject
{
	friend class VModelObjectFactory;

public:
	VModelObject();
	virtual ~VModelObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	VModel *getModel() const
	{
		return mModel;
	}

	const VString &getModelName() const
	{
		return mModelName;
	}

	void setModelName(const VString &modelName);

	Ogre::Real getAnimationLevel() const
	{
		return mAnimationLevel;
	}

	void setAnimationLevel(Ogre::Real level);

	VBOOL getCastShadows() const
	{
		return mCastShadows;
	}

	void setCastShadows(VBOOL castShadows);

	static VSceneObjectFactory *getFactory();

protected:
	void _createModel();
	void _destroyModel();

protected:
	static const VString msType;
	static const VString msCategory;

	Ogre::SceneManager	*mSceneMgr;

	VModel	*mModel;
	VString mModelName;
	Ogre::Real	mAnimationLevel;
	VBOOL	mCastShadows;
};


#endif	/*__VISION_MODEL_OBJECT_H__*/
