

#include "VTerrainLiquidObject.h"


const VString VTerrainLiquidObject::msType = "TerrainLiquid";
const VString VTerrainLiquidObject::msCategory = "StaticEntity";


/**************************************************************************************************
 *									class VLightObjectFactory
 *************************************************************************************************/

class VTerrainLiquidObjectFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VTerrainLiquidObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VTerrainLiquidObject();
	}
};


/**************************************************************************************************
 *								Property Command of VTerrainLiquidObject
 *************************************************************************************************/


/**************************************************************************************************
 *									class VTerrainLiquidObject
 *************************************************************************************************/

VTerrainLiquidObject::VTerrainLiquidObject()
	: VSceneObject()
	, mSceneMgr(VNULL)
{
	// TODO: Add properties definition.
}

VTerrainLiquidObject::~VTerrainLiquidObject()
{
	destroyRenderInstance();
}

const VString &VTerrainLiquidObject::getType() const
{
	return msType;
}

const VString &VTerrainLiquidObject::getCategory() const
{
	return msCategory;
}

void VTerrainLiquidObject::createRenderInstance()
{
	if (VNULL == mSceneMgr)
	{
	}
}

void VTerrainLiquidObject::destroyRenderInstance()
{
	if (mSceneMgr != VNULL)
	{
	}
}

VSceneObjectFactory *VTerrainLiquidObject::getFactory()
{
	static VTerrainLiquidObjectFactory factory;
	return &factory;
}

