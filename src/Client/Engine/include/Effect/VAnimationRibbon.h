

#ifndef __VISION_ANIMATION_RIBBON_H__
#define __VISION_ANIMATION_RIBBON_H__


#include "VEngineConfig.h"
#include "VTransform.h"

class VAnimationRibbon : public Ogre::StringInterface, public Ogre::SimpleRenderable
{
	struct Vertex
	{
		Ogre::Vector3	pos;
		VFLOAT			lifeTime;

		Vertex(const Ogre::Vector3 &v)
			: pos(v)
			, lifeTime(0.0f)
		{

		}
	};
	typedef std::list<Vertex> Points;
	typedef Points::iterator PointIterator;

protected:
	class CmdPointCount : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdLifeTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdStartColor : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdEndColor : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdStartAlpha : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdEndAlpha : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdFadeInTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdFadeOutTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdMaterial : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdRibbonWidth : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

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

	class CmdUseInter : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdInterTimes : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdVertexLifeTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VAnimationRibbon(VWORD pointCount = 30, VFLOAT lifeTime = 1.0f, VFLOAT ribbonWidth = 100.0f,
		const Ogre::ColourValue &startColor = Ogre::ColourValue::White,
		const Ogre::ColourValue &endColor = Ogre::ColourValue::White,
		VFLOAT startAlpha = 1.0f, VFLOAT endAlpha = 1.0f, VFLOAT vertexLifeTime = 0.0f);
	~VAnimationRibbon();

	VBOOL createRenderInstance();
	void updateRenderInstance(const Real &dt, const VTransform &xform);
	void destroyRenderInstance();

	// from Ogre::SimpleRenderable
	virtual float getBoundingRadius() const;
	virtual float getSquaredViewDepth(const Ogre::Camera* cam) const;

	void setPointCount(VWORD count);
	VWORD getPointCount() const
	{
		return mPointCount;
	}

	void setRibbonWidth(VFLOAT width)
	{
		mRibbonWidth = width;
	}
	VFLOAT getRibbonWidth() const
	{
		return mRibbonWidth;
	}

	void setStartColor(const Ogre::ColourValue &value)
	{
		mStartColor = value;
	}
	const Ogre::ColourValue &getStartColor() const
	{
		return mStartColor;
	}

	void setEndColor(const Ogre::ColourValue &value)
	{
		mEndColor = value;
	}
	const Ogre::ColourValue &getEndColor() const
	{
		return mEndColor;
	}

	void setStartAlphaValue(VFLOAT value)
	{
		mStartAlphaValue = value;
	}
	VFLOAT getStartAlphaValue() const
	{
		return mStartAlphaValue;
	}

	void setEndAlphaValue(VFLOAT value)
	{
		mEndAlphaValue = value;
	}
	VFLOAT getEndAlphaValue() const
	{
		return mEndAlphaValue;
	}

	void setMaterialName(const VString &matName);
	const VString &getMaterialName() const
	{
		return m_strMatName;
	}

	void setLifeTime(VFLOAT time)
	{
		mLifeTime = time;
	}
	VFLOAT getLifeTime() const
	{
		return mLifeTime;
	}

	void setFadeInTime(VFLOAT time)
	{
		if ((time < 0.0f) || (time > 1.0f))
			mFadeInTime = 0.0f;
		else
			mFadeInTime = time;
	}
	VFLOAT getFadeInTime() const
	{
		return mFadeInTime;
	}

	void setFadeOutTime(VFLOAT time)
	{
		if ((time < 0.0f) || (time > 1.0f))
			mFadeOutTime = 0.0f;
		else
			mFadeOutTime = time;
	}
	VFLOAT getFadeOutTime() const
	{
		return mFadeOutTime;
	}

	void setVertexLifeTime(VFLOAT time)
	{
		mVertexLifeTime = time;
	}
	VFLOAT getVertexLifeTime() const
	{
		return mVertexLifeTime;
	}

	void setAttachTime(VFLOAT time)
	{
		mAttachTime = time;
	}
	VFLOAT getAttachTime() const
	{
		return mAttachTime;
	}

	void setAttachPoint(const VString &name)
	{
		mAttachPoint = name;
	}
	const VString &getAttachPoint() const
	{
		return mAttachPoint;
	}

	void setOffsetPos(const Ogre::Vector3 &pos)
	{
		mOffsetPos = pos;
	}
	const Ogre::Vector3 &getOffsetPos() const
	{
		return mOffsetPos;
	}

	void setOffsetRotation(const Ogre::Quaternion &rotation)
	{
		mOffsetRotation = rotation;
	}
	const Ogre::Quaternion &getOffsetRotation() const
	{
		return mOffsetRotation;
	}

	void setUseInterpolation(VBOOL use);
	VBOOL getUseInterpolation(void) const
	{
		return mUseInterpolation;
	}

	void setInterpolationTimes(VINT32 times);
	VINT32 getInterpolationTimes(void) const
	{
		return mInterpolationTimes;
	}

	void copyParameters(VAnimationRibbon &other) const;

protected:
	void _createVertexBuffer(VINT32 vertexCount);
	void _destroyVertexBuffer();

protected:
	static CmdPointCount msPointCountCmd;
	static CmdStartColor msStartColor;
	static CmdEndColor msEndColor;
	static CmdStartAlpha msStartAlpha;
	static CmdEndAlpha msEndAlpha;
	static CmdFadeInTime msFadeInTimeCmd;
	static CmdFadeOutTime msFadeOutTimeCmd;
	static CmdMaterial msMaterial;
	static CmdRibbonWidth msRibbonWidthCmd;
	static CmdLifeTime msLifeTimeCmd;
	static CmdAttachTime msAttachTimeCmd;
	static CmdAttachPoint msAttachPointCmd;
	static CmdOffsetPos msOffsetPosCmd;
	static CmdOffsetRotation msOffsetRotationCmd;
	static CmdUseInter msUseInterCmd;
	static CmdInterTimes msInterTimesCmd;
	static CmdVertexLifeTime msVertexLifeTimeCmd;

	Points	mPoints;	/**< �������� */

	Ogre::HardwareVertexBufferSharedPtr	mVertexBuffer;	/**< ���㻺�� */

	Ogre::SceneNode		*mBaseNode;		/**< �����ڵ� */

	Ogre::SimpleSpline	mFirstPosSpline;	/**< ��һ����Ĳ�ֵ���� */
	Ogre::SimpleSpline	mSecondPosSpline;	/**< �ڶ�����Ĳ�ֵ���� */

	VFLOAT	mBoundingRadius;	/**< bounding radius */

	VFLOAT	mAttachTime;	/**< �ҽӵ�ģ���ϵ�ʱ��� */
	VString	mAttachPoint;	/**< �ҽӵ�ģ���Ϲ������� */

	Ogre::Vector3		mOffsetPos;			/**< �ҽӵ�ƫ��λ�� */
	Ogre::Quaternion	mOffsetRotation;	/**< �ҽӵ�ƫ����ת */

	VWORD	mPointCount;		/**< ��������4�ı�����������OT_TRIANGLE_STRIP */
	
	VFLOAT	mLifeTime;				/**< �������� */
	VFLOAT	mElapseLifeTime;		/**< ���ŵ�����ʱ�� */
	VFLOAT	mRibbonWidth;			/**< ribbon��� */
	VFLOAT	mStartAlphaValue;		/**< ��ʼ�İ�͸��ֵ */
	VFLOAT	mEndAlphaValue;			/**< �����İ�͸��ֵ */
	VFLOAT	mCurrentStartAlphaValue;/**< ��ǰ��ʼ�İ�͸��ֵ */
	VFLOAT	mCurrentEndAlphaValue;	/**< ��ǰ�����İ�͸��ֵ */
	VFLOAT	mFadeInTime;			/**< ����ʱ�� */
	VFLOAT	mFadeOutTime;			/**< ����ʱ�� */
	VFLOAT	mVertexLifeTime;		/**< ���������ʱ�� */

	Ogre::ColourValue	mStartColor;	/**< ��ʼ��ɫ */
	Ogre::ColourValue	mEndColor;		/**< ������ɫ */

	VINT32	mInterpolationTimes;	/**< ��ֵ���� */
	VBOOL	mUseInterpolation;		/**< �Ƿ�ʹ�ò�ֵ */

	VBOOL	mBufferCreated;			/**< �����Ƿ񴴽� */
	VBOOL	mRenderInstanceCreated;	/**< ��Ⱦʵ���Ƿ񴴽�*/
};


#endif	/*__VISION_ANIMATION_RIBBON_H__*/
