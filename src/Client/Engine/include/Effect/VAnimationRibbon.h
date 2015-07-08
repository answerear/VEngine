

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

	Points	mPoints;	/**< 顶点链表 */

	Ogre::HardwareVertexBufferSharedPtr	mVertexBuffer;	/**< 顶点缓存 */

	Ogre::SceneNode		*mBaseNode;		/**< 场景节点 */

	Ogre::SimpleSpline	mFirstPosSpline;	/**< 第一个点的插值对象 */
	Ogre::SimpleSpline	mSecondPosSpline;	/**< 第二个点的插值对象 */

	VFLOAT	mBoundingRadius;	/**< bounding radius */

	VFLOAT	mAttachTime;	/**< 挂接到模型上的时间点 */
	VString	mAttachPoint;	/**< 挂接到模型上骨骼名称 */

	Ogre::Vector3		mOffsetPos;			/**< 挂接点偏移位置 */
	Ogre::Quaternion	mOffsetRotation;	/**< 挂接点偏移旋转 */

	VWORD	mPointCount;		/**< 顶点数，4的倍数，以用于OT_TRIANGLE_STRIP */
	
	VFLOAT	mLifeTime;				/**< 生命周期 */
	VFLOAT	mElapseLifeTime;		/**< 流逝的生命时间 */
	VFLOAT	mRibbonWidth;			/**< ribbon宽度 */
	VFLOAT	mStartAlphaValue;		/**< 开始的半透明值 */
	VFLOAT	mEndAlphaValue;			/**< 结束的半透明值 */
	VFLOAT	mCurrentStartAlphaValue;/**< 当前开始的半透明值 */
	VFLOAT	mCurrentEndAlphaValue;	/**< 当前结束的半透明值 */
	VFLOAT	mFadeInTime;			/**< 淡入时间 */
	VFLOAT	mFadeOutTime;			/**< 淡出时间 */
	VFLOAT	mVertexLifeTime;		/**< 顶点的生命时间 */

	Ogre::ColourValue	mStartColor;	/**< 起始颜色 */
	Ogre::ColourValue	mEndColor;		/**< 结束颜色 */

	VINT32	mInterpolationTimes;	/**< 插值次数 */
	VBOOL	mUseInterpolation;		/**< 是否使用插值 */

	VBOOL	mBufferCreated;			/**< 缓存是否创建 */
	VBOOL	mRenderInstanceCreated;	/**< 渲染实例是否创建*/
};


#endif	/*__VISION_ANIMATION_RIBBON_H__*/
