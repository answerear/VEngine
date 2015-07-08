

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

		VString		mMeshName;		/**< xml��ȡ���������ԣ��������� */
		VString		mMaterialName;	/**< xml��ȡ���������ԣ��������� */

		Ogre::Entity	*mEntity;	/**< ������ʵ����� */
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

		VString	mEntityName;	/**< xml��ȡ���������ԣ�ʵ������ */
		VString mMaterialName;	/**< xml��ȡ���������ԣ��������� */
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

		VString		mModelName;	/**< xml��ȡ���������ԣ�ģ������ */

		VLogicModel	*mModel;	/**< ģ�Ͷ��� */
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
		
		VString				mBoneName;			/**< xml��ȡ���������ԣ��������� */
		Ogre::Vector3		mPosition;			/**< xml��ȡ���������ԣ�λ�� */
		Ogre::Quaternion	mOrientation;		/**< xml��ȡ���������ԣ����� */
		VBOOL				mTransferable;		/**< xml��ȡ���������ԣ�*/
		VBOOL				mTranslateFirst;	/**< xml��ȡ���������ԣ�*/

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

		VString	mValue;		/**< xml��ȡ���������ԣ�����ֵ */
		VString	mHint;		/**< xml��ȡ���������ԣ���ʾֵ */
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

		VString				mEffectName;	/**< xml��ȡ���������ԣ�Ч������ */
		VString				mLocator;		/**< xml��ȡ���������ԣ���λ�� */
		VBOOL				mTranslateAll;	/**< xml��ȡ���������ԣ� */
		Ogre::ColourValue	mEffectColor;	/**< xml��ȡ���������ԣ�Ч����ɫ */
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

	// ���ҽ��ϵ�����ԭ����node
	typedef std::map<VLogicModel*, Ogre::Node*> VAttachedModelOriginSceneNodes;

	enum MovementType
	{
		MT_NORMAL,				/**< ��ͨ���������������object�ķ�λ */
		MT_ROTATE_X_AXIS,		/**< ǰ��ҡ�� */
		MT_ROTATE_X_Z_AXIS,		/**< ǰ������ҡ�� */
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

	// ���úͻ�ȡ�ɼ��Ա��
	void setVisibilityFlags(VUINT32 flags);
	VUINT32 getVisibilityFlags() const
	{
		return mVisibilityFlags;
	}

	// ���úͻ�ȡ�ɼ���
	void setVisible(VBOOL visible);
	VBOOL isVisible() const
	{
		return mVisible;
	}

	// �ҽ�logic model
	VBOOL attachModel(const VString &locatorName, VLogicModel *model);
	// ж��logic model
	VBOOL detachModel(VLogicModel *model);
	// ���ùҽ����ڵĸ�logic model
	void notifyAttach(VLogicModel *parentModel)
	{
		mAttachParent = parentModel;
	}
	// �жϵ�ǰ�Ƿ�ҽ��ڱ��logic model��
	VBOOL isAttached() const
	{
		return (mAttachParent != VNULL);
	}
	// ��ȡ���ҽӵĸ�logic model
	VLogicModel *getAttachModel()
	{
		return mAttachParent;
	}

	// ����logic modelλ��
	void setPosition(const Vector3 &pos);
	// ��ȡlogic modelλ��
	const Vector3 &getPosition() const
	{
		return mModelPosition;
	}

	// ����logic model����
	void setOrientation(const Quaternion &orientation);
	// ��ȡlogic model����
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
	// ��������ʱ����������
	static const VString MESH_HINT;
	static const VString MATERIAL_HINT;
	static const VString SLOT_CREATE_HINT;

	// ������������
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

	Ogre::Vector3		mModelPosition;		// ģ��λ��
	Ogre::Quaternion	mModelOrientation;	// ģ�ͷ���

	VString		mName;						/**< logic modle���� */
	VString		mDefaultAnimationName;		/**< Ĭ�϶������� */
	VString		mExternalBoundingBoxValue;	/**< �ⲿ���õ�bounding boxֵ */
	VString		mCurrentAnimationName;		/**< ��ǰ���Ŷ������� */

	Ogre::SceneNode		*mModelMainNode;
	Ogre::Node			*mParentNode;

	Ogre::Entity			*mSkeletonEntity;
	Ogre::SkeletonInstance	*mSkeleton;

	Ogre::AxisAlignedBox			*mExternalBoundingBox;
	mutable Ogre::AxisAlignedBox	*mFullBoundingBox;

	VAnimationSkeletonLinks			*mLinks;

	VLogicModel		*mAttachParent;	// �ҽӵĸ�ģ��

	IActionEventListener	*mActionListener;			/**< �����¼��������� */
	Ogre::AnimationState	*mCurrentAnimationState;	/**< ��ǰ���� */

	VSkill			*mCurrentSkill;	/**< ��ǰ���ܶ��� */

	MovementType	mMovementType;

	VUINT32		mCreatedEntityCount;
	VUINT32		mVisibilityFlags;	// �ɼ��Ա��

	VBOOL		mShadowCastable;	// �Ƿ�ͶӰ��Ӱ
	VBOOL		mMainNodeNeedUpdate;// �Ƿ���Ҫ�������ڵ�
	VBOOL		mVisible;			// ģ���Ƿ���Լ�
	VBOOL		mIsActive;			// �Ƿ��ڻ״̬
	VBOOL		mIsChild;			// �Ƿ���ģ��
	VBOOL		mIsAnimationLoop;	// �Ƿ�ѭ������
};

#endif	/*__VISION_LOGIC_MODEL_H__*/
