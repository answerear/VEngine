

#include "OgreBillboardChainEx.h"
#include "VBeamElement.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VBeamElement::CmdNoiseXMin::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getNoiseXMin());
}

void VBeamElement::CmdNoiseXMin::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setNoiseXMin(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBeamElement::CmdNoiseXMax::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getNoiseXMax());
}

void VBeamElement::CmdNoiseXMax::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setNoiseXMax(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBeamElement::CmdNoiseYMin::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getNoiseXMin());
}

void VBeamElement::CmdNoiseYMin::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setNoiseYMin(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBeamElement::CmdNoiseYMax::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getNoiseYMax());
}

void VBeamElement::CmdNoiseYMax::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setNoiseYMax(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBeamElement::CmdNoiseZMin::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getNoiseZMin());
}

void VBeamElement::CmdNoiseZMin::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setNoiseZMin(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBeamElement::CmdNoiseZMax::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getNoiseZMax());
}

void VBeamElement::CmdNoiseZMax::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setNoiseZMax(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBeamElement::CmdNumElements::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getNumBillboardElements());
}

void VBeamElement::CmdNumElements::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setNumBillboardElements(Ogre::StringConverter::parseInt(val));
}

Ogre::String VBeamElement::CmdWidth::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getWidth());
}

void VBeamElement::CmdWidth::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setWidth(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBeamElement::CmdColor::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getColor());
}

void VBeamElement::CmdColor::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setColor(Ogre::StringConverter::parseColourValue(val));
}

Ogre::String VBeamElement::CmdColorRangeStart::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getColorRangeStart());
}

void VBeamElement::CmdColorRangeStart::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setColorRangeStart(Ogre::StringConverter::parseColourValue(val));
}

Ogre::String VBeamElement::CmdColorRangeEnd::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getColorRangeEnd());
}

void VBeamElement::CmdColorRangeEnd::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setColorRangeEnd(Ogre::StringConverter::parseColourValue(val));
}

Ogre::String VBeamElement::CmdMaterial::doGet(const void *target) const
{
	return static_cast<const VBeamElement*>(target)->getMaterialName();
}

void VBeamElement::CmdMaterial::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setMaterialName(val);
}

Ogre::String VBeamElement::CmdFrequency::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBeamElement*>(target)->getFrequency());
}

void VBeamElement::CmdFrequency::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBeamElement*>(target)->setFrequency(Ogre::StringConverter::parseReal(val));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// class VBeamElement
////////////////////////////////////////////////////////////////////////////////////////////////////

VBeamElement::CmdNoiseXMin VBeamElement::msNoiseXMinCmd;
VBeamElement::CmdNoiseXMax VBeamElement::msNoiseXMaxCmd;
VBeamElement::CmdNoiseYMin VBeamElement::msNoiseYMinCmd;
VBeamElement::CmdNoiseYMax VBeamElement::msNoiseYMaxCmd;
VBeamElement::CmdNoiseZMin VBeamElement::msNoiseZMinCmd;
VBeamElement::CmdNoiseZMax VBeamElement::msNoiseZMaxCmd;
VBeamElement::CmdNumElements VBeamElement::msNumElementsCmd;
VBeamElement::CmdWidth VBeamElement::msWidthCmd;
VBeamElement::CmdColor VBeamElement::msColorCmd;
VBeamElement::CmdColorRangeStart VBeamElement::msColorRangeStartCmd;
VBeamElement::CmdColorRangeEnd VBeamElement::msColorRangeEndCmd;
VBeamElement::CmdMaterial VBeamElement::msMaterialCmd;
VBeamElement::CmdFrequency VBeamElement::msFrequencyCmd;


VBeamElement::VBeamElement(const VString &type)
	: VEffectElement(type)
	, mNoiseXMin(0.0f)
	, mNoiseXMax(0.0f)
	, mNoiseYMin(0.0f)
	, mNoiseYMax(0.0f)
	, mNoiseZMin(0.0f)
	, mNoiseZMax(0.0f)
	, mWidth(10.0f)
	, mDestPos(Ogre::Vector3::UNIT_Y)
	, mNumBillboardElements(10)
	, mBillboardChain(VNULL)
	, mColorRangeStart(Ogre::ColourValue::White)
	, mColorRangeEnd(Ogre::ColourValue::White)
	, mMaterialName("BaseWhiteNoLighting")
	, mFrequency(20.0f)
	, mFrequencyTime(0.0f)
	, mCurrentFrequencyTime(0.0f)
{
	if (_initDefaults(mElementType))
	{
		Ogre::ParamDictionary *dict = getParamDictionary();

		// Custom params      
		dict->addParameter(Ogre::ParameterDef("NoiseXMin", 
			"min noise of x axis.",
			Ogre::PT_REAL),
			&msNoiseXMinCmd);

		dict->addParameter(Ogre::ParameterDef("NoiseXMax", 
			"max noise of x axis.",
			Ogre::PT_REAL),
			&msNoiseXMaxCmd);

		dict->addParameter(Ogre::ParameterDef("NoiseYMin", 
			"min noise of y axis.",
			Ogre::PT_REAL),
			&msNoiseYMinCmd);

		dict->addParameter(Ogre::ParameterDef("NoiseYMax", 
			"max noise of y axis.",
			Ogre::PT_REAL),
			&msNoiseYMaxCmd);

		dict->addParameter(Ogre::ParameterDef("NoiseZMin", 
			"min noise of z axis.",
			Ogre::PT_REAL),
			&msNoiseZMinCmd);

		dict->addParameter(Ogre::ParameterDef("NoiseZMax", 
			"max noise of z axis.",
			Ogre::PT_REAL),
			&msNoiseZMaxCmd);

		dict->addParameter(Ogre::ParameterDef("NumElements", 
			"the num of billboard chain elements.",
			Ogre::PT_INT),
			&msNumElementsCmd);

		dict->addParameter(Ogre::ParameterDef("Width", 
			"width of billboard.",
			Ogre::PT_REAL),
			&msWidthCmd);

		dict->addParameter(Ogre::ParameterDef("Colour", 
			"The colour of emitted particles.", 
			Ogre::PT_COLOURVALUE),
			&msColorCmd);

		dict->addParameter(Ogre::ParameterDef("ColourRangeStart", 
			"The start of a range of colours to be assigned to emitted particles.", 
			Ogre::PT_COLOURVALUE),
			&msColorRangeStartCmd);

		dict->addParameter(Ogre::ParameterDef("ColourRangeEnd", 
			"The end of a range of colours to be assigned to emitted particles.", 
			Ogre::PT_COLOURVALUE),
			&msColorRangeEndCmd);

		dict->addParameter(Ogre::ParameterDef("Material", 
			"name of beam material.",
			Ogre::PT_STRING),
			&msMaterialCmd);

		dict->addParameter(Ogre::ParameterDef("Frequency", 
			"frequency of billboard.",
			Ogre::PT_REAL),
			&msFrequencyCmd);
	}
}

VBeamElement::~VBeamElement()
{
	destroyRenderInstance();
}

VBOOL VBeamElement::createRenderInstance()
{
	return _createBillboardChain();
}

void VBeamElement::updateRenderInstance(const Real &dt, const VTransform &xform)
{
	if (mBaseNode != VNULL)
	{
		_updateBillboardChain(dt);
	}
}

void VBeamElement::destroyRenderInstance()
{
	_destroyBillboardChain();
	VEffectElement::destroyRenderInstance();
}

void VBeamElement::updateExtraTransforms(const VTransforms &xforms)
{
	mDestPos = xforms[0].position;
}

void VBeamElement::setNumBillboardElements(VINT32 num)
{
	mNumBillboardElements = num;

	if (mBillboardChain != VNULL)
	{
		mBillboardChain->setMaxChainElements(mNumBillboardElements);
	}
}

void VBeamElement::setMaterialName(const VString &matName)
{
	mMaterialName = matName;

	if (mBillboardChain != VNULL)
	{
		mBillboardChain->setMaterial(mMaterialName);
	}
}

void VBeamElement::setFrequency(const Real &frequency)
{
	mFrequency = frequency;

	if (mFrequency > 0.0f)
	{
		mFrequencyTime = 1.0f / mFrequency;
	}
	else
	{
		mFrequencyTime = 0.0f;
		mCurrentFrequencyTime = 0.0f;
	}
}

void VBeamElement::setVisibilityFlags(VUINT32 flags)
{
	if (mBillboardChain != VNULL)
	{
		mBillboardChain->setVisibilityFlags(flags);
	}
}

void VBeamElement::sleep()
{
	if (mBillboardChain != VNULL)
	{
		mBillboardChain->setEnabled(VFALSE);
	}

	VEffectElement::sleep();
}

VBOOL VBeamElement::_createBillboardChain()
{
	Ogre::SceneNode *parent = mBaseNode;

	VBOOL result = VFALSE;

	if (parent != VNULL)
	{
		mBillboardChain = new Ogre::BillboardChainEx(mNumBillboardElements);
		mBillboardChain->setMaterial(mMaterialName);
		mBillboardChain->updateBoundingBox();
		mBillboardChain->setCastShadows(VFALSE);

		parent->attachObject(mBillboardChain);
		result = VTRUE;
	}

	return result;
}

void VBeamElement::_updateBillboardChain(const Real &dt)
{
	mCurrentFrequencyTime += dt;

	if (mCurrentFrequencyTime > mFrequencyTime || mFrequency == 0.0f)
	{
		if (mDestPos == Ogre::Vector3::UNIT_Y)
		{
			return;
		}

		Ogre::Vector3 originPos = mBaseNode->_getDerivedPosition();

		// 如果初始点还没初始化（在LogicModel::addEffect中会把这个特效的位置设成一个很低的地方），也return
		if (originPos.y < -9000.0f)
		{
			return;
		}

		if (mBillboardChain)
		{
			Ogre::ColourValue color;
			_genColor(color);

			for (VINT32 j = 0; j < mNumBillboardElements; ++j)
			{
				if (j == 0)
				{
					mBillboardChain->updateChainElement(j, 
						Ogre::BillboardChainEx::Element(
						Ogre::Vector3::ZERO,
						mWidth,
						0.0f,
						color)
						);
				}
				else if ( j == (mNumBillboardElements-1) )
				{
					mBillboardChain->updateChainElement(j, 
						Ogre::BillboardChainEx::Element(
						mDestPos - originPos,
						mWidth,
						1.0f,
						color)
						);
				}
				else
				{
					float interpolateValue = (float)j / (float)mNumBillboardElements;
					Ogre::Vector3 pos = (mDestPos - originPos) * interpolateValue;
					pos.x += Ogre::Math::RangeRandom(mNoiseXMin, mNoiseXMax);
					pos.y += Ogre::Math::RangeRandom(mNoiseYMin, mNoiseYMax);
					pos.z += Ogre::Math::RangeRandom(mNoiseZMin, mNoiseZMax);

					mBillboardChain->updateChainElement(j, 
						Ogre::BillboardChainEx::Element(
						pos,
						mWidth,
						interpolateValue,
						color)
						);
				}					
			}

			mBillboardChain->updateBoundingBox();

			// 数据已准备好，可以进行渲染了
			mBillboardChain->setEnabled(VTRUE);
		}

		mCurrentFrequencyTime = 0.0f;
	}
}

void VBeamElement::_destroyBillboardChain()
{
	if (mBillboardChain != VNULL && mBaseNode != VNULL)
	{
		mBaseNode->detachObject(mBillboardChain);

		delete mBillboardChain;
		mBillboardChain = VNULL;
	}
}

void VBeamElement::_genColor(ColorValue &destColor)
{
	if (mColorRangeStart != mColorRangeEnd)
	{
		// Randomise
		//Ogre::Real t = Math::UnitRandom();
		destColor.r = mColorRangeStart.r + (Ogre::Math::UnitRandom() * (mColorRangeEnd.r - mColorRangeStart.r));
		destColor.g = mColorRangeStart.g + (Ogre::Math::UnitRandom() * (mColorRangeEnd.g - mColorRangeStart.g));
		destColor.b = mColorRangeStart.b + (Ogre::Math::UnitRandom() * (mColorRangeEnd.b - mColorRangeStart.b));
		destColor.a = mColorRangeStart.a + (Ogre::Math::UnitRandom() * (mColorRangeEnd.a - mColorRangeStart.a));
	}
	else
	{
		destColor = mColorRangeStart;
	}
}
