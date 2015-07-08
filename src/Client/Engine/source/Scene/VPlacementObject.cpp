

#include "VPlacementObject.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


/**************************************************************************************************
 *								Property Command of VPlacementObject
 *************************************************************************************************/

class VPlacementPositionCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VPlacementObject *object = static_cast<const VPlacementObject*>(source);
		return object->mTransform.position;
	}

	VString doGetAsString(const void *source) const
	{
		const VPlacementObject *object = static_cast<const VPlacementObject*>(source);
		return Ogre::StringConverter::toString(object->mTransform.position);
	}

	void doSet(void *target, const Variant &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		object->mTransform.position = VariantCast<Ogre::Vector3>(value);
		if (object->getSceneNode())
			object->getSceneNode()->setPosition(object->mTransform.position);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		object->mTransform.position = Ogre::StringConverter::parseVector3(value);
		if (object->getSceneNode())
			object->getSceneNode()->setPosition(object->mTransform.position);
	}
};

class VPlacementOrientationCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VPlacementObject *object = static_cast<const VPlacementObject*>(source);
		return object->mTransform.orientation;
	}

	Ogre::String doGetAsString(const void *source) const
	{
		const VPlacementObject *object = static_cast<const VPlacementObject*>(source);
		return Ogre::StringConverter::toString(object->mTransform.orientation);
	}

	void doSet(void *target, const Variant &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		object->mTransform.orientation = VariantCast<Ogre::Quaternion>(value);
		if (object->getSceneNode())
			object->getSceneNode()->setOrientation(object->mTransform.orientation);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		object->mTransform.orientation = Ogre::StringConverter::parseQuaternion(value);
		if (object->getSceneNode())
			object->getSceneNode()->setOrientation(object->mTransform.orientation);
	}
};

// 欧拉角到四元数的转换步骤： 欧拉角->矩阵->四元数
// 四元数到欧拉角的转换步骤： 四元数->矩阵->欧拉角
class VPlacementOrientationXYZCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VPlacementObject* object = static_cast<const VPlacementObject*>(source);
		Ogre::Radian radianX, radianY, radianZ;
		Ogre::Matrix3 mtemp;
		object->mTransform.orientation.ToRotationMatrix(mtemp);
		mtemp.ToEulerAnglesXYZ(radianX, radianY, radianZ);
		Ogre::Vector3 vectorXYZ(radianX.valueDegrees(), radianY.valueDegrees(), radianZ.valueDegrees());
		return vectorXYZ;
	}

	Ogre::String doGetAsString(const void *source) const
	{
		const VPlacementObject* object = static_cast<const VPlacementObject*>(source);
		Ogre::Radian radianX, radianY, radianZ;
		Ogre::Matrix3 mtemp;
		object->mTransform.orientation.ToRotationMatrix(mtemp);
		mtemp.ToEulerAnglesXYZ(radianX, radianY, radianZ);
		Ogre::Vector3 vectorXYZ(radianX.valueDegrees(), radianY.valueDegrees(), radianZ.valueDegrees());
		return Ogre::StringConverter::toString(vectorXYZ);
	}

	void doSet(void *target, const Variant &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		Ogre::Vector3 vectorOrient = VariantCast<Ogre::Vector3>(value);
		Ogre::Degree degreeX(vectorOrient.x), degreeY(vectorOrient.y), degreeZ(vectorOrient.z);
		Ogre::Radian radianX(degreeX), radianY(degreeY), radianZ(degreeZ);
		Ogre::Matrix3 mtemp;
		mtemp.FromEulerAnglesXYZ(radianX, radianY, radianZ);
		object->mTransform.orientation.FromRotationMatrix(mtemp);
		if (object->getSceneNode())
			object->getSceneNode()->setOrientation(object->mTransform.orientation);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		Ogre::Vector3 vectorOrient = Ogre::StringConverter::parseVector3(value);
		Ogre::Degree degreeX(vectorOrient.x), degreeY(vectorOrient.y), degreeZ(vectorOrient.z);
		Ogre::Radian radianX(degreeX), radianY(degreeY), radianZ(degreeZ);
		Ogre::Matrix3 mtemp;
		mtemp.FromEulerAnglesXYZ(radianX, radianY, radianZ);
		object->mTransform.orientation.FromRotationMatrix(mtemp);
		if (object->getSceneNode())
			object->getSceneNode()->setOrientation(object->mTransform.orientation);
	}
};

class VPlacementScaleCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VPlacementObject *object = static_cast<const VPlacementObject*>(source);
		return object->mTransform.scale;
	}

	VString doGetAsString(const void *source) const
	{
		const VPlacementObject *object = static_cast<const VPlacementObject*>(source);
		return Ogre::StringConverter::toString(object->mTransform.scale);
	}

	void doSet(void *target, const Variant &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		object->mTransform.scale = VariantCast<Ogre::Vector3>(value);
		if (object->getSceneNode())
			object->getSceneNode()->setScale(object->mTransform.scale);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		object->mTransform.scale = Ogre::StringConverter::parseVector3(value);
		if (object->getSceneNode())
			object->getSceneNode()->setScale(object->mTransform.scale);
	}
};

class VPlacementCreateLevelCmd : public VPropertyCommand
{
public:
	Variant doGet(const void *source) const
	{
		const VPlacementObject *object = static_cast<const VPlacementObject*>(source);
		return object->getCreateLevel();
	}

	VString doGetAsString(const void *source) const
	{
		const VPlacementObject *object = static_cast<const VPlacementObject*>(source);
		return Ogre::StringConverter::toString(object->getCreateLevel());
	}

	void doSet(void *target, const Variant &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		Ogre::Real createLevel = VariantCast<Ogre::Real>(value);
		object->setCreateLevel(createLevel);
	}

	void doSetAsString(void *target, const VString &value)
	{
		VPlacementObject *object = static_cast<VPlacementObject*>(target);
		Ogre::Real createLevel = Ogre::StringConverter::parseReal(value);
		object->setCreateLevel(createLevel);
	}
};

/**************************************************************************************************
 *									class VPlacementObject
 *************************************************************************************************/

VPlacementObject::VPlacementObject()
	: VSceneObject()
	, mSceneNode(VNULL)
	, mCreateLevel(50.0f)
{

}

VPlacementObject::~VPlacementObject()
{
	if (mSceneNode)
	{
		mSceneNode->getCreator()->destroySceneNode(mSceneNode);
		mSceneNode = VNULL;
	}
}

void VPlacementObject::createRenderInstance()
{
	assert(VNULL == mSceneNode);

	VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();
	assert(pGfxSystem);
	mSceneNode = pGfxSystem->getBaseSceneNode()->createChildSceneNode(mTransform.position, mTransform.orientation);
	mSceneNode->setScale(mTransform.scale);
	mSceneNode->setInitialState();
}

void VPlacementObject::destroyRenderInstance()
{
	if (VNULL != mSceneNode)
	{
		mSceneNode->getCreator()->destroySceneNode(mSceneNode);
		mSceneNode = VNULL;
	}
}

void VPlacementObject::setCreateLevel(Ogre::Real level)
{
	if (mCreateLevel != level)
	{
		mCreateLevel = level;
		_updateCreateLevel();
	}
}

void VPlacementObject::_addBaseProperty(void)
{
	static VPlacementPositionCmd positionCmd;
	defineProperty(
		"position",
		"The position of this object.",
		"Position",
		&positionCmd,
		0);

	static VPlacementOrientationCmd orientationCmd;
	defineProperty(
		"orientation",
		"The orientation of this object.",
		"Orientation",
		&orientationCmd,
		0);

	static VPlacementOrientationXYZCmd orientationXYZCmd;
	defineProperty(
		"orientationXYZ",
		"The orientationXYZ of this object.",
		"OrientationXYZ",
		&orientationXYZCmd,
		0);

	static VPlacementScaleCmd scaleCmd;
	defineProperty(
		"scale",
		"The scaling factor applied to this object.",
		"Scale",
		&scaleCmd,
		0);

	static VPlacementCreateLevelCmd createLevelCmd;
	defineProperty(
		"create level",
		"The create level of this object.",
		"Ogre::Real",
		&createLevelCmd,
		0);
}

void VPlacementObject::_updateCreateLevel()
{
}
