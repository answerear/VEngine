

#ifndef __VISION_BILLBOARD_SET_ELEMENT_H__
#define __VISION_BILLBOARD_SET_ELEMENT_H__


#include "VEffectElement.h"


class VBillboardSetElement : public VEffectElement
{
protected:
	class CmdBillboardColor : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdBillboardAlpha : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdBillboardCount : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdBillboardMaterial : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdBillboardInterval : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdBillboardWidth : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdBillboardHeight : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdBillboardScale : public Ogre::ParamCommand
	{
	public:
		size_t mIndex;

	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdScaleIntervalTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdStacks : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdSlices : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdTexIntervalTime : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

protected:
	enum { MAX_STAGES = 3 };

	struct VScaleInfo
	{
		Ogre::Real mCurrentScaleIntervalTime;
		int mCurrentIndex;
	};

public:
	VBillboardSetElement(const VString &type);
	~VBillboardSetElement();

	virtual VBOOL createRenderInstance();
	virtual void updateRenderInstance(const Real &dt, const VTransform &xform);
	virtual void destroyRenderInstance();

	virtual void setVisibilityFlags(VUINT32 flags);

	void setBillboardColor(const ColorValue &color);
	const ColorValue &getBillboardColor() const
	{
		return mBillboardColor;
	}

	void setBillboardScale(VINT32 index, const Vector3 &scale);
	const Vector3 &getBillboardScale(VINT32 index) const
	{
		return mBillboardScale[index];
	}

	void setScaleIntervalTime(const Real &time)
	{
		mScaleIntervalTime = time;
	}
	const Real &getScaleIntervalTime() const
	{
		return mScaleIntervalTime;
	}

	void setBillboardCount(VINT32 count);
	VINT32 getBillboardCount() const
	{
		return mBillboardCount;
	}

	void setBillboardMaterial(const VString &material);
	const VString &getBillboardMaterial() const
	{
		return mBillboardMaterial;
	}

	void setBillboardInterval(const Real &interval);
	const Real &getBillboardInterval() const
	{
		return mBillboardInterval;
	}

	void setBillboardWidth(const Real &width);
	const Real &getBillboardWidth() const
	{
		return mBillboardWidth;
	}

	void setBillboardHeight(const Real &height);
	const Real &getBillboardHeight() const
	{
		return mBillboardHeight;
	}

	void setBillboardAlpha(const Real &alpha)
	{
		mAlphaValue = alpha;
		mColorChanged = VTRUE;
	}
	const Real &getBillboardAlpha() const
	{
		return mAlphaValue;
	}

	void setStacks(VULONG stacks);
	VULONG getStacks() const
	{
		return mStacks;
	}

	void setSlices(VULONG slices);
	VULONG getSlices() const
	{
		return mSlices;
	}

	void setTexIntervalTime(const Real &time)
	{
		mTexIntervalTime = time;
	}
	const Real &getTexIntervalTime() const
	{
		return mTexIntervalTime;
	}

protected:
	VBOOL _createBillboardSet();
	void _destoryBillboardSet();

	void _resetBillboardStartTexIndex();
	void _resetBillboardStartScaleTime();

protected:
	static CmdBillboardColor msBillboardColorCmd;
	static CmdBillboardAlpha msBillboardAlphaCmd;
	static CmdBillboardCount msBillboardCountCmd;
	static CmdBillboardMaterial msBillboardMaterialCmd;
	static CmdBillboardInterval msBillboardIntervalCmd;
	static CmdBillboardHeight msBillboardHeightCmd;
	static CmdBillboardWidth msBillboardWidthCmd;

	static CmdBillboardScale msBillboardScaleCmd[MAX_STAGES];
	static CmdScaleIntervalTime msScaleIntervalTimeCmd;

	static CmdStacks msStacksCmd;
	static CmdSlices msSlicesCmd;
	static CmdTexIntervalTime msTexIntervalTimeCmd;


	std::vector<VScaleInfo> mCurrentScaleInfos;		/// 保存每个billboard的大小信息
	
	Ogre::Vector3	mBillboardScale[MAX_STAGES];	/// billboard的缩放值

	Ogre::String	mBillboardMaterial;		/// billboard材质名称

	Ogre::ColourValue	mBillboardColor;	/// 所有billboard的颜色

	Ogre::Real	mBillboardInterval;			/// 两个billboard中心点之间的间隔
	Ogre::Real	mBillboardWidth;			/// billboard的宽度
	Ogre::Real	mBillboardHeight;			/// billboard的高度
	Ogre::Real	mScaleIntervalTime;			/// billboard大小变化的时间间隔
	Ogre::Real	mAlphaValue;				/// billboard所用的alpha值
	Ogre::Real	mTexIntervalTime;			/// billboard两次改变纹理索引的时间间隔
	Ogre::Real	mCurrentTexIntervalTime;	/// 到下次改变纹理索引时间计数值

	Ogre::BillboardSet	*mBillboardSet;		/// 特效所用的BillbaordSet

	VINT32	mBillboardCount;		/// billboard的数量
	VULONG	mStacks;				/// billboard纹理划分的行数
	VULONG	mSlices;				/// billboard纹理划分的列数

	VBOOL	mSizeChanged;			/// billboard大小变化
	VBOOL	mColorChanged;			/// billboard颜色变化
};


#endif	/*__VISION_BILLBOARD_SET_ELEMENT_H__*/
