

#include "VBillboardSetElement.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VBillboardSetElement::CmdBillboardColor::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getBillboardColor());
}

void VBillboardSetElement::CmdBillboardColor::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setBillboardColor(Ogre::StringConverter::parseColourValue(val));
}

Ogre::String VBillboardSetElement::CmdBillboardAlpha::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getBillboardAlpha());
}

void VBillboardSetElement::CmdBillboardAlpha::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setBillboardAlpha(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBillboardSetElement::CmdBillboardCount::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getBillboardCount());
}

void VBillboardSetElement::CmdBillboardCount::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setBillboardCount(Ogre::StringConverter::parseInt(val));
}

Ogre::String VBillboardSetElement::CmdBillboardMaterial::doGet(const void *target) const
{
	return static_cast<const VBillboardSetElement*>(target)->getBillboardMaterial();
}

void VBillboardSetElement::CmdBillboardMaterial::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setBillboardMaterial(val);
}

Ogre::String VBillboardSetElement::CmdBillboardInterval::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getBillboardInterval());
}

void VBillboardSetElement::CmdBillboardInterval::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setBillboardInterval(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBillboardSetElement::CmdBillboardWidth::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getBillboardWidth());
}

void VBillboardSetElement::CmdBillboardWidth::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setBillboardWidth(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBillboardSetElement::CmdBillboardHeight::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getBillboardHeight());
}

void VBillboardSetElement::CmdBillboardHeight::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setBillboardHeight(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBillboardSetElement::CmdBillboardScale::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getBillboardScale(mIndex));
}

void VBillboardSetElement::CmdBillboardScale::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setBillboardScale(mIndex, Ogre::StringConverter::parseVector3(val));
}

Ogre::String VBillboardSetElement::CmdScaleIntervalTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getScaleIntervalTime());
}

void VBillboardSetElement::CmdScaleIntervalTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setScaleIntervalTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VBillboardSetElement::CmdStacks::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getStacks());
}

void VBillboardSetElement::CmdStacks::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setStacks(Ogre::StringConverter::parseUnsignedLong(val));
}

Ogre::String VBillboardSetElement::CmdSlices::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getSlices());
}

void VBillboardSetElement::CmdSlices::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setSlices(Ogre::StringConverter::parseUnsignedLong(val));
}

Ogre::String VBillboardSetElement::CmdTexIntervalTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VBillboardSetElement*>(target)->getTexIntervalTime());
}

void VBillboardSetElement::CmdTexIntervalTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VBillboardSetElement*>(target)->setTexIntervalTime(Ogre::StringConverter::parseReal(val));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// VBillboardSetElement
////////////////////////////////////////////////////////////////////////////////////////////////////

VBillboardSetElement::CmdBillboardColor VBillboardSetElement::msBillboardColorCmd;
VBillboardSetElement::CmdBillboardScale VBillboardSetElement::msBillboardScaleCmd[MAX_STAGES];
VBillboardSetElement::CmdScaleIntervalTime VBillboardSetElement::msScaleIntervalTimeCmd;
VBillboardSetElement::CmdBillboardAlpha VBillboardSetElement::msBillboardAlphaCmd;
VBillboardSetElement::CmdBillboardCount VBillboardSetElement::msBillboardCountCmd;
VBillboardSetElement::CmdBillboardMaterial VBillboardSetElement::msBillboardMaterialCmd;
VBillboardSetElement::CmdBillboardInterval VBillboardSetElement::msBillboardIntervalCmd;
VBillboardSetElement::CmdBillboardHeight VBillboardSetElement::msBillboardHeightCmd;
VBillboardSetElement::CmdBillboardWidth VBillboardSetElement::msBillboardWidthCmd;
VBillboardSetElement::CmdStacks VBillboardSetElement::msStacksCmd;
VBillboardSetElement::CmdSlices VBillboardSetElement::msSlicesCmd;
VBillboardSetElement::CmdTexIntervalTime VBillboardSetElement::msTexIntervalTimeCmd;


VBillboardSetElement::VBillboardSetElement(const VString &type)
	: VEffectElement(type)
	, mBillboardMaterial("BaseWhiteNoLighting")
	, mBillboardColor(Ogre::ColourValue::White)
	, mBillboardInterval(10.0f)
	, mBillboardWidth(100.0f)
	, mBillboardHeight(100.0f)
	, mScaleIntervalTime(0.1f)
	, mAlphaValue(1.0f)
	, mTexIntervalTime(0.1f)
	, mCurrentTexIntervalTime(0.0f)
	, mBillboardSet(VNULL)
	, mBillboardCount(5)
	, mStacks(1)
	, mSlices(1)
	, mSizeChanged(VFALSE)
	, mColorChanged(VFALSE)
{
	for (VINT32 i=0; i<MAX_STAGES; ++i)
	{
		mBillboardScale[i] = Ogre::Vector3(1.0f, 1.0f, 1.0f);
	}

	if (_initDefaults(mElementType))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();

		// Custom params      

		dict->addParameter(Ogre::ParameterDef("BillboardColor", 
			"BillboardColor",
			Ogre::PT_COLOURVALUE), 
			&msBillboardColorCmd);

		dict->addParameter(Ogre::ParameterDef("BillboardAlpha", 
			"Billboard Alpha Value of first point and second point.",
			Ogre::PT_REAL),
			&msBillboardAlphaCmd);

		dict->addParameter(Ogre::ParameterDef("BillboardCount", 
			"the life time of the vertex.",
			Ogre::PT_INT),
			&msBillboardCountCmd);

		dict->addParameter(Ogre::ParameterDef("Material", 
			"alpha factor of head point.",
			Ogre::PT_STRING),
			&msBillboardMaterialCmd);

		dict->addParameter(Ogre::ParameterDef("BillboardInterval", 
			"alpha factor of tail point.",
			Ogre::PT_REAL),
			&msBillboardIntervalCmd);

		dict->addParameter(Ogre::ParameterDef("BillboardHeight", 
			"alpha factor of tail point.",
			Ogre::PT_REAL),
			&msBillboardHeightCmd);

		dict->addParameter(Ogre::ParameterDef("BillboardWidth", 
			"alpha factor of tail point.",
			Ogre::PT_REAL),
			&msBillboardWidthCmd); 

		for (int i=0;i<MAX_STAGES;i++)
		{
			msBillboardScaleCmd[i].mIndex = i;

			Ogre::StringUtil::StrStreamType stage;
			stage << i;
			Ogre::String colour_title = Ogre::String("BillboardScale") + stage.str();
			Ogre::String colour_descr = Ogre::String("Stage ") + stage.str() +
				Ogre::String(" BillboardScale of first point and second point.");

			dict->addParameter(Ogre::ParameterDef(colour_title, 
				colour_descr,
				Ogre::PT_VECTOR3), 
				&msBillboardScaleCmd[i]);
		}

		dict->addParameter(Ogre::ParameterDef("ScaleIntervalTime", 
			"the interval time of changing scale.",
			Ogre::PT_REAL),
			&msScaleIntervalTimeCmd);   

		dict->addParameter(Ogre::ParameterDef("stacks", 
			"the stacks of texture coordinates.",
			Ogre::PT_UNSIGNED_LONG),
			&msStacksCmd);

		dict->addParameter(Ogre::ParameterDef("slices",
			"the slices of texture coordinates.",
			Ogre::PT_UNSIGNED_LONG),
			&msSlicesCmd);

		dict->addParameter(Ogre::ParameterDef("TexIntervalTime", 
			"the interval time of changing texture index.",
			Ogre::PT_REAL),
			&msTexIntervalTimeCmd);   
	}
}

VBillboardSetElement::~VBillboardSetElement()
{
	destroyRenderInstance();
}

VBOOL VBillboardSetElement::createRenderInstance()
{
	return _createBillboardSet();
}

void VBillboardSetElement::updateRenderInstance(const Real &dt, const VTransform &xform)
{
	if (mBillboardSet != VNULL)
	{
		VBOOL updateTexIndex = VFALSE;

		if (mSlices > 1 || mStacks > 1)
		{
			mCurrentTexIntervalTime += dt;

			if (mCurrentTexIntervalTime >= mTexIntervalTime)
			{
				// 间隔时间到，需要更新纹理索引
				updateTexIndex = VTRUE;
				mCurrentTexIntervalTime = 0.0f;
			}
		}

		for (VINT32 i = 0; i < mBillboardSet->getNumBillboards(); ++i)
		{
			Ogre::Billboard *billboard = mBillboardSet->getBillboard(i);

			if (updateTexIndex)
			{
				// 更新纹理索引
				VWORD texIndex = billboard->getTexcoordIndex();
				++texIndex;

				if (texIndex == mStacks * mSlices)
					texIndex = 0;

				billboard->setTexcoordIndex(texIndex);
			}

			if (mColorChanged)
			{
				// billboard颜色改变，重新设置颜色
				billboard->setColour(mBillboardColor * mAlphaValue);
			}

			if (mSizeChanged)
			{
				// billboard大小改变，重新设置大小
				VScaleInfo &info = mCurrentScaleInfos[i];
				info.mCurrentScaleIntervalTime += dt;

				VFLOAT colorPresent = fmod(info.mCurrentScaleIntervalTime, mScaleIntervalTime);

				if (info.mCurrentScaleIntervalTime > mScaleIntervalTime)
				{
					// 超过大小改变时间间隔，设置当前索引
					info.mCurrentScaleIntervalTime = colorPresent;
					++info.mCurrentIndex;

					if (info.mCurrentIndex == MAX_STAGES)
						info.mCurrentIndex = 0;
				}

				Ogre::Vector3 resultColor;
				Ogre::Real rate = colorPresent / mScaleIntervalTime;
				VINT32 incIndex = info.mCurrentIndex + 1;
				if (incIndex == MAX_STAGES)
					incIndex = 0;

				resultColor = mBillboardScale[info.mCurrentIndex] * (1 - rate) + mBillboardScale[incIndex] * rate;
				billboard->setDimensions(mBillboardWidth * resultColor.x, mBillboardHeight * resultColor.y);
			}
		}

		if (mColorChanged)
			mColorChanged = VFALSE;
	}
}

void VBillboardSetElement::destroyRenderInstance()
{
	_destoryBillboardSet();

	VEffectElement::destroyRenderInstance();
}

void VBillboardSetElement::setVisibilityFlags(VUINT32 flags)
{
	if (mBillboardSet != VNULL)
	{
		mBillboardSet->setVisibilityFlags(flags);
	}
}

void VBillboardSetElement::setBillboardColor(const ColorValue &color)
{
	if (mBillboardColor != color)
	{
		mBillboardColor = color;
		mColorChanged = VTRUE;
	}
}

void VBillboardSetElement::setBillboardScale(VINT32 index, const Vector3 &scale)
{
	if (mBillboardScale[index] != scale)
	{
		mBillboardScale[index] = scale;
		mSizeChanged = VTRUE;
	}
}

void VBillboardSetElement::setBillboardCount(VINT32 count)
{
	mBillboardCount = count;

	if (mBillboardSet != VNULL)
	{
		mBillboardSet->clear();

		for (VINT32 i = 0; i < mBillboardCount; ++i)
		{
			Ogre::Billboard *billboard = mBillboardSet->createBillboard(0, i*mBillboardInterval, 0, mBillboardColor * mAlphaValue);
			assert(billboard != VNULL);
			billboard->setDimensions(mBillboardWidth, mBillboardHeight);
		}

		_resetBillboardStartTexIndex();
		_resetBillboardStartScaleTime();
	}
}

void VBillboardSetElement::setBillboardMaterial(const VString &material)
{
	mBillboardMaterial = material;

	if (mBillboardSet != VNULL)
	{
		mBillboardSet->setMaterialName(material);
	}
}

void VBillboardSetElement::setBillboardInterval(const Real &interval)
{
	mBillboardInterval = interval;

	if (mBillboardSet != VNULL)
	{
		for (VINT32 i = 0; i < mBillboardSet->getNumBillboards(); ++i)
		{
			mBillboardSet->getBillboard(i)->setPosition(0, i*mBillboardInterval, 0);
		}

		_resetBillboardStartScaleTime();
	}
}

void VBillboardSetElement::setBillboardWidth(const Real &width)
{
	mBillboardWidth = width;

	if (mBillboardSet != VNULL)
	{
		for (VINT32 i = 0; i < mBillboardSet->getNumBillboards(); ++i)
		{
			mBillboardSet->getBillboard(i)->setDimensions(mBillboardWidth, mBillboardHeight);
		}
	}
}

void VBillboardSetElement::setBillboardHeight(const Real &height)
{
	mBillboardHeight = height;

	if (mBillboardSet != VNULL)
	{
		for (VINT32 i = 0; i < mBillboardSet->getNumBillboards(); ++i)
		{
			mBillboardSet->getBillboard(i)->setDimensions(mBillboardWidth, mBillboardHeight);
		}
	}
}

void VBillboardSetElement::setStacks(VULONG stacks)
{
	if (stacks < 1)
	{
		mStacks = 1;
	}
	else
	{
		mStacks = stacks;
	}

	if (mBillboardSet != VNULL)
	{
		mBillboardSet->setTextureStacksAndSlices((VBYTE)mStacks, (VBYTE)mSlices);
		_resetBillboardStartTexIndex();
	}
}

void VBillboardSetElement::setSlices(VULONG slices)
{
	if (slices < 1)
	{
		mSlices = 1;
	}
	else
	{
		mSlices = slices;
	}

	if (mBillboardSet != VNULL)
	{
		mBillboardSet->setTextureStacksAndSlices((VBYTE)mStacks, (VBYTE)mSlices);
		_resetBillboardStartTexIndex();
	}
}

VBOOL VBillboardSetElement::_createBillboardSet()
{
	VBOOL result = VFALSE;
	Ogre::SceneNode *parent = mBaseNode;

	if (parent != VNULL)
	{
		VGraphicsSystem *gfxSystem = VENGINE.getGfxSystem();
		mBillboardSet = gfxSystem->getSceneManager()->createBillboardSet(parent->getName());
		mBillboardSet->setMaterialName(mBillboardMaterial);

		if (mStacks > 1 || mSlices > 1)
		{
			mBillboardSet->setTextureStacksAndSlices((VBYTE)mStacks, (VBYTE)mSlices);
		}

		mBillboardSet->setCastShadows(VFALSE);
		mBillboardSet->setVisible(mVisible);

		parent->attachObject(mBillboardSet);

		for (VINT32 i = 0; i < mBillboardCount; ++i)
		{
			Ogre::Billboard *billboard = mBillboardSet->createBillboard(0, i*mBillboardInterval, 0, mBillboardColor * mAlphaValue);
			assert(billboard != VNULL);
			billboard->setDimensions(mBillboardWidth, mBillboardHeight);
		}

		_resetBillboardStartTexIndex();
		_resetBillboardStartScaleTime();

		result = VTRUE;
	}

	return result;
}

void VBillboardSetElement::_destoryBillboardSet()
{
	if (mBillboardSet != VNULL)
	{
		Ogre::SceneNode *parent = mBillboardSet->getParentSceneNode();
		if (parent != VNULL)
			parent->detachObject(mBillboardSet->getName());

		VGraphicsSystem *gfxSystem = VENGINE.getGfxSystem();
		gfxSystem->getSceneManager()->destroyBillboardSet(mBillboardSet);
	}
}

void VBillboardSetElement::_resetBillboardStartTexIndex()
{
	assert(mBillboardSet != VNULL);

	for (VINT32 i = 0; i < mBillboardCount; ++i)
	{
		VWORD texIndex = Ogre::Math::RangeRandom(0, mStacks * mSlices - 1);
		mBillboardSet->getBillboard(i)->setTexcoordIndex(texIndex);
	}
}

void VBillboardSetElement::_resetBillboardStartScaleTime()
{
	mCurrentScaleInfos.resize(mBillboardCount);

	for (VINT32 i = 0; i < mBillboardCount; ++i)
	{
		mCurrentScaleInfos[i].mCurrentScaleIntervalTime = Ogre::Math::RangeRandom(0.0f, mScaleIntervalTime);
		mCurrentScaleInfos[i].mCurrentIndex = (VINT32)(Ogre::Math::RangeRandom(0, MAX_STAGES));
	}
}
