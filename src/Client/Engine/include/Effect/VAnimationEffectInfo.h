

#ifndef __VISION_ANIMATION_EFFECT_INFO_H__
#define __VISION_ANIMATION_EFFECT_INFO_H__


#include "VEngineConfig.h"


class VEffect;


class VAnimationEffectInfo : public Ogre::StringInterface
{
protected:
	class CmdAttachTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdAttachPoint : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdEffectTemplateName : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdOffsetPos : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdOffsetRotation : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdAttach : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VAnimationEffectInfo();
	~VAnimationEffectInfo();

	void setAttachTime(VFLOAT time)
	{
		mAttachTime = time;
	}
	VFLOAT getAttachTime() const
	{
		return mAttachTime;
	}

	void setAttachPoint(const VString &point)
	{
		mAttachPoint = point;
	}
	const VString &getAttachPoint() const
	{
		return mAttachPoint;
	}

	void setEffectTemplateName(const VString &name)
	{
		mEffectTemplateName = name;
	}
	const VString &getEffectTemplateName() const
	{
		return mEffectTemplateName;
	}

	void setOffsetPos(const Vector3 &pos)
	{
		mOffsetPos = pos;
	}
	const Vector3 &getOffsetPos() const
	{
		return mOffsetPos;
	}

	void setOffsetRotation(const Quaternion &rotation)
	{
		mOffsetRotation = rotation;
	}
	const Quaternion &getOffsetRotation() const
	{
		return mOffsetRotation;
	}

	void setAttach(VBOOL attach)
	{
		mAttach = attach;
	}
	VBOOL getAttach() const
	{
		return mAttach;
	}

	void setEffect(VEffect *effect)
	{
		mEffect = effect;
	}
	VEffect *getEffect()
	{
		return mEffect;
	}

	void copyParameters(VAnimationEffectInfo &other) const;

protected:
	VBOOL _initParamDictionary();

protected:
	static CmdAttachTime	msAttachTimeCmd;
	static CmdAttachPoint	msAttachPointCmd;
	static CmdEffectTemplateName	msEffectTemplateNameCmd;
	static CmdOffsetPos		msOffsetPosCmd;
	static CmdOffsetRotation	msOffsetRotationCmd;
	static CmdAttach		msAttachCmd;

	VEffect	*mEffect;		/**< ��Ч���� */

	VFLOAT	mAttachTime;	/**< �ҽ�ʱ�� */

	VString	mAttachPoint;			/**< �ҽӵĽڵ����� */
	VString mEffectTemplateName;	/**< ��Чģ������ */

	Vector3		mOffsetPos;			/**< ƫ�Ƶ�λ�� */
	Quaternion	mOffsetRotation;	/**< ƫ�Ƶ���ת */

	VBOOL	mAttach;	/**< �Ƿ�ҽ� */
};

#endif	/*__VISION_ANIMATION_EFFECT_INFO_H__*/
