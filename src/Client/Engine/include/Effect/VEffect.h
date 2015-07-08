

#ifndef __VISION_EFFECT_H__
#define __VISION_EFFECT_H__


#include "VEffectBase.h"


class VEffectElement;


class VEffect 
	: public Ogre::StringInterface
	, public VEffectBase
{
	friend class VEffectManager;

	typedef std::vector<VEffectElement*> VElementList;
	typedef VElementList::iterator VElementIterator;

protected:
	class CmdLifeTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VEffect(const VString &templateName);
	~VEffect();

	VEffect(const VEffect &other);
	VEffect &operator =(const VEffect &other);

	const VString &getTemplateName() const 
	{ 
		return mTemplateName; 
	}
	void setTemplateName(const VString &name)
	{
		mTemplateName = name;
	}

	/** ������Ч */
	void update(const Real &dt);

	/** ���ñ任 */
	void setTransform(const VTransform &xform);
	void setExtraTransforms(const VTransforms &xforms);

	/** ����������Ч */
	void reborn();

	/** �����ЧԪ�� */
	void addElement(VEffectElement *element);
	void addElement(const VString &type);

	/** �Ƴ���ЧԪ�� */
	void removeElement(VEffectElement *element);
	void removeAllElements();

	/** ����Ч�Ӹ��ڵ��Ƴ� */
	void removeFromParentSceneNode();

	/** ������Ч */
	void destroyElement(VEffectElement *element);
	void destroyAllElements();

	/** ��ЧԪ�ص����� */
	VWORD numberOfElements() const
	{
		return static_cast<VWORD>(mElements.size());
	}

	VEffectElement *getElement(VWORD index);

	Ogre::SceneNode *createSceneNode();
	void createSceneNode(Ogre::SceneNode *parentNode);

	/** �ر���Ч */
	void shutdown();

	virtual VBOOL existVisibleElements() const;
	virtual void setVisible(VBOOL visible);
	virtual void setVisibilityFlags(VUINT32 flags);
	virtual void setColor(const ColorValue &color);

	void setLifeTime(const Real &time) 
	{
		mLifeTime = time;
	}
	const Real &getLifeTime() const
	{
		return mLifeTime;
	}

	VBOOL isAlive() const
	{
		return mIsAlive;
	}

	void setIterationInterval(const Real &time)
	{
		mIterationInterval = time;
	}
	const Real &getIterationInterval() const
	{
		return mIterationInterval;
	}

	void setDeleteLaterFlag(VBOOL flag)
	{
		mDeleteLaterFlag = flag;
	}
	VBOOL getDeleteLaterFlag() const
	{
		return mDeleteLaterFlag;
	}

	void setRemoveParentNodeFlag(VBOOL flag)
	{
		mRemoveParentNodeFlag = flag;
	}
	VBOOL getRemoveParentNodeFlag() const
	{
		return mRemoveParentNodeFlag;
	}

	static void setDefaultIterationInterval(const Real &time)
	{
		msDefaultIterationsInterval = time;
	}
	static Real getDefaultIterationInterval()
	{
		return msDefaultIterationsInterval;
	}

	void setUseFreePool(VBOOL enable)
	{
		mUseFreePool = enable;
	}
	VBOOL getUseFreePool() const
	{
		return mUseFreePool;
	}

protected:
	VBOOL _initDefaults();

	void _updateEffect(const Real &dt);
	void _updateElements(const Real &dt);

	void _destroyElement(VWORD index);
	VElementIterator _destroyElement(VElementIterator itr);

	void _copy(const VEffect &rhs);

protected:
	static CmdLifeTime msLifeTimeCmd;
	static Real msDefaultIterationsInterval;

	VString mTemplateName;	/**< ��Чģ������ */

	VTransform mTransform;	/**< �仯��Ϣ */

	VElementList mElements;	/**< ��ЧԪ���б� */

	Real	mLifeTime;			/**< �������� */
	Real	mElapseLifeTime;	/**< ��ǰ���ŵ�ʱ�� */

	Real	mIterationInterval;		/**< ����ʱ���� */
	Real	mRemainIterationTime;	/**< ����ʣ��ʱ�� */

	VUINT32	mVisibilityFlags;		/**< �ɼ��Ա�� */

	VBOOL	mIsAlive;				/**< ��Ч�Ƿ񻹴��� */
	VBOOL	mDeleteLaterFlag;		/**< �Ժ�ɾ����� */
	VBOOL	mRemoveParentNodeFlag;	/**< �Ƿ�Ҫ�Ӹ��ڵ��Ƴ� */
	VBOOL	mNeedSetVisibilityFlag;	/**< �Ƿ���Ҫ���ÿɼ��Ա�� */
	VBOOL	mUseFreePool;			/**< �Ƿ���Ҫ���������Ч�� */
};


#endif	/*__VISION_EFFECT_H__*/
