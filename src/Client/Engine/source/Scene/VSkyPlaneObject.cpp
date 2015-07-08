

#include "VSkyPlaneObject.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


const VString VSkyPlaneObject::msType = "SkyPlane";
const VString VSkyPlaneObject::msCategory = "Environment";


/**************************************************************************************************
 *									class VSkyPlaneFactory
 *************************************************************************************************/

class VSkyPlaneFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VSkyPlaneObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VSkyPlaneObject();
	}
};


/**************************************************************************************************
 *								Property Command of VSkyPlaneObject
 *************************************************************************************************/


/**************************************************************************************************
 *									class VSkyPlaneObject
 *************************************************************************************************/

VSkyPlaneObject::VSkyPlaneObject()
	: VSceneObject()
	, mSceneMgr(VNULL)
	, mPlane()
	, mMaterialName("Base White")
	, mScale(1000)
	, mTiling(10)
	, mDrawFirst(VTRUE)
	, mBow(0)
	, mXsegments(1)
	, mYsegments(1)
{
	// TODO: Add properties definition.
}

VSkyPlaneObject::~VSkyPlaneObject()
{
	destroyRenderInstance();
}

const VString &VSkyPlaneObject::getType() const
{
	return msType;
}

const VString &VSkyPlaneObject::getCategory() const
{
	return msCategory;
}

void VSkyPlaneObject::createRenderInstance()
{
	if (VNULL == mSceneMgr)
	{
		VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();
		mSceneMgr = pGfxSystem->getSceneManager();

		_updateRenderInstanceImp();
	}
}

void VSkyPlaneObject::updateRenderInstance()
{
	if (mSceneMgr != VNULL)
	{
		_updateRenderInstanceImp();
	}
}

void VSkyPlaneObject::destroyRenderInstance()
{
	if (mSceneMgr != VNULL)
	{
		mSceneMgr->setSkyPlane(false, mPlane, mMaterialName);
		mSceneMgr = NULL;
	}
}

void VSkyPlaneObject::_updateRenderInstanceImp()
{
	assert(mSceneMgr);
	mSceneMgr->setSkyPlane(true, mPlane, mMaterialName,
		mScale, mTiling, mDrawFirst, mBow, mXsegments, mYsegments);
}

VSceneObjectFactory *VSkyPlaneObject::getFactory()
{
	static VSkyPlaneFactory factory;
	return &factory;
}


