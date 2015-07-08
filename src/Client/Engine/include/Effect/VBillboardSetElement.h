

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


	std::vector<VScaleInfo> mCurrentScaleInfos;		/// ����ÿ��billboard�Ĵ�С��Ϣ
	
	Ogre::Vector3	mBillboardScale[MAX_STAGES];	/// billboard������ֵ

	Ogre::String	mBillboardMaterial;		/// billboard��������

	Ogre::ColourValue	mBillboardColor;	/// ����billboard����ɫ

	Ogre::Real	mBillboardInterval;			/// ����billboard���ĵ�֮��ļ��
	Ogre::Real	mBillboardWidth;			/// billboard�Ŀ��
	Ogre::Real	mBillboardHeight;			/// billboard�ĸ߶�
	Ogre::Real	mScaleIntervalTime;			/// billboard��С�仯��ʱ����
	Ogre::Real	mAlphaValue;				/// billboard���õ�alphaֵ
	Ogre::Real	mTexIntervalTime;			/// billboard���θı�����������ʱ����
	Ogre::Real	mCurrentTexIntervalTime;	/// ���´θı���������ʱ�����ֵ

	Ogre::BillboardSet	*mBillboardSet;		/// ��Ч���õ�BillbaordSet

	VINT32	mBillboardCount;		/// billboard������
	VULONG	mStacks;				/// billboard�����ֵ�����
	VULONG	mSlices;				/// billboard�����ֵ�����

	VBOOL	mSizeChanged;			/// billboard��С�仯
	VBOOL	mColorChanged;			/// billboard��ɫ�仯
};


#endif	/*__VISION_BILLBOARD_SET_ELEMENT_H__*/
