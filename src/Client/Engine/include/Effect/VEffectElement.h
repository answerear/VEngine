

#ifndef __VISION_EFFECT_ELEMENT_H__
#define __VISION_EFFECT_ELEMENT_H__


#include "VEffectBase.h"


class VEffect;


class VEffectElement 
	: public Ogre::StringInterface
	, public VEffectBase
{
protected:
	class CmdLifeTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdStartTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdRelPos : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdRelOrientation : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

protected:
	VBOOL _initDefaults(const VString &type);

public:
	VEffectElement(const VString &type);
	virtual ~VEffectElement();

	/** 获取特效元素类型 */
	const VString &getType() const
	{
		return mElementType;
	}

	/** 创建渲染实例 */
	virtual VBOOL createRenderInstance() = 0;
	/** 更新渲染实例 */
	virtual void updateRenderInstance(const Real &dt, const VTransform &xform);
	/** 销毁渲染实例 */
	virtual void destroyRenderInstance();

	/** 设置变化运算 */
	virtual void setTransform(const VTransform &xform);

	/** 创建特效元素挂载的节点 */
	void createSceneNode(Ogre::SceneNode *parentNode);

	/** 更新特效元素 */
	void update(const Real &dt, VTransform &xform);

	/** 销毁特效元素 */
	virtual void destroy();
	/** 关闭特效 */
	virtual void shutdown();
	/** 更新额外的变换 */
	virtual void updateExtraTransforms(const VTransforms &xforms);

	/** 让特效休眠，暂停并且隐藏起来 */
	virtual void sleep();
	/** 激活特效 */
	virtual void active();

	void setParent(VEffect *parent)
	{
		mParentEffect = parent;
	}
	VEffect *getParent()
	{
		return mParentEffect;
	}

	void setLifeTime(const Real &time)
	{
		mLifeTime = time;
	}
	const Real &getLifeTime() const
	{
		return mLifeTime;
	}

	virtual void setStartTime(const Real &time)
	{
		mStartTime = time;
	}
	const Real &getStartTime() const
	{
		return mStartTime;
	}

	void setRelPos(const Vector3 &pos);
	const Vector3 &getRelPos() const
	{
		return mRelPos;
	}

	void setRelOrientation(const Quaternion &orientation);
	const Quaternion &getRelOrientation() const
	{
		return mRelOrientation;
	}

	VBOOL isActive() const
	{
		return mIsActive;
	}

	VBOOL isDestroy() const
	{
		return mIsDestroy;
	}

protected:
	static CmdStartTime msStartTimeCmd;
	static CmdLifeTime msLifeTimeCmd;
	static CmdRelPos msRelPosCmd;
	static CmdRelOrientation msRelOrientationCmd;

	VString	mElementType;		/**< 特效元素类型 */

	VEffect	*mParentEffect;		/**< 父特效对象 */

	Real	mLifeTime;			/**< 生命周期 */
	Real	mStartTime;			/**< 出生的时间 */

	Real	mElapseLifeTime;	/**< 流逝的生命时间 */
	Real	mElapseStartTime;	/**< 出生时间计时 */

	Vector3		mRelPos;		/**< 相对父节点位置 */
	Quaternion	mRelOrientation;/**< 相对父节点方向 */

	VBOOL	mIsActive;					/**< 活动状态标记 */
	VBOOL	mIsRenderInstanceCreated;	/**< 创建渲染实例标记 */
	VBOOL	mIsDestroy;					/**< 是否要销毁标记 */
};


#endif	/*__VISION_EFFECT_ELEMENT_H__*/
