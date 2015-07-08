

#ifndef __VISION_BEAM_ELEMENT_H__
#define __VISION_BEAM_ELEMENT_H__


#include "VEffectElement.h"
#include "OgreBillboardChainEx.h"


class VBeamElement : public VEffectElement
{
protected:
	class CmdNoiseXMin : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdNoiseXMax : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdNoiseYMin : public Ogre::ParamCommand
	{
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdNoiseYMax : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdNoiseZMin : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdNoiseZMax : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdNumElements : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdWidth : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdColor : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdColorRangeStart : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdColorRangeEnd : public Ogre::ParamCommand
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

	class CmdFrequency : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VBeamElement(const VString &type);
	~VBeamElement();

	virtual VBOOL createRenderInstance();
	virtual void updateRenderInstance(const Real &dt, const VTransform &xform);
	virtual void destroyRenderInstance();

	virtual void updateExtraTransforms(const VTransforms &xforms);

	void setNoiseXMin(const Real &value)
	{
		mNoiseXMin = value;
	}
	const Real &getNoiseXMin() const
	{
		return mNoiseXMin;
	}

	void setNoiseXMax(const Real &value)
	{
		mNoiseXMax = value;
	}
	const Real &getNoiseXMax() const
	{
		return mNoiseXMax;
	}

	void setNoiseYMin(const Real &value)
	{
		mNoiseYMin = value;
	}
	const Real &getNoiseYMin() const
	{
		return mNoiseYMin;
	}

	void setNoiseYMax(const Real &value)
	{
		mNoiseYMax = value;
	}
	const Real &getNoiseYMax() const
	{
		return mNoiseYMax;
	}

	void setNoiseZMin(const Real &value)
	{
		mNoiseZMin = value;
	}
	const Real &getNoiseZMin() const
	{
		return mNoiseZMin;
	}

	void setNoiseZMax(const Real &value)
	{
		mNoiseZMax = value;
	}
	const Real &getNoiseZMax() const
	{
		return mNoiseZMax;
	}

	void setWidth(const Real &width)
	{
		mWidth = width;
	}
	const Real &getWidth() const
	{
		return mWidth;
	}

	void setNumBillboardElements(VINT32 num);
	VINT32 getNumBillboardElements() const
	{
		return mNumBillboardElements;
	}

	void setColor(const ColorValue &color)
	{
		mColorRangeStart = mColorRangeEnd = color;
	}
	void setColor(const ColorValue &startColor, const ColorValue &endColor)
	{
		mColorRangeStart = startColor;
		mColorRangeEnd = endColor;
	}
	void setColorRangeStart(const ColorValue &color)
	{
		mColorRangeStart = color;
	}
	void setColorRangeEnd(const ColorValue &color)
	{
		mColorRangeEnd = color;
	}

	const ColorValue &getColor() const
	{
		return mColorRangeStart;
	}
	const ColorValue &getColorRangeStart() const
	{
		return mColorRangeStart;
	}
	const ColorValue &getColorRangeEnd() const
	{
		return mColorRangeEnd;
	}

	void setMaterialName(const VString &matName);
	const VString &getMaterialName() const
	{
		return mMaterialName;
	}

	void setFrequency(const Real &frequency);
	const Real &getFrequency() const
	{
		return mFrequency;
	}

	virtual void setVisibilityFlags(VUINT32 flags);
	virtual void sleep();

protected:
	VBOOL _createBillboardChain();
	void _updateBillboardChain(const Real &dt);
	void _destroyBillboardChain();

	void _genColor(ColorValue &destColor);

protected:
	static CmdNoiseXMin msNoiseXMinCmd;
	static CmdNoiseXMax msNoiseXMaxCmd;
	static CmdNoiseYMin msNoiseYMinCmd;
	static CmdNoiseYMax msNoiseYMaxCmd;
	static CmdNoiseZMin msNoiseZMinCmd;
	static CmdNoiseZMax msNoiseZMaxCmd;

	static CmdNumElements msNumElementsCmd;
	static CmdWidth msWidthCmd;

	static CmdColor				msColorCmd;
	static CmdColorRangeStart	msColorRangeStartCmd;
	static CmdColorRangeEnd		msColorRangeEndCmd;

	static CmdMaterial msMaterialCmd;

	static CmdFrequency msFrequencyCmd;

	Ogre::Real	mNoiseXMin;
	Ogre::Real	mNoiseXMax;
	Ogre::Real	mNoiseYMin;
	Ogre::Real	mNoiseYMax;
	Ogre::Real	mNoiseZMin;
	Ogre::Real	mNoiseZMax;

	Ogre::Real	mWidth;

	Ogre::Vector3	mDestPos;

	VINT32	mNumBillboardElements;

	Ogre::BillboardChainEx *mBillboardChain;

	Ogre::ColourValue mColorRangeStart;
	Ogre::ColourValue mColorRangeEnd;

	Ogre::String mMaterialName;

	Ogre::Real mFrequency;
	Ogre::Real mFrequencyTime;
	Ogre::Real mCurrentFrequencyTime;
};


#endif	/*__VISION_BEAM_ELEMENT_H__*/
