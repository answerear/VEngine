

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

	/** 更新特效 */
	void update(const Real &dt);

	/** 设置变换 */
	void setTransform(const VTransform &xform);
	void setExtraTransforms(const VTransforms &xforms);

	/** 重新生成特效 */
	void reborn();

	/** 添加特效元素 */
	void addElement(VEffectElement *element);
	void addElement(const VString &type);

	/** 移除特效元素 */
	void removeElement(VEffectElement *element);
	void removeAllElements();

	/** 把特效从父节点移除 */
	void removeFromParentSceneNode();

	/** 销毁特效 */
	void destroyElement(VEffectElement *element);
	void destroyAllElements();

	/** 特效元素的数量 */
	VWORD numberOfElements() const
	{
		return static_cast<VWORD>(mElements.size());
	}

	VEffectElement *getElement(VWORD index);

	Ogre::SceneNode *createSceneNode();
	void createSceneNode(Ogre::SceneNode *parentNode);

	/** 关闭特效 */
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

	VString mTemplateName;	/**< 特效模板名称 */

	VTransform mTransform;	/**< 变化信息 */

	VElementList mElements;	/**< 特效元素列表 */

	Real	mLifeTime;			/**< 生命周期 */
	Real	mElapseLifeTime;	/**< 当前流逝的时间 */

	Real	mIterationInterval;		/**< 迭代时间间隔 */
	Real	mRemainIterationTime;	/**< 迭代剩余时间 */

	VUINT32	mVisibilityFlags;		/**< 可见性标记 */

	VBOOL	mIsAlive;				/**< 特效是否还存在 */
	VBOOL	mDeleteLaterFlag;		/**< 稍后删除标记 */
	VBOOL	mRemoveParentNodeFlag;	/**< 是否要从父节点移除 */
	VBOOL	mNeedSetVisibilityFlag;	/**< 是否需要设置可见性标记 */
	VBOOL	mUseFreePool;			/**< 是否需要放入空闲特效池 */
};


#endif	/*__VISION_EFFECT_H__*/
