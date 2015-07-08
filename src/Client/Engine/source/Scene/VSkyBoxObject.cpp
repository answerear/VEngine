

#include "VSkyBoxObject.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


const VString VSkyBoxObject::msType = "SkyBox";
const VString VSkyBoxObject::msCategory = "Environment";


/**************************************************************************************************
 *										class VSkyBoxFactory
 *************************************************************************************************/

class VSkyBoxFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VSkyBoxObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VSkyBoxObject();
	}
};


/**************************************************************************************************
 *								Property Command of VSkyBoxObject
 *************************************************************************************************/


/**************************************************************************************************
 *									class VSkyBoxObject
 *************************************************************************************************/

VSkyBoxObject::VSkyBoxObject()
	: VSceneObject()
	, mSceneMgr(VNULL)
	, mMaterialName("BaseWhite")
	, mDistance(5000)
	, mDrawFirst(VTRUE)
	, mOrientation(Ogre::Quaternion::IDENTITY)
{
	// TODO: Add properties definition.
}

VSkyBoxObject::~VSkyBoxObject()
{
	destroyRenderInstance();
}

const VString &VSkyBoxObject::getType() const
{
	return msType;
}

const VString &VSkyBoxObject::getCategory() const
{
	return msCategory;
}

void VSkyBoxObject::createRenderInstance()
{
	if (VNULL == mSceneMgr)
	{
		VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();
		mSceneMgr = pGfxSystem->getSceneManager();

		_updateRenderInstanceImp();
	}
}

void VSkyBoxObject::updateRenderInstance()
{
	if (mSceneMgr != VNULL)
	{
		_updateRenderInstanceImp();
	}
}

void VSkyBoxObject::destroyRenderInstance()
{
	if (mSceneMgr != VNULL)
	{
		mSceneMgr->setSkyBox(false, mMaterialName);
		mSceneMgr = VNULL;
	}
}

void VSkyBoxObject::_updateRenderInstanceImp()
{
	assert(mSceneMgr);
	mSceneMgr->setSkyBox(true, mMaterialName, mDistance, mDrawFirst, mOrientation);
}

VSceneObjectFactory *VSkyBoxObject::getFactory()
{
	static VSkyBoxFactory factory;
	return &factory;
}
