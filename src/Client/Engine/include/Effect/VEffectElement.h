

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

	/** ��ȡ��ЧԪ������ */
	const VString &getType() const
	{
		return mElementType;
	}

	/** ������Ⱦʵ�� */
	virtual VBOOL createRenderInstance() = 0;
	/** ������Ⱦʵ�� */
	virtual void updateRenderInstance(const Real &dt, const VTransform &xform);
	/** ������Ⱦʵ�� */
	virtual void destroyRenderInstance();

	/** ���ñ仯���� */
	virtual void setTransform(const VTransform &xform);

	/** ������ЧԪ�ع��صĽڵ� */
	void createSceneNode(Ogre::SceneNode *parentNode);

	/** ������ЧԪ�� */
	void update(const Real &dt, VTransform &xform);

	/** ������ЧԪ�� */
	virtual void destroy();
	/** �ر���Ч */
	virtual void shutdown();
	/** ���¶���ı任 */
	virtual void updateExtraTransforms(const VTransforms &xforms);

	/** ����Ч���ߣ���ͣ������������ */
	virtual void sleep();
	/** ������Ч */
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

	VString	mElementType;		/**< ��ЧԪ������ */

	VEffect	*mParentEffect;		/**< ����Ч���� */

	Real	mLifeTime;			/**< �������� */
	Real	mStartTime;			/**< ������ʱ�� */

	Real	mElapseLifeTime;	/**< ���ŵ�����ʱ�� */
	Real	mElapseStartTime;	/**< ����ʱ���ʱ */

	Vector3		mRelPos;		/**< ��Ը��ڵ�λ�� */
	Quaternion	mRelOrientation;/**< ��Ը��ڵ㷽�� */

	VBOOL	mIsActive;					/**< �״̬��� */
	VBOOL	mIsRenderInstanceCreated;	/**< ������Ⱦʵ����� */
	VBOOL	mIsDestroy;					/**< �Ƿ�Ҫ���ٱ�� */
};


#endif	/*__VISION_EFFECT_ELEMENT_H__*/
