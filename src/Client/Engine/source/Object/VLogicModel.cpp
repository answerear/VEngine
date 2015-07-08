

#include "VLogicModel.h"
#include "VLogicModelManager.h"
#include "VGraphicsSystem.h"
#include "VEngine.h"
#include "VResource.h"
#include "VFlags.h"
#include "VSkill.h"
#include "VEffectManager.h"


using namespace Vision;

const VString VLogicModel::MESH_HINT = "Mesh";
const VString VLogicModel::MATERIAL_HINT = "Material";
const VString VLogicModel::SLOT_CREATE_HINT = "SlotCreate";

const VString VLogicModel::MOVEMENT_TYPE_ATTR = "MovementType";
const VString VLogicModel::DEFAULT_ANIM_NAME_ATTR = "DefaultAnimation";
const VString VLogicModel::BOUNDING_BOX_ATTR = "BoundingBox";
const VString VLogicModel::SCALE_FACTOR_ATTR = "Scale";
const VString VLogicModel::HAIR_MESH_NAME_ATTR = "HairMesh";


VLogicModel::VLogicModel(const VString &name)
	: mEntities()
	, mMaterials()
	, mLocators()
	, mAttributes()
	, mEffects()
	, mFabricColliders()
	, mShadowUncastableMeshes()
    , mName(name)
	, mModelMainNode(VNULL)
	, mParentNode(VNULL)
	, mSkeletonEntity(VNULL)
	, mSkeleton(VNULL)
	, mExternalBoundingBox(VNULL)
	, mFullBoundingBox(VNULL)
	, mLinks(VNULL)
	, mAttachParent(VNULL)
	, mActionListener(VNULL)
	, mCurrentAnimationState(VNULL)
	, mExternalScaleFactor(Ogre::Vector3::UNIT_SCALE)
	, mCreatedEntityCount(0)
	, mShadowCastable(VTRUE)
	, mMainNodeNeedUpdate(VTRUE)
	, mVisible(VTRUE)
	, mIsActive(VTRUE)
	, mIsChild(VFALSE)
{

}

VLogicModel::~VLogicModel()
{

}


VBOOL VLogicModel::addEntity(const VString &name, const VString &meshName, const VString &matName)
{
	VEntityMap::iterator itr = mEntities.find(name);

	if (itr != mEntities.end())
	{
		Ogre::LogManager::getSingleton().logMessage("Logic Model Entity with name '" + name + "' already exists! " +
			"LogicModel::addEntity " + mName);
		return VFALSE;
	}

	std::pair<VEntityMap::iterator, VBOOL> inserted = 
		mEntities.insert(VEntityMap::value_type(name, VEntityValue(meshName, matName)));
	if (!inserted.second)
	{
		Ogre::LogManager::getSingleton().logMessage("Logic Model Entity with name '" + name + "' inserted failed! " +
			"LogicModel::addEntity " + mName);
		return VFALSE;
	}

	return VTRUE;
}

VBOOL VLogicModel::changeEntity(const VString &name, const VString &value)
{
	VEntityMap::iterator itr = mEntities.find(name);

	if (itr == mEntities.end())
	{
		Ogre::LogManager::getSingleton().logMessage("Logic Model Entity with name '" + name + "' dosen't exists! " +
			"LogicModel::changeEntity " + mName);
		return VFALSE;
	}

	VEntityValue &tempValue = itr->second;

	if (tempValue.mEntity)
	{
		//

		//
		_destroyEntity(tempValue.mEntity);
		tempValue.mEntity = VNULL;
	}

	if (!value.empty())
	{
		tempValue.mEntity = _createEntity(value);
	}

	// 
	if (VNULL == mExternalBoundingBox)
	{
		_updateBoundingBox();
	}

	return VTRUE;
}

VBOOL VLogicModel::removeEntity(const VString &name)
{
	VEntityMap::iterator itr = mEntities.find(name);

	if (itr == mEntities.end())
	{
		Ogre::LogManager::getSingleton().logMessage("Logic Model Entity with name '" + name + "' dosen't exists! " +
			"LogicModel::removeEntity " + mName);
		return VFALSE;
	}

	return VTRUE;
}

VBOOL VLogicModel::addMaterial(const VString &name, const VString &entityName, const VString &matName)
{
	VMaterialMap::iterator itr = mMaterials.find(name);

	if (itr != mMaterials.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' already exists! " +
			"LogicModel::addMaterial " + mName );
		return VFALSE;
	}

	std::pair<VMaterialMap::iterator, VBOOL> inserted = 
		mMaterials.insert(VMaterialMap::value_type(name, VMaterialValue(entityName, matName)));
	if (!inserted.second)
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' inserted failed! " +
			"LogicModel::addMaterial " + mName );
		return VFALSE;
	}

	return VTRUE;
}

VBOOL VLogicModel::changeMaterial(const VString &name, const VString &value)
{
	VMaterialMap::iterator itr = mMaterials.find(name);

	if (itr == mMaterials.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' dosen't exists! " +
			"LogicModel::changeMaterial " + mName );

		return VFALSE;
	}

	VBOOL result = VFALSE;
	VMaterialValue &tempValue = itr->second;
	if (_setEntityMaterial(tempValue.mEntityName, value))
	{
		tempValue.mMaterialName = value;
		result = VTRUE;
	}

	return result;
}

VBOOL VLogicModel::addLocator(const VString &name, const VString &boneName, 
	const Ogre::Vector3 &pos, const Ogre::Quaternion &orientation, 
	VBOOL transferable /* = VFALSE */, VBOOL translateFirst /* = VFALSE */)
{
	VLocatorMap::iterator itr = mLocators.find(name);

	if (itr != mLocators.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + name + "' already exists! " +
			"LogicModel::addLocator " + mName );
		return VFALSE;
	}

	std::pair<VLocatorMap::iterator, VBOOL> inserted =
		mLocators.insert(VLocatorMap::value_type(name, VLocatorValue(boneName, pos, orientation, transferable, translateFirst)));
	if (!inserted.second)
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + name + "' inserted failed! " +
			"LogicModel::addLocator " + mName );
		return VFALSE;
	}

	return VTRUE;
}

VBOOL VLogicModel::addSlot(const VString &locatorName, const VString &slotName, 
	const VString &modelName, const VString &attrib)
{
	VLocatorMap::iterator itr = mLocators.find(locatorName);

	if (itr == mLocators.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
			"LogicModel::addSlot " + mName );
		return VFALSE;
	}

	VSlotMap &slots = itr->second.mSlots;
	std::pair<VSlotMap::iterator, VBOOL> inserted =
		slots.insert(VSlotMap::value_type(slotName, VSlotValue(modelName)));
	if (!inserted.second)
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Slot with name '" + slotName + "' inserted failed! " +
			"LogicModel::addSlot " + mName );
		return VFALSE;
	}

	return VTRUE;
}

VBOOL VLogicModel::changeSlotAttrib(const VString &name, const VString &value)
{
	for (VLocatorMap::iterator itr = mLocators.begin(); itr != mLocators.end(); ++itr)
	{
		VLocatorValue &locator = itr->second;
		VSlotMap::iterator i = locator.mSlots.find(name);

		if (i != locator.mSlots.end())
		{
			VSlotValue &slot = i->second;

			if (value.empty())
			{
				if (slot.mModel != VNULL)
				{
					VLOGIC_MODEL_MANAGER.destroyLogicModel(slot.mModel);
				}

				slot.mModel = VNULL;
				slot.mModelName = value;
				return VTRUE;
			}
			else
			{
				slot.mModelName = value;
				return _createModelInSlot(slot, locator);
			}
		}
	}

	return VFALSE;
}

VBOOL VLogicModel::addAttribute(const VString &name, const VString &value, const VString &hint)
{
	VAttribMap::iterator itr = mAttributes.find(name);

	if (itr != mAttributes.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' already exists! " +
			"LogicModel::addAttribute " + mName );
		return VFALSE;
	}

	std::pair<VAttribMap::iterator, VBOOL> inserted = 
		mAttributes.insert(VAttribMap::value_type(name, VAttribValue(value, hint)));
	if (!inserted.second)
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' inserted failed! " +
			"LogicModel::addAttribute " + mName );
		return VFALSE;
	}

	_processExtraAttributes(name, value);

	return VTRUE;
}

VBOOL VLogicModel::changeAttribute(const VString &name, const VString &value)
{
	VAttribMap::iterator itr = mAttributes.find(name);

	if (itr == mAttributes.end())
	{
		// 没有符合的属性，报错
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' doesn't exists! " +
			"VLogicModel::changeAttribute " + mName );
		return VFALSE;
	}

	VAttribValue &tempValue = itr->second;

	if (tempValue.mValue == value)
	{
		// 与原来的值相同，不需要重新设置，直接返回
		return VFALSE;
	}

	VBOOL result = VFALSE;

	if (tempValue.mHint == MESH_HINT)
	{
		// 修改的属性是mesh
		if (changeEntity(name, value))
		{
			tempValue.mValue = value;
			result = VTRUE;
		}
	}
	else if (tempValue.mHint == MATERIAL_HINT)
	{
		// 修改的属性是material
		if (changeMaterial(name, value))
		{
			tempValue.mValue = value;
			result = VTRUE;
		}
	}
	else if (tempValue.mHint == SLOT_CREATE_HINT)
	{
		// 修改的属性是挂接点
		if (changeSlotAttrib(name, value))
		{
			tempValue.mValue = value;
			result = VTRUE;
		}
	}
	else
	{
		// 处理其他类型的属性
		_processExtraAttributes(name, value);
	}

	return result;
}

VBOOL VLogicModel::hasAttribute(const VString &name) const
{
	VAttribMap::const_iterator itr = mAttributes.find(name);

	return (itr != mAttributes.end());
}

VBOOL VLogicModel::addEffect(const VString &name, const VString &effectName, const VString &locatorName,
	VBOOL translateAll /* = VFALSE */, const Ogre::ColourValue &color /* = Ogre::ColourValue::White */)
{
	VEffectMap::iterator itr = mEffects.find(name);

	if (itr != mEffects.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Effect with name '" + name + "' already exists! " +
			"LogicModel::addEffectInfo " + mName );

		return VFALSE;
	}

	std::pair<VEffectMap::iterator, VBOOL> inserted =
		mEffects.insert(VEffectMap::value_type(name, VEffectValue(effectName, locatorName, translateAll, color)));
	if (!inserted.second)
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Effect with name '" + name + "' inserted failed! " +
			"LogicModel::addEffectInfo " + mName );

		return VFALSE;
	}

	return VTRUE;
}

VBOOL VLogicModel::addShadowUncastable(const VString &meshName)
{
	VShadowUncastableMeshes::iterator itr = mShadowUncastableMeshes.begin();

	while (itr != mShadowUncastableMeshes.end())
	{
		if (*itr == meshName)
		{
			break;
		}

		++itr;
	}

	if (itr != mShadowUncastableMeshes.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model ShadowUncastable with name '" + 
			meshName + "' already exists!  " + "LogicModel::addShadowUncastable " + mName );

		return VFALSE;
	}

	mShadowUncastableMeshes.push_back(meshName);
	return VTRUE;
}

VBOOL VLogicModel::isShadowUncastable(const VString &meshName)
{
	VShadowUncastableMeshes::iterator itr = mShadowUncastableMeshes.begin();

	// 查找是否有重名的
	while (itr != mShadowUncastableMeshes.end())
	{
		if (*itr == meshName)
			return VTRUE;

		++itr;
	}

	return VFALSE;
}

VBOOL VLogicModel::addFabricCollider(const VString &colliderName, const VString &locatorName, 
	VFabricColliderType colliderType, const VString &colliderInfo)
{
	VFabricColliderMap::iterator itr = mFabricColliders.find(colliderName);
	if (itr != mFabricColliders.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Fabric Collider with name '"
			+ colliderName + "' already exists! " +
			"LogicModel::addFabricCollider " + mName );
		return VFALSE;
	}

	std::pair<VFabricColliderMap::iterator, VBOOL> inserted =
		mFabricColliders.insert(VFabricColliderMap::value_type(colliderName, VFabricColliderValue(locatorName, colliderType, colliderInfo)));

	if (VFALSE == inserted.second)
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Fabric Collider with name '"
			+ colliderName + "' inserted failed! " +
			"LogicModel::addFabricCollider " + mName );
		return VFALSE;
	}

	return VTRUE;
}

void VLogicModel::init()
{
	_createMainSceneNode();

	if (!mExternalBoundingBoxValue.empty())
	{
		_createBoundingBoxFromExternal();
	}

	_createEntities();

	_setEntityMaterials();

	_createLocatorNode();

	_createEffects();
}

void VLogicModel::createSkill(const VString &skillName, VBOOL loop, VBOOL anim, const Real &delay)
{
	_destroyCurrentSkill();

	try
	{
		if (!anim)
		{
			mCurrentSkill = VEFFECT_MANAGER.createSkill(skillName);

			if (VNULL == mCurrentSkill)
			{
				_changeAnimation(skillName, delay);
			}
			else
			{
				_changeAnimation(mCurrentSkill->getAnimationName(), delay);
			}
		}
		else
		{
			_changeAnimation(skillName, delay);
		}
	}
	catch (const Ogre::Exception &e)
	{
		if (e.getNumber() ==  Ogre::Exception::ERR_RT_ASSERTION_FAILED)
		{
			Ogre::LogManager::getSingleton().logMessage("Animation : " + skillName + '\n' + "_getAnimationState Failed!");
		}
	}

	setAnimationLoop(loop);
}

void VLogicModel::setParentSceneNode(Ogre::Node *parentNode)
{
	if (mModelMainNode != VNULL && mModelMainNode->getParentSceneNode())
	{
		mModelMainNode->getParentSceneNode()->removeChild(mModelMainNode->getName());
	}

	if (parentNode != VNULL)
	{
		parentNode->addChild(mModelMainNode);
	}

	mParentNode = parentNode;
}

void VLogicModel::setVisibilityFlags(VUINT32 flags)
{
	if (mVisibilityFlags != flags)
	{
		// 先设置成员变量
		mVisibilityFlags = flags;

		// 设置自己的entity的visibility flags
		for (VEntityMap::iterator itr = mEntities.begin(); itr != mEntities.end(); ++itr)
		{
			Ogre::Entity *entity = itr->second.mEntity;
			if (entity != VNULL)
			{
				entity->setVisibilityFlags(flags);
			}
		}

		// 设置特效的visibility flags

		// 设置fabric的visibility flags

		// 设置挂接点上model的visibility flags
		for (VLocatorMap::iterator itr = mLocators.begin(); itr != mLocators.end(); ++itr)
		{
			VLocatorValue &locator = itr->second;

			for (VSlotMap::iterator i = locator.mSlots.begin(); i != locator.mSlots.end(); ++i)
			{
				VSlotValue &slot = i->second;
				if (slot.mModel != VNULL)
				{
					slot.mModel->setVisibilityFlags(flags);
				}
			}
		}
	}
}

void VLogicModel::setVisible(VBOOL visible)
{
	if (mModelMainNode != VNULL)
	{
		mModelMainNode->setVisible(visible);

		Ogre::Node *parentNode = mModelMainNode->getParent();

		if (visible)
		{
			if (parentNode == VNULL)
			{
				mParentNode->addChild(mModelMainNode);
			}
			else
			{
				if (mParentNode != parentNode)
				{

				}
			}
		}
		else
		{
			if (parentNode != VNULL)
			{
				if (mParentNode != parentNode)
				{

				}

				parentNode->removeChild(mModelMainNode);
			}
			else
			{
				if (mVisible != visible)
				{

				}
			}
		}

		mVisible = visible;

		// 特效也需要设置可见性

		// locator上挂接的model也需要设置可见性
		for (VLocatorMap::iterator itr = mLocators.begin(); itr != mLocators.end(); ++itr)
		{
			VLocatorValue &locator = itr->second;

			for (VSlotMap::iterator i = locator.mSlots.begin(); i != locator.mSlots.end(); ++i)
			{
				VSlotValue &slot = i->second;
				if (slot.mModel != VNULL)
				{
					slot.mModel->setVisible(visible);
				}
			}
		}

		// 设置技能特效可见性
		if (!visible)
		{

		}
	}
}

VBOOL VLogicModel::attachModel(const VString &locatorName, VLogicModel *model)
{
	assert(model != VNULL);

	VLocatorMap::iterator itr = mLocators.find(locatorName);
	if (itr == mLocators.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
			"VLogicModel::attachModel " + mName );

		return VFALSE;
	}

	VAttachedModelOriginSceneNodes::iterator it = mAttachedModelOriginSceneNodes.find(model);
	if (it != mAttachedModelOriginSceneNodes.end())
	{
		it->second = mParentNode;
	}
	else
	{
		std::pair<VAttachedModelOriginSceneNodes::iterator, VBOOL> inserted =
			mAttachedModelOriginSceneNodes.insert(VAttachedModelOriginSceneNodes::value_type(model, mParentNode));
		assert(inserted.second);
	}

	Ogre::SceneNode *locatorNode = _getLocatorSceneNode(itr->second);

	model->setParentSceneNode(locatorNode);
	// 挂接上去的model需要重置位置和方向
	model->_resetPosition();

	if (VNULL == mExternalBoundingBox)
	{
		
	}

	// 告诉被挂接模型谁挂接上了
	model->notifyAttach(this);

	return VTRUE;
}

VBOOL VLogicModel::detachModel(VLogicModel *model)
{
	return VTRUE;
}

void VLogicModel::setPosition(const Vector3 &pos)
{

}

void VLogicModel::setOrientation(const Quaternion &orientation)
{

}

void VLogicModel::setScale(const Vector3 &factor)
{
	if (!mExternalScaleFactor.positionEquals(factor))
	{
		mExternalScaleFactor = factor;

		if (!isChildModel() && mModelMainNode != VNULL)
		{
			mModelMainNode->setScale(mExternalScaleFactor);
		}
	}
}

const Ogre::AxisAlignedBox &VLogicModel::getBoundingBox() const
{
	if (mExternalBoundingBox != VNULL)
	{
		return *mExternalBoundingBox;
	}

	return _getFullBoundingBox();
}

void VLogicModel::setAnimationLoop(VBOOL loop)
{
	mIsAnimationLoop = loop;

	if (mCurrentAnimationState != VNULL)
	{
		mCurrentAnimationState->setLoop(loop);
	}
}

void VLogicModel::execute(Real dt)
{
	if (!mVisible && mVisibilityFlags != OVF_GUI_ELEMENTS)
		return;

	if (mMainNodeNeedUpdate)
	{
		mModelMainNode->_update(true, false);

		mMainNodeNeedUpdate = VFALSE;
	}

	if (mCurrentAnimationState != VNULL)
	{
		Ogre::Real totalLength = 0.0f;
		Ogre::Real lastTime = 0.0f;
		Ogre::Real curTime = 0.0f;

		totalLength = mCurrentAnimationState->getLength();

		if (!Ogre::Math::RealEqual(0.0f, totalLength))
		{
			lastTime = mCurrentAnimationState->getTimePosition() / totalLength;
			mCurrentAnimationState->addTime(dt);
			curTime = mCurrentAnimationState->getTimePosition() / totalLength;
		}

		VBOOL isContinuous = _handleAnimationEvent(lastTime, curTime);

		_updateLocatorPos();

		if (isContinuous)
		{
			
		}
	}

	_executeLocatorModel(dt);

	_executeAttachedModel(dt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//								Protected methods
///////////////////////////////////////////////////////////////////////////////////////////////////

void VLogicModel::_createMainSceneNode()
{
	VGraphicsSystem *gfx = VENGINE.getGfxSystem();
	Ogre::SceneManager *sceneMgr = gfx->getSceneManager();
	mModelMainNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	mParentNode = mModelMainNode->getParent();
	mModelMainNode->setScale(mExternalScaleFactor);
}

void VLogicModel::_createEntities()
{
	VEntityMap::iterator itr;
	for (itr = mEntities.begin(); itr != mEntities.end(); ++itr)
	{
		Ogre::Entity *entity = VNULL;
		VEntityValue &entValue = itr->second;

		VString meshName = entValue.mMeshName;
		VString materialName = entValue.mMaterialName;

		try
		{
			Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(meshName);
		}
		catch (const Ogre::Exception &e)
		{
			Ogre::LogManager::getSingleton().logMessage(e.getDescription());
			continue;
		}

		if (!meshName.empty())
		{
			entValue.mEntity = _createEntity(meshName);
			if (entValue.mEntity && !materialName.empty())
			{
				_setEntityMaterial(itr->first, entValue.mMaterialName);
			}
		}
	}
}

Ogre::Entity *VLogicModel::_createEntity(const VString &meshName)
{
	Ogre::Entity *entity = VNULL;

	loadMesh(meshName, Ogre::StringUtil::BLANK, Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);

	VString entityName = mModelMainNode->getName() + "_Entity" + "_" + meshName +
		Ogre::StringConverter::toString(mCreatedEntityCount++);

	VGraphicsSystem *system = VENGINE.getGfxSystem();
	entity = system->getSceneManager()->createEntity(entityName, meshName);

	mModelMainNode->attachObject(entity);

	if (!entity->getMesh()->getSkeleton().isNull())
	{
		if (VNULL == mSkeletonEntity)
		{
			const Ogre::MeshPtr &originMesh = entity->getMesh();
			assert(!originMesh.isNull());
			const Ogre::SkeletonPtr &originSke = originMesh->getSkeleton();
			assert(!originSke.isNull());

			_createSkeletonEntity(originSke);
		}

		assert(mSkeletonEntity);

		entity->shareSkeletonInstanceWith(mSkeletonEntity);
	}

// 	_convertEntityToCharacterMaterial(entity);

	if (mSkeletonEntity && !mSkeleton)
	{
		mSkeleton = mSkeletonEntity->getSkeleton();
		mSkeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
	}

	entity->setCastShadows(mShadowCastable && !isShadowUncastable(meshName));

	return entity;
}

void VLogicModel::_destroyEntity(Ogre::Entity *entity)
{
	assert(entity);
	VString entityName = entity->getName();

	entity->getParentSceneNode()->detachObject(entityName);
	VGraphicsSystem *system = VENGINE.getGfxSystem();
	system->getSceneManager()->destroyEntity(entityName);
}

void VLogicModel::_createSkeletonEntity(const Ogre::SkeletonPtr& skeleton)
{
	mSkeletonEntity = VLogicModelManager::instance()._createSkeletalEntity(skeleton);
	mSkeletonEntity->setQueryFlags(0);
	mModelMainNode->attachObject(mSkeletonEntity);
}

void VLogicModel::_destroySkeletonEntity()
{
	if (mSkeletonEntity)
	{
		Ogre::SceneNode *node = mSkeletonEntity->getParentSceneNode();
		if (node && node == mSkeletonEntity->getParentNode())
		{
			node->detachObject(mSkeletonEntity);
		}

		VLogicModelManager::instance()._destroySkeletalEntity(mSkeletonEntity);
		mSkeletonEntity = VNULL;
	}
}


void VLogicModel::_setEntityMaterials()
{
	VMaterialMap::iterator itr;
	for (itr = mMaterials.begin(); itr != mMaterials.end(); ++itr )
	{
		VMaterialValue &matValue = itr->second;
		_setEntityMaterial(matValue.mEntityName, matValue.mMaterialName);
	}
}

VBOOL VLogicModel::_setEntityMaterial(const VString &entityName, const VString &matName)
{
	if (matName.empty())
	{
		return VTRUE;
	}
	else
	{
		VEntityMap::iterator itr = mEntities.find(entityName);

		if (itr == mEntities.end())
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + entityName + "' dosen't exists! " +
				"LogicModel::_setEntityMaterial " + mName );
			return VFALSE;
		}

		VEntityValue &entValue = itr->second;
		Ogre::Entity *entity = entValue.mEntity;
		assert(entity);

		if (matName.find(";") != VString::npos)
		{
			Ogre::StringVector matNames = Ogre::StringUtil::split(matName, ";");
			assert(matName.size() > 1);

			for (VUINT32 i = 0; i < entity->getNumSubEntities(); ++i)
			{
				Ogre::SubEntity *subEntity = entity->getSubEntity(i);
				assert(subEntity);

				VString subMatName;

				if (i < matNames.size())
				{
					subMatName = matNames[i];
				}
				else
				{
					subMatName = matNames[0];
				}

				const Ogre::MaterialPtr subMat = Ogre::MaterialManager::getSingleton().getByName(subMatName);
				if (!subMat.isNull())
				{
					subEntity->setMaterialName(subMatName);
				}
			}
		}
		else
		{
			const Ogre::MaterialPtr entityMat = Ogre::MaterialManager::getSingleton().getByName(matName);
			if (!entityMat.isNull())
			{
				entity->setMaterialName(matName);
			}
		}
	}

	return VTRUE;
}

void VLogicModel::_createLocatorNode()
{
	VLocatorMap::iterator itr;
	for (itr = mLocators.begin(); itr != mLocators.end(); ++itr)
	{
		VLocatorValue &locator = itr->second;
		VSlotMap::iterator it;
		for (it = locator.mSlots.begin(); it != locator.mSlots.end(); ++it)
		{
			VSlotValue &slot = it->second;
			if (!slot.mModelName.empty())
			{
				_createModelInSlot(slot, locator);
			}
		}
	}
}

void VLogicModel::_updateLocatorPos()
{
	for (VLocatorMap::iterator itr = mLocators.begin(); itr != mLocators.end(); ++itr)
	{
		VLocatorValue &tempValue = itr->second;

		// 只更新创建出来的（已经使用的）locator node
		if (tempValue.mLocatorNode != VNULL)
		{
			if (!tempValue.mBoneName.empty())
			{
				Ogre::Bone *bone = VNULL;
				try
				{
					bone = mSkeleton->getBone(tempValue.mBoneName);
				}
				catch (const Ogre::Exception &e)
				{
					Ogre::LogManager::getSingleton().logMessage("LogicModel::_updateLocatorPos " + mName + e.getDescription());
					continue;
				}

				assert(bone != VNULL);

				Ogre::SceneNode *locatorNode = tempValue.mLocatorNode;
				assert(locatorNode != VNULL);

				locatorNode->setPosition(bone->_getDerivedPosition());
				locatorNode->setOrientation(bone->_getDerivedOrientation());

				if (tempValue.mTranslateFirst)
				{
					// 先平移，再旋转
					locatorNode->translate(tempValue.mPosition, Ogre::Node::TS_LOCAL);
					locatorNode->rotate(tempValue.mOrientation, Ogre::Node::TS_LOCAL);
				}
				else
				{
					// 先旋转，再平移
					locatorNode->rotate(tempValue.mOrientation, Ogre::Node::TS_LOCAL);
					locatorNode->translate(tempValue.mPosition, Ogre::Node::TS_LOCAL);
				}
			}
		}
	}

	if (mLocators.size() != 0)
	{
		mModelMainNode->_update(true, false);
	}
}

void VLogicModel::_executeLocatorModel(const Ogre::Real &dt)
{
	for (VLocatorMap::iterator itr = mLocators.begin(); itr != mLocators.end(); ++itr)
	{
		VLocatorValue &locator = itr->second;

		for (VSlotMap::iterator i = locator.mSlots.begin(); i != locator.mSlots.end(); ++i)
		{
			VSlotValue &slot = i->second;
			if (slot.mModel != VNULL)
			{
				slot.mModel->execute(dt);
			}
		}
	}
}

void VLogicModel::_createEffects()
{

}

VBOOL VLogicModel::_changeAnimation(const VString &animName, const Real &delay)
{
	VBOOL result = VFALSE;

	if (animName.empty())
	{
		mCurrentAnimationName = "";
		mCurrentAnimationState = VNULL;
		result = VTRUE;
	}
	else
	{
		try
		{
			if (mSkeletonEntity != VNULL)
			{
				mCurrentAnimationState = _getAnimationState(animName);
				mSkeleton->getAnimation(animName)->setInterpolationMode(VLOGIC_MODEL_MANAGER.getAnimationInterpolationMode());
			}
			else
			{
				Ogre::LogManager::getSingleton().logMessage( "Logic Model : " + mName + 
					" Skeleton Entity is NULL, Maybe the skeleton file is lost!" +
					"LogicModel::_changeAnimation");
			}
		}
		catch (const Ogre::Exception &e)
		{
			Ogre::LogManager::getSingleton().logMessage("Model : " + mName + '\n' + "_getAnimationState Failed!");
			mCurrentAnimationState = NULL;

			OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
					"Resource was incorrectness due incaution producer. "
					"Full error description: " + e.getFullDescription(),
					"LogicModel::_changeAnimation");
		}

		if (mCurrentAnimationState != VNULL)
		{
			mCurrentAnimationState->setEnabled(VTRUE);
			mCurrentAnimationState->setTimePosition(0.0f);
			mCurrentAnimationState->setWeight(1);
			mCurrentAnimationName = animName;
			result = VTRUE;
		}
		else
		{
			mCurrentAnimationName = "";
			result = VFALSE;
		}
	}

	return result;
}

Ogre::AnimationState *VLogicModel::_getAnimationState(const VString &animName) const
{
	// 先判断当前skeleton有没有这个animation
	// 这里不用判断mSkeletonEntity->getAllAnimationStates()不为空，因为能进到
	// 这个函数，说明mSkeletonEntity不为空，所以肯定是带骨骼的
	if (!mSkeletonEntity->getAllAnimationStates()->hasAnimationState(animName))
	{
		if (!mSkeletonEntity->getSkeleton()->hasAnimation(animName))
		{
			// 如果没有，就从animation skeleton link中获取要读取的skeleton的名称
			mSkeletonEntity->getSkeleton()->addLinkedSkeletonAnimationSource(
				VLOGIC_MODEL_MANAGER.getSkeletonFromAnimationName(mName, animName));
		}

		mSkeletonEntity->refreshAvailableAnimationState();
	}

	return mSkeletonEntity->getAnimationState(animName);
}

void VLogicModel::_destroyCurrentSkill()
{
	if (mCurrentSkill != VNULL)
	{
		VEFFECT_MANAGER.destroySkill(mCurrentSkill);
		mCurrentSkill = VNULL;
	}
}

VBOOL VLogicModel::_createModelInSlot(VSlotValue &slot, VLocatorValue &locator)
{
	if (slot.mModelName.empty())
	{
		return VFALSE;
	}

	VBOOL result = VFALSE;
	VLogicModel *model = VLogicModelManager::instance().createLogicModel(slot.mModelName);
	if (model != VNULL)
	{
		// 删除原来挂的model
		if (slot.mModel != VNULL)
		{
			VLogicModelManager::instance().destroyLogicModel(slot.mModel);
		}

		slot.mModel = model;
		Ogre::SceneNode *locatorNode = _getLocatorSceneNode(locator);

		// 设置父节点为挂接节点
		model->setParentSceneNode(locatorNode);
		// 设置子模型标记
		model->setChildModel(VTRUE);

		// 设置挂接上的模型的可见性标记
		model->setVisibilityFlags(mVisibilityFlags);
		// 设置挂接模型的是否可见
		model->setVisible(mVisible);

		model->setScale(mExternalScaleFactor);

		result = VTRUE;
	}

	return result;
}

void VLogicModel::_processExtraAttributes(const VString &name, const VString &value)
{
	// 处理其他类型属性
	if (name == MOVEMENT_TYPE_ATTR)
	{
		_setMovementType(value);
	}
	else if (name == DEFAULT_ANIM_NAME_ATTR)
	{
		_setDefaultAnimationName(value);
	}
	else if (name == BOUNDING_BOX_ATTR)
	{
		mExternalBoundingBoxValue = value;
	}
	else if (name == SCALE_FACTOR_ATTR)
	{
		_setScaleFactor(value);
	}
}

Ogre::SceneNode *VLogicModel::_getLocatorSceneNode(VLocatorValue &locator)
{
	if (locator.mLocatorNode == VNULL)
	{
		locator.mLocatorNode = _createLocatorSceneNode(locator.mPosition, locator.mOrientation);
	}

	return locator.mLocatorNode;
}

Ogre::SceneNode *VLogicModel::_createLocatorSceneNode(const Ogre::Vector3 &pos, const Ogre::Quaternion &ori)
{
	static VUINT32 count = 0;
	Ogre::StringUtil::StrStreamType str;
	str<<"Locator_"<<std::setw(5)<<std::setfill('0')<<count++<<std::ends;
	Ogre::SceneNode *locatorNode = mModelMainNode->createChildSceneNode(str.str());
	locatorNode->translate(pos);
	locatorNode->setOrientation(ori);
	return locatorNode;
}

void VLogicModel::_executeAttachedModel(const Ogre::Real &dt)
{
	VAttachedModelOriginSceneNodes::const_iterator itr = mAttachedModelOriginSceneNodes.begin();

	while (itr != mAttachedModelOriginSceneNodes.end())
	{
		if (itr->first)
		{
			itr->first->execute(dt);
		}

		++itr;
	}
}

void VLogicModel::_updateBoundingBox()
{
	if (VNULL == mFullBoundingBox)
	{
		mFullBoundingBox = new Ogre::AxisAlignedBox();
	}

	mFullBoundingBox->setNull();

	for (VEntityMap::iterator itr = mEntities.begin(); itr != mEntities.end(); ++itr)
	{
		Ogre::Entity *entity = itr->second.mEntity;
		if (entity != VNULL)
		{
			mFullBoundingBox->merge(entity->getBoundingBox());
		}
	}

	VAttachedModelOriginSceneNodes::const_iterator itr = mAttachedModelOriginSceneNodes.begin();
	while (itr != mAttachedModelOriginSceneNodes.end())
	{
		if (itr->first)
		{
			mFullBoundingBox->merge(itr->first->_getFullBoundingBox());
		}

		++itr;
	}
}

void VLogicModel::_createBoundingBoxFromExternal()
{
	Ogre::StringVector values = Ogre::StringUtil::split(mExternalBoundingBoxValue, " ");

	if (values.size() != 3)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
			"the bounding box parameter count must be 3!",
			"LogicModel::_createBoundingBoxFromExternal"); 

		return;
	}

	Ogre::Real x = Ogre::StringConverter::parseReal(values[0]);
	Ogre::Real y = Ogre::StringConverter::parseReal(values[1]);
	Ogre::Real z = Ogre::StringConverter::parseReal(values[2]);

	Ogre::Vector3 ptMin;
	ptMin.x = -x / 2;
	ptMin.y = 0.0f;
	ptMin.z = -z / 2;

	Ogre::Vector3 ptMax;
	ptMax.x = x / 2;
	ptMax.y = y;
	ptMax.z = z / 2;

	mExternalBoundingBox = new Ogre::AxisAlignedBox(ptMin, ptMax);
}

void VLogicModel::_resetPosition()
{
	mModelPosition = Ogre::Vector3::ZERO;
	mModelOrientation = Ogre::Quaternion::IDENTITY;

	mModelMainNode->setPosition(mModelPosition);
	mModelMainNode->setOrientation(mModelOrientation);
}

void VLogicModel::_setMovementType(const VString &value)
{
	if (value == "Normal")
	{
		mMovementType = MT_NORMAL;
	}
	else if (value == "RotateX")
	{
		mMovementType = MT_ROTATE_X_AXIS;
	}
	else if (value == "RotateXZ")
	{
		mMovementType = MT_ROTATE_X_Z_AXIS;
	}
	else
	{
		Ogre::LogManager::getSingleton().logMessage( "Wrong Movement Type '" + value + "' ! " +
			"LogicModel::_setMovementType " + mName );
	}
}

void VLogicModel::_setScaleFactor(const VString &value)
{
	Ogre::StringVector values = Ogre::StringUtil::split(value, " ");

	if (values.size() != 3)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
			"the scale factor parameter count must be 3!",
			"LogicModel::_setScaleFactor");
		return;
	}

	setScale(Ogre::StringConverter::parseVector3(value));
}

VBOOL VLogicModel::_handleAnimationEvent(const Ogre::Real &lastTime, const Ogre::Real &curTime)
{
	VBOOL bContinue = VTRUE;

	if (curTime <= lastTime)
	{
		if (!mIsAnimationLoop)
		{
			// 不循环播放动画
			bContinue = VFALSE;
			if (mActionListener != VNULL)
			{
				mActionListener->handleAnimationEvent(mCurrentAnimationName, VTAG_ANIM_END);
				mActionListener->handleAnimationEvent(mCurrentAnimationName, VTAG_ANIM_FINISH);
			}
		}
		else
		{
			// 循环播放动画
			bContinue = VTRUE;
			if (mActionListener != VNULL)
			{
				mActionListener->handleAnimationEvent(mCurrentAnimationName, VTAG_ANIM_END);
			}
		}
	}

	return bContinue;
}
