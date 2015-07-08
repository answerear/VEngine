

#ifndef __VISION_LOGIC_MODEL_H__
#define __VISION_LOGIC_MODEL_H__


#include "VEngineConfig.h"
#include "VLogicModelManager.h"
#include "IActionEventListener.h"


class VSkill;


class VLogicModel
{
	friend class VLogicModelManager;

protected:
	struct VEntityValue
	{
		VEntityValue() 
			: mMeshName("")
			, mMaterialName("")
			, mEntity(VNULL)
		{}
		VEntityValue(const VString &meshName, const VString &matName)
			: mMeshName(meshName)
			, mMaterialName(matName)
			, mEntity(VNULL)
		{}

		VString		mMeshName;		/**< xml读取进来的属性，网格名称 */
		VString		mMaterialName;	/**< xml读取进来的属性，材质名称 */

		Ogre::Entity	*mEntity;	/**< 创建的实体对象 */
	};
	typedef std::map<VString, VEntityValue> VEntityMap;

	struct VMaterialValue
	{
		VMaterialValue()
			: mEntityName("")
			, mMaterialName("")
		{}
		VMaterialValue(const VString &entityEntity, const VString &matName)
			: mEntityName("")
			, mMaterialName("")
		{}

		VString	mEntityName;	/**< xml读取进来的属性，实体名称 */
		VString mMaterialName;	/**< xml读取进来的属性，材质名称 */
	};
	typedef std::map<VString, VMaterialValue> VMaterialMap;

	struct VSlotValue
	{
		VSlotValue()
			: mModelName("")
			, mModel(VNULL)
		{}
		VSlotValue(const VString &modelName)
			: mModelName(modelName)
			, mModel(VNULL)
		{}
		~VSlotValue()
		{
			if (mModel)
			{

			}
		}

		VString		mModelName;	/**< xml读取进来的属性，模型名称 */

		VLogicModel	*mModel;	/**< 模型对象 */
	};
	typedef std::map<VString, VSlotValue> VSlotMap;

	struct VLocatorValue
	{
		VLocatorValue()
			: mBoneName("")
			, mPosition(Ogre::Vector3::ZERO)
			, mOrientation(Ogre::Quaternion::IDENTITY)
			, mTransferable(VFALSE)
			, mTranslateFirst(VFALSE)
			, mLocatorNode(VNULL)
		{
			mSlots.clear();
		}
		
		VLocatorValue(const VString &boneName, const Ogre::Vector3 &pos, const Ogre::Quaternion &orientation,
					  VBOOL transferable = VFALSE, VBOOL translateFirst = VFALSE)
			: mBoneName(boneName)
			, mPosition(pos)
			, mOrientation(orientation)
			, mTransferable(transferable)
			, mTranslateFirst(translateFirst)
			, mLocatorNode(VNULL)
		{
			mSlots.clear();
		}
		
		VString				mBoneName;			/**< xml读取进来的属性，骨骼名称 */
		Ogre::Vector3		mPosition;			/**< xml读取进来的属性，位置 */
		Ogre::Quaternion	mOrientation;		/**< xml读取进来的属性，方向 */
		VBOOL				mTransferable;		/**< xml读取进来的属性，*/
		VBOOL				mTranslateFirst;	/**< xml读取进来的属性，*/

		Ogre::SceneNode		*mLocatorNode;
		VSlotMap			mSlots;
	};
	typedef std::map<VString, VLocatorValue> VLocatorMap;

	struct VAttribValue
	{
		VAttribValue()
			: mValue("")
			, mHint(VNULL)
		{}
		VAttribValue(const VString &value, const VString &hint)
			: mValue(value)
			, mHint(hint)
		{}

		VString	mValue;		/**< xml读取进来的属性，属性值 */
		VString	mHint;		/**< xml读取进来的属性，提示值 */
	};
	typedef std::map<VString, VAttribValue> VAttribMap;

	struct VEffectValue
	{
		VEffectValue()
			: mEffectName("")
			, mLocator("")
			, mTranslateAll(VFALSE)
			, mEffectColor(Ogre::ColourValue::White)
		{}
		VEffectValue(const VString &effectName, const VString &locatorName, VBOOL translateAll, const Ogre::ColourValue &color)
			: mEffectName(effectName)
			, mLocator(locatorName)
			, mTranslateAll(translateAll)
			, mEffectColor(color)
		{}

		VString				mEffectName;	/**< xml读取进来的属性，效果名称 */
		VString				mLocator;		/**< xml读取进来的属性，定位点 */
		VBOOL				mTranslateAll;	/**< xml读取进来的属性， */
		Ogre::ColourValue	mEffectColor;	/**< xml读取进来的属性，效果颜色 */
	};
	typedef std::map<VString, VEffectValue> VEffectMap;

	typedef std::list<VString> VShadowUncastableMeshes;

	enum VFabricColliderType
	{
		FCT_BOX,
		FCT_SPHERE,
		FCT_PLANE,
	};

	struct VFabricColliderValue
	{
		VFabricColliderValue()
			: mLocatorName("")
			, mColliderInfo("")
		{}
		VFabricColliderValue(const VString &locator, VFabricColliderType type, const VString &info)
			: mLocatorName(locator)
			, mColliderType(type)
			, mColliderInfo(info)
		{}

		VString				mLocatorName;
		VFabricColliderType mColliderType;
		VString				mColliderInfo;
	};
	typedef std::map<VString, VFabricColliderValue> VFabricColliderMap;

	// 被挂接上的物体原来的node
	typedef std::map<VLogicModel*, Ogre::Node*> VAttachedModelOriginSceneNodes;

	enum MovementType
	{
		MT_NORMAL,				/**< 普通，不会随地形修正object的方位 */
		MT_ROTATE_X_AXIS,		/**< 前后摇摆 */
		MT_ROTATE_X_Z_AXIS,		/**< 前后左右摇摆 */
	};

public:
	VLogicModel(const VString &name);
	virtual ~VLogicModel();

	VBOOL addEntity(const VString &name, const VString &meshName, const VString &matName);
	VBOOL changeEntity(const VString &name, const VString &value);
	VBOOL removeEntity(const VString &name);

	VBOOL addMaterial(const VString &name, const VString &entityName, const VString &matName);
	VBOOL changeMaterial(const VString &name, const VString &value);

	VBOOL addLocator(const VString &name, const VString &boneName, const Ogre::Vector3 &pos,
		const Ogre::Quaternion &orientation, VBOOL transferable = VFALSE, VBOOL translateFirst = VFALSE);

	VBOOL addSlot(const VString &locatorName, const VString &slotName, const VString &modelName, const VString &attrib);
	VBOOL changeSlotAttrib(const VString &name, const VString &value);

	VBOOL addAttribute(const VString &name, const VString &value, const VString &hint);
	VBOOL changeAttribute(const VString &name, const VString &value);
	VBOOL changeAttributeHints(const VString &name, const VString &hints);
	Variant getAttribute(const VString &attribName);
	VBOOL hasAttribute(const VString &name) const;

	VBOOL addEffect(const VString &name, const VString &effectName, const VString &locatorName,
		VBOOL translateAll = VFALSE, const Ogre::ColourValue &color = Ogre::ColourValue::White);

	VBOOL addShadowUncastable(const VString &meshName);
	VBOOL isShadowUncastable(const VString &meshName);

	VBOOL addFabricCollider(const VString &colliderName, const VString &locatorName, VFabricColliderType colliderType, const VString &colliderInfo);

	void init();

	void createSkill(const VString &skillName, VBOOL loop, VBOOL anim, const Real &delay);

	void setParentSceneNode(Ogre::Node *parentNode);

	Ogre::SceneNode *getSceneNode()
	{
		return mModelMainNode;
	}

	void setActive(VBOOL isActive)
	{
		mIsActive = isActive;
	}
	VBOOL isActive() const
	{
		return mIsActive;
	}

	void setChildModel(VBOOL isChild)
	{
		mIsChild = isChild;
	}
	VBOOL isChildModel() const
	{
		return mIsChild;
	}

	// 设置和获取可见性标记
	void setVisibilityFlags(VUINT32 flags);
	VUINT32 getVisibilityFlags() const
	{
		return mVisibilityFlags;
	}

	// 设置和获取可见性
	void setVisible(VBOOL visible);
	VBOOL isVisible() const
	{
		return mVisible;
	}

	// 挂接logic model
	VBOOL attachModel(const VString &locatorName, VLogicModel *model);
	// 卸载logic model
	VBOOL detachModel(VLogicModel *model);
	// 设置挂接所在的父logic model
	void notifyAttach(VLogicModel *parentModel)
	{
		mAttachParent = parentModel;
	}
	// 判断当前是否挂接在别的logic model上
	VBOOL isAttached() const
	{
		return (mAttachParent != VNULL);
	}
	// 获取所挂接的父logic model
	VLogicModel *getAttachModel()
	{
		return mAttachParent;
	}

	// 设置logic model位置
	void setPosition(const Vector3 &pos);
	// 获取logic model位置
	const Vector3 &getPosition() const
	{
		return mModelPosition;
	}

	// 设置logic model方向
	void setOrientation(const Quaternion &orientation);
	// 获取logic model方向
	const Quaternion &getOrientation() const
	{
		return mModelOrientation;
	}

	void setScale(const Vector3 &factor);
	const Vector3 &getScale() const
	{
		return mExternalScaleFactor;
	}

	const Ogre::AxisAlignedBox &getBoundingBox() const;

	void setActionEventListener(IActionEventListener *listener)
	{
		mActionListener = listener;
	}

	void setAnimationLoop(VBOOL loop);
	VBOOL getAnimationLoop() const
	{
		return mIsAnimationLoop;
	}

	void execute(Real dt);

protected:
	void _createMainSceneNode();

	void _createEntities();
	Ogre::Entity *_createEntity(const VString &meshName);
	void _destroyEntity(Ogre::Entity *entity);

	void _createSkeletonEntity(const Ogre::SkeletonPtr& skeleton);
	void _destroySkeletonEntity();

	void _setEntityMaterials();
	VBOOL _setEntityMaterial(const VString &entityName, const VString &matName);

	VBOOL _createModelInSlot(VSlotValue &slot, VLocatorValue &locator);

	void _processExtraAttributes(const VString &name, const VString &value);

	void _createLocatorNode();
	Ogre::SceneNode *_getLocatorSceneNode(VLocatorValue &locator);
	Ogre::SceneNode *_createLocatorSceneNode(const Ogre::Vector3 &pos, const Ogre::Quaternion &ori);
	void _updateLocatorPos();
	void _executeLocatorModel(const Ogre::Real &dt);

	void _createEffects();

	VBOOL _changeAnimation(const VString &animName, const Real &delay);
	Ogre::AnimationState *_getAnimationState(const VString &animName) const;

	void _destroyCurrentSkill();

	void _executeAttachedModel(const Ogre::Real &dt);

	const Ogre::AxisAlignedBox& _getFullBoundingBox(void) const
	{
		if (mFullBoundingBox == VNULL)
		{
			mFullBoundingBox = new Ogre::AxisAlignedBox;
		}

		return *mFullBoundingBox;
	}
	void _updateBoundingBox();
	void _createBoundingBoxFromExternal();

	void _resetPosition();

	void _setMovementType(const VString &value);
	void _setDefaultAnimationName(const VString &value)
	{
		mDefaultAnimationName = value;
	}
	void _setScaleFactor(const VString &value);

	void _setEntityNormal(VBOOL normal);

	VBOOL _handleAnimationEvent(const Ogre::Real &lastTime, const Ogre::Real &curTime);

protected:
	// 设置属性时的属性类型
	static const VString MESH_HINT;
	static const VString MATERIAL_HINT;
	static const VString SLOT_CREATE_HINT;

	// 特殊属性名称
	static const VString MOVEMENT_TYPE_ATTR;
	static const VString DEFAULT_ANIM_NAME_ATTR;
	static const VString BOUNDING_BOX_ATTR;
	static const VString SCALE_FACTOR_ATTR;
	static const VString HAIR_MESH_NAME_ATTR;

	VEntityMap			mEntities;
	VMaterialMap		mMaterials;
	VLocatorMap			mLocators;
	VAttribMap			mAttributes;
	VEffectMap			mEffects;
	VFabricColliderMap	mFabricColliders;

	VShadowUncastableMeshes	mShadowUncastableMeshes;

	VAttachedModelOriginSceneNodes	mAttachedModelOriginSceneNodes;

	Ogre::Vector3		mExternalScaleFactor;

	Ogre::Vector3		mModelPosition;		// 模型位置
	Ogre::Quaternion	mModelOrientation;	// 模型方向

	VString		mName;						/**< logic modle名称 */
	VString		mDefaultAnimationName;		/**< 默认动画名称 */
	VString		mExternalBoundingBoxValue;	/**< 外部设置的bounding box值 */
	VString		mCurrentAnimationName;		/**< 当前播放动画名称 */

	Ogre::SceneNode		*mModelMainNode;
	Ogre::Node			*mParentNode;

	Ogre::Entity			*mSkeletonEntity;
	Ogre::SkeletonInstance	*mSkeleton;

	Ogre::AxisAlignedBox			*mExternalBoundingBox;
	mutable Ogre::AxisAlignedBox	*mFullBoundingBox;

	VAnimationSkeletonLinks			*mLinks;

	VLogicModel		*mAttachParent;	// 挂接的父模型

	IActionEventListener	*mActionListener;			/**< 动作事件监听对象 */
	Ogre::AnimationState	*mCurrentAnimationState;	/**< 当前动画 */

	VSkill			*mCurrentSkill;	/**< 当前技能对象 */

	MovementType	mMovementType;

	VUINT32		mCreatedEntityCount;
	VUINT32		mVisibilityFlags;	// 可见性标记

	VBOOL		mShadowCastable;	// 是否投影阴影
	VBOOL		mMainNodeNeedUpdate;// 是否需要更新主节点
	VBOOL		mVisible;			// 模型是否可以见
	VBOOL		mIsActive;			// 是否处于活动状态
	VBOOL		mIsChild;			// 是否子模型
	VBOOL		mIsAnimationLoop;	// 是否循环动画
};

#endif	/*__VISION_LOGIC_MODEL_H__*/
