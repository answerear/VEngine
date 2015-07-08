

#include "VSkyDomeObject.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


const VString VSkyDomeObject::msType = "SkyDome";
const VString VSkyDomeObject::msCategory ="Environment";


/**************************************************************************************************
 *										class VSkyDomeFactory
 *************************************************************************************************/

class VSkyDomeFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VSkyDomeObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VSkyDomeObject();
	}
};


/**************************************************************************************************
 *								Property Command of VSkyDomeObject
 *************************************************************************************************/

class VSkyDomeMaterialNameCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VSkyDomeObject *object = static_cast<const VSkyDomeObject*>(source);
		return object->mMaterialName;
	}

	VString doGetAsString(const void *source) const
	{
		const VSkyDomeObject *object = static_cast<const VSkyDomeObject*>(source);
		return object->mMaterialName;
	}

	void doSet(void *target, const Variant &value)
	{
		VSkyDomeObject *object = static_cast<VSkyDomeObject*>(target);
		object->mMaterialName = VariantCast<VString>(value);
		object->updateRenderInstance();
	}

	void doSetAsString(void *target, const VString &value)
	{
		VSkyDomeObject *object = static_cast<VSkyDomeObject*>(target);
		object->mMaterialName = value;
		object->updateRenderInstance();
	}
};



/**************************************************************************************************
 *									class VSkyDomeObject
 *************************************************************************************************/

VSkyDomeObject::VSkyDomeObject()
	: VSceneObject()
	, mSceneMgr(VNULL)
	, mMaterialName("BaseWhite")
	, mCurvature(10)
	, mTiling(8)
	, mDistance(4000)
	, mDrawFirst(VTRUE)
	, mOrientation(Ogre::Quaternion::IDENTITY)
	, mXsegments(16)
	, mYsegments(16)
	, mYsegments_keep(-1)
{
	// TODO: Add properties definition.
	if (createPropertyDictionary("SkyDome"))
	{
		static VSkyDomeMaterialNameCmd materialNameCmd;
		defineProperty(
			"material",
			"the name of material",
			"Material",
			&materialNameCmd,
			PF_PRIMARY);
// 		static DistanceCmd distanceCmd;
// 		defineProperty(
// 			"distance",
// 			"the distance of SkyDome.",
// 			"Ogre::Real",    
// 			&distanceCmd,
// 			0);
// 
// 		static DrawFirstCmd drawFirstCmd;
// 		defineProperty(
// 			"draw first",
// 			"whether the box is drawn before all other geometry in the scene.",
// 			"Bool",
// 			&drawFirstCmd,
// 			0);
// 
// 		static OrientationCmd orientationCmd;
// 		defineProperty(
// 			"orientation",
// 			"Optional parameter to specify the orientation of the box.",
// 			"Orientation",
// 			&orientationCmd,
// 			0);
// 
// 		static CurvatureCmd curvatureCmd;
// 		defineProperty(
// 			"curvature",
// 			"The curvature of the dome.",
// 			"Ogre::Real",
// 			&curvatureCmd,
// 			0);
// 
// 		static TilingCmd tilingCmd;
// 		defineProperty(
// 			"tiling",
// 			"How many times to tile the texture(s) across the dome.",
// 			"Ogre::Real",
// 			&tilingCmd,
// 			0);
// 
// 		static XsegmentsCmd xsegmentsCmd;
// 		defineProperty(
// 			"x segments",
// 			"the number of xsegments the dome will have to it",
// 			"Integer",
// 			&xsegmentsCmd,
// 			0);
// 
// 		static YsegmentsCmd ysegmentsCmd;
// 		defineProperty(
// 			"y segments",
// 			"the number of ysegments the dome will have to it",
// 			"Integer",
// 			&ysegmentsCmd,
// 			0);
// 
// 		static Ysegments_keepCmd ysegments_keepCmd;
// 		defineProperty(
// 			"y segments keep",
// 			"the keep of ysegments",
// 			"Integer",
// 			&ysegments_keepCmd,
// 			0);
// 
	}
}

VSkyDomeObject::~VSkyDomeObject()
{
	destroyRenderInstance();
}

const VString &VSkyDomeObject::getType() const
{
	return msType;
}

const VString &VSkyDomeObject::getCategory() const
{
	return msCategory;
}

void VSkyDomeObject::createRenderInstance()
{
	if (VNULL == mSceneMgr)
	{
		VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();
		mSceneMgr = pGfxSystem->getSceneManager();

		_updateRenderInstanceImp();
	}
}

void VSkyDomeObject::updateRenderInstance()
{
	if (mSceneMgr != VNULL)
	{
		_updateRenderInstanceImp();
	}
}

void VSkyDomeObject::destroyRenderInstance()
{
	if (mSceneMgr != VNULL)
	{
		mSceneMgr->setSkyDome(false, mMaterialName);
		mSceneMgr = VNULL;
	}
}

void VSkyDomeObject::_updateRenderInstanceImp()
{
	assert(mSceneMgr);
	mSceneMgr->setSkyDome(true, mMaterialName, mCurvature, 
		mTiling, mDistance, mDrawFirst, mOrientation,
		mXsegments, mYsegments, mYsegments_keep);
}

VSceneObjectFactory *VSkyDomeObject::getFactory()
{
	static VSkyDomeFactory factory;
	return &factory;
}


