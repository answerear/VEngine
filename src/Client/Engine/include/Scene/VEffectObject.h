

#ifndef __VISION_EFFECT_OBJECT_H__
#define __VISION_EFFECT_OBJECT_H__


#include "VPlacementObject.h"
#include "VSceneObjectFactory.h"


class VEffect;

class VEffectObject : public VPlacementObject
{
	friend class VEffectObjectFactory;

public:
	VEffectObject();
	virtual ~VEffectObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	void setEffectName(const VString &name);
	const VString &getEffectName() const
	{
		return mEffectName;
	}

	VEffect *getEffect() const
	{
		return mEffect;
	}

	static VSceneObjectFactory *getFactory();

protected:
	void _createEffect();
	void _destroyEffect();

protected:
	static const VString msType;
	static const VString msCategory;

	Ogre::SceneManager	*mSceneMgr;
	VEffect				*mEffect;
	VString				mEffectName;
};


#endif	/*__VISION_EFFECT_OBJECT_H__*/
