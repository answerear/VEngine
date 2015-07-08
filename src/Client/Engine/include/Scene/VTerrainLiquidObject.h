

#ifndef __VISION_TERRAIN_LIQUID_OBJECT_H__
#define __VISION_TERRAIN_LIQUID_OBJECT_H__


#include "VSceneObject.h"
#include "VSceneObjectFactory.h"


class VTerrainLiquidObject : public VSceneObject
{
	friend class VTerrainLiquidObjectFactory;

public:
	VTerrainLiquidObject();
	virtual ~VTerrainLiquidObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	static VSceneObjectFactory *getFactory();

protected:

protected:
	static const VString msType;
	static const VString msCategory;

	Ogre::SceneManager	*mSceneMgr;
};


#endif	/*__VISION_TERRAIN_LIQUID_OBJECT_H__*/
