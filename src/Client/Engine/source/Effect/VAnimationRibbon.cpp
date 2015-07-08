

#include "VAnimationRibbon.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VAnimationRibbon::CmdPointCount::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getPointCount());
}

void VAnimationRibbon::CmdPointCount::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setPointCount(Ogre::StringConverter::parseUnsignedInt(val));
}

Ogre::String VAnimationRibbon::CmdLifeTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getLifeTime());
}

void VAnimationRibbon::CmdLifeTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setLifeTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationRibbon::CmdStartColor::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getStartColor());
}

void VAnimationRibbon::CmdStartColor::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setStartColor(Ogre::StringConverter::parseColourValue(val));
}

Ogre::String VAnimationRibbon::CmdEndColor::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getEndColor());
}

void VAnimationRibbon::CmdEndColor::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setEndColor(Ogre::StringConverter::parseColourValue(val));
}

Ogre::String VAnimationRibbon::CmdStartAlpha::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getStartAlphaValue());
}

void VAnimationRibbon::CmdStartAlpha::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setStartAlphaValue(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationRibbon::CmdEndAlpha::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getEndAlphaValue());
}

void VAnimationRibbon::CmdEndAlpha::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setEndAlphaValue(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationRibbon::CmdFadeInTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getFadeInTime());
}

void VAnimationRibbon::CmdFadeInTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setFadeInTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationRibbon::CmdFadeOutTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getFadeOutTime());
}

void VAnimationRibbon::CmdFadeOutTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setFadeOutTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationRibbon::CmdMaterial::doGet(const void *target) const
{
	return static_cast<const VAnimationRibbon*>(target)->getMaterialName();
}

void VAnimationRibbon::CmdMaterial::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setMaterialName(val);
}

Ogre::String VAnimationRibbon::CmdRibbonWidth::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getRibbonWidth());
}

void VAnimationRibbon::CmdRibbonWidth::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setRibbonWidth(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationRibbon::CmdAttachTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getAttachTime());
}

void VAnimationRibbon::CmdAttachTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
}

Ogre::String VAnimationRibbon::CmdAttachPoint::doGet(const void *target) const
{
	return static_cast<const VAnimationRibbon*>(target)->getAttachPoint();
}

void VAnimationRibbon::CmdAttachPoint::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setAttachPoint(val);
}

Ogre::String VAnimationRibbon::CmdOffsetPos::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getOffsetPos());
}

void VAnimationRibbon::CmdOffsetPos::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setOffsetPos(Ogre::StringConverter::parseVector3(val));
}

Ogre::String VAnimationRibbon::CmdOffsetRotation::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getOffsetRotation());
}

void VAnimationRibbon::CmdOffsetRotation::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setOffsetRotation(Ogre::StringConverter::parseQuaternion(val));
}

Ogre::String VAnimationRibbon::CmdUseInter::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getUseInterpolation());
}

void VAnimationRibbon::CmdUseInter::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setUseInterpolation(Ogre::StringConverter::parseBool(val));
}

Ogre::String VAnimationRibbon::CmdInterTimes::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getInterpolationTimes());
}

void VAnimationRibbon::CmdInterTimes::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setInterpolationTimes(Ogre::StringConverter::parseInt(val));
}

Ogre::String VAnimationRibbon::CmdVertexLifeTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VAnimationRibbon*>(target)->getVertexLifeTime());
}

void VAnimationRibbon::CmdVertexLifeTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VAnimationRibbon*>(target)->setVertexLifeTime(Ogre::StringConverter::parseReal(val));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// VAniatmionRibbon
////////////////////////////////////////////////////////////////////////////////////////////////////

VAnimationRibbon::CmdPointCount VAnimationRibbon::msPointCountCmd;
VAnimationRibbon::CmdStartColor VAnimationRibbon::msStartColor;
VAnimationRibbon::CmdEndColor VAnimationRibbon::msEndColor;
VAnimationRibbon::CmdStartAlpha VAnimationRibbon::msStartAlpha;
VAnimationRibbon::CmdEndAlpha VAnimationRibbon::msEndAlpha;
VAnimationRibbon::CmdFadeInTime VAnimationRibbon::msFadeInTimeCmd;
VAnimationRibbon::CmdFadeOutTime VAnimationRibbon::msFadeOutTimeCmd;
VAnimationRibbon::CmdMaterial VAnimationRibbon::msMaterial;
VAnimationRibbon::CmdRibbonWidth VAnimationRibbon::msRibbonWidthCmd;
VAnimationRibbon::CmdLifeTime VAnimationRibbon::msLifeTimeCmd;
VAnimationRibbon::CmdAttachTime VAnimationRibbon::msAttachTimeCmd;
VAnimationRibbon::CmdAttachPoint VAnimationRibbon::msAttachPointCmd;
VAnimationRibbon::CmdOffsetPos VAnimationRibbon::msOffsetPosCmd;
VAnimationRibbon::CmdOffsetRotation VAnimationRibbon::msOffsetRotationCmd;
VAnimationRibbon::CmdUseInter VAnimationRibbon::msUseInterCmd;
VAnimationRibbon::CmdInterTimes VAnimationRibbon::msInterTimesCmd;
VAnimationRibbon::CmdVertexLifeTime VAnimationRibbon::msVertexLifeTimeCmd;

VAnimationRibbon::VAnimationRibbon(VWORD pointCount /* = 30 */, VFLOAT lifeTime /* = 1.0f */, 
	VFLOAT ribbonWidth /* = 100.0f */, 
	const Ogre::ColourValue &startColor /* = Ogre::ColourValue::White */, 
	const Ogre::ColourValue &endColor /* = Ogre::ColourValue::White */, 
	VFLOAT startAlpha /* = 1.0f */, VFLOAT endAlpha /* = 1.0f */, VFLOAT vertexLifeTime /* = 0.0f */)
	: mPoints()
	, mVertexBuffer()
	, mBaseNode(VNULL)
	, mFirstPosSpline()
	, mSecondPosSpline()
	, mBoundingRadius(0.0f)
	, mAttachTime(0.0f)
	, mAttachPoint("")
	, mOffsetPos(Ogre::Vector3::ZERO)
	, mOffsetRotation(Ogre::Quaternion::IDENTITY)
	, mPointCount(pointCount)
	, mLifeTime(lifeTime)
	, mElapseLifeTime(0.0f)
	, mRibbonWidth(ribbonWidth)
	, mStartAlphaValue(startAlpha)
	, mEndAlphaValue(endAlpha)
	, mCurrentStartAlphaValue(1.0f)
	, mCurrentEndAlphaValue(1.0f)
	, mFadeInTime(0.0f)
	, mFadeOutTime(1.0f)
	, mVertexLifeTime(vertexLifeTime)
	, mStartColor(startColor)
	, mEndColor(endColor)
	, mInterpolationTimes(2)
	, mUseInterpolation(VFALSE)
	, mBufferCreated(VFALSE)
	, mRenderInstanceCreated(VFALSE)
{
	mPoints.clear();

	if (createParamDictionary("AnimationRibbon"))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();

		dict->addParameter(Ogre::ParameterDef("AttachTime", 
			"the time that showing the ribbon.",
			Ogre::PT_REAL),
			&msAttachTimeCmd);	  

		dict->addParameter(Ogre::ParameterDef("AttachPoint", 
			"the attached bone or locator.",
			Ogre::PT_STRING),
			&msAttachPointCmd);	  

		dict->addParameter(Ogre::ParameterDef("OffsetPos", 
			"offset position to the attach point.",
			Ogre::PT_VECTOR3),
			&msOffsetPosCmd);	  

		dict->addParameter(Ogre::ParameterDef("OffsetRotation", 
			"offset orientation to the attach point.",
			Ogre::PT_QUATERNION),
			&msOffsetRotationCmd);	  

		dict->addParameter(Ogre::ParameterDef("PointCount", 
			"the number of points.",
			Ogre::PT_UNSIGNED_INT),
			&msPointCountCmd);

		dict->addParameter(Ogre::ParameterDef("LifeTime", 
			"the life time of ribbon vertex.",
			Ogre::PT_REAL),
			&msLifeTimeCmd);

		dict->addParameter(Ogre::ParameterDef("RibbonWidth", 
			"the width of ribbon.",
			Ogre::PT_REAL),
			&msRibbonWidthCmd);

		dict->addParameter(Ogre::ParameterDef("StartAlphaValue", 
			"the head point alpha value.",
			Ogre::PT_REAL),
			&msStartAlpha);

		dict->addParameter(Ogre::ParameterDef("EndAlphaValue", 
			"the tail point of alpha value.",
			Ogre::PT_REAL),
			&msEndAlpha);

		dict->addParameter(Ogre::ParameterDef("StartColour", 
			"the colour of head point.",
			Ogre::PT_COLOURVALUE),
			&msStartColor);

		dict->addParameter(Ogre::ParameterDef("EndColour", 
			"the colour of tail point.",
			Ogre::PT_COLOURVALUE),
			&msEndColor);

		dict->addParameter(Ogre::ParameterDef("FadeInTime", 
			"the alpha fade in time.",
			Ogre::PT_REAL),
			&msFadeInTimeCmd);

		dict->addParameter(Ogre::ParameterDef("FadeOutTime", 
			"the alpha fade out time.",
			Ogre::PT_REAL),
			&msFadeOutTimeCmd);

		dict->addParameter(Ogre::ParameterDef("Material", 
			"name of ribbon material.",
			Ogre::PT_STRING),
			&msMaterial);	 

		dict->addParameter(Ogre::ParameterDef("InterTimes", 
			"the time that showing the ribbon.",
			Ogre::PT_INT),
			&msInterTimesCmd);	  

		dict->addParameter(Ogre::ParameterDef("UseInter", 
			"the time that showing the ribbon.",
			Ogre::PT_BOOL),
			&msUseInterCmd);	 

		dict->addParameter(Ogre::ParameterDef("VertexLifeTime", 
			"the life time of vertex.",
			Ogre::PT_REAL),
			&msVertexLifeTimeCmd);
	}
}

VAnimationRibbon::~VAnimationRibbon()
{
	destroyRenderInstance();
}

VBOOL VAnimationRibbon::createRenderInstance()
{
	if (mRenderInstanceCreated)
	{
		mBaseNode->setVisible(VTRUE);
		mElapseLifeTime = 0.0f;
	}
	else
	{
		// 创建scene node
		VGraphicsSystem *gfsSystem = VENGINE.getGfxSystem();
		Ogre::SceneNode *baseNode = gfsSystem->getBaseSceneNode();
		mBaseNode = baseNode->createChildSceneNode();

		Ogre::SceneNode *parentNode = mBaseNode;

		if (parentNode != VNULL)
		{
			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
			mRenderOp.useIndexes = false;

			if (mUseInterpolation)
			{
				_createVertexBuffer(mPointCount * mInterpolationTimes);
			}
			else
			{
				_createVertexBuffer(mPointCount);
			}

			parentNode->attachObject(this);
			setMaterial(m_strMatName);
			setCastShadows(false);

			mRenderInstanceCreated = VTRUE;
		}
	}

	return mRenderInstanceCreated;
}

void VAnimationRibbon::updateRenderInstance(const Real &dt, const VTransform &xform)
{
	if (mRenderInstanceCreated)
		return;

	if (!Ogre::Math::RealEqual(0.0f, mLifeTime))
	{
		// 如果ribbon生命期不为默认值(0.0f)，就需要计算当前的生命期
		mElapseLifeTime += dt;

		if (mElapseLifeTime >= mLifeTime)
		{
			mBaseNode->setVisible(false);
			mPoints.clear();

			return;
		}
	}

	// 如果vertex生命期不为默认值(0.0f)，就需要计算当前的生命期
	if (!Ogre::Math::RealEqual(0.0f, mVertexLifeTime))
	{
		// 给所有的点添加life time，如果这个点的生命期到了，就从列表中删除
		PointIterator itr = mPoints.begin();

		while (itr != mPoints.end())
		{
			Vertex &vertex = *itr;
			vertex.lifeTime += dt;
			if (vertex.lifeTime >= mVertexLifeTime)
			{
				itr = mPoints.erase(itr);
			}
			else
			{
				++itr;
			}				
		}

		// 保证点的个数是2的倍数
		if ((mPoints.size() & 1) != 0)
			mPoints.pop_back();
	}

	Ogre::Vector3 firstPos = xform.position;
	Ogre::Vector3 dir = xform.orientation.xAxis();
	dir.normalise();
	Ogre::Vector3 secondPos = firstPos + dir * mRibbonWidth;

	mPoints.push_front(Vertex(secondPos));
	mPoints.push_front(Vertex(firstPos));

	// 移除多余的点
	while (mPoints.size() > mPointCount)
	{
		mPoints.pop_back();
		mPoints.pop_back();
	}

	mFirstPosSpline.clear();
	mSecondPosSpline.clear();

	std::list<Vertex>::iterator it = mPoints.begin();

	if (mUseInterpolation)
	{
		while (it != mPoints.end())
		{
			mSecondPosSpline.addPoint(it->pos);
			++it;
			mFirstPosSpline.addPoint(it->pos);
			++it;
		}
	}

	// change the vertex buffer,if necessary
	if (!mBufferCreated)
	{
		if (mUseInterpolation)
			_createVertexBuffer(mPointCount * mInterpolationTimes);
		else
			_createVertexBuffer(mPointCount);
	}

	// set the vertex count that will be rendered
	if (mUseInterpolation)
		mRenderOp.vertexData->vertexCount = mPoints.size() * mInterpolationTimes;
	else
		mRenderOp.vertexData->vertexCount = mPoints.size();

	Ogre::RenderSystem *renderSystem = Ogre::Root::getSingleton().getRenderSystem();
	VFLOAT *lockPtr = static_cast<VFLOAT*>(mVertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	VFLOAT count = 0.0f;
	VFLOAT maxSqrtLen = -1.0f;

	Ogre::Vector3 min(Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY);
	Ogre::Vector3 max(Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY);
	
	// 如果alpha值有一个不为0,才需要计算当前的alpha值
	if (!Ogre::Math::RealEqual(1.0f, mStartAlphaValue) || !Ogre::Math::RealEqual(1.0f, mEndAlphaValue))
	{
		// 由于现在lifetime都保留在各个点中了，所以不进行alpha 的插值
		VFLOAT timeRatio = mElapseLifeTime / mLifeTime;

		// 根据当前的生命时间来决定透明度
		if (timeRatio < mFadeInTime)
		{
			// 如果在淡入时间内，就根据比例设置透明度
			VFLOAT ratio = mElapseLifeTime / (mFadeInTime * mLifeTime);
			mCurrentStartAlphaValue *= ratio;
			mCurrentEndAlphaValue *= ratio;
		}
		else if (timeRatio > mFadeOutTime)
		{
			// 如果在淡出时间内，就根据比例设置透明度
			VFLOAT ratio = (mLifeTime - mElapseLifeTime) / (mFadeOutTime * mLifeTime);
			mCurrentStartAlphaValue *= ratio;
			mCurrentEndAlphaValue *= ratio;
		}
		else
		{
			// 在淡入与淡出时间中，就用原透明度
			mCurrentStartAlphaValue = mStartAlphaValue;
			mCurrentEndAlphaValue = mEndAlphaValue;
		}
	}
	else
	{
		mCurrentStartAlphaValue = 1.0f;
		mCurrentEndAlphaValue = 1.0f;
	}

	if (mUseInterpolation)
	{
		// if we use the simple spline to interpolate the ribbon
		// we will render more vertices
		VINT32 numLoop = mPoints.size() / 2 * mInterpolationTimes; 
		for (VINT32 i=0; i<numLoop; ++i)
		{
			float delta = 1.0f;
			if (numLoop > 0.0f)
				delta = count/(numLoop-1);

			float endOpacity = (1.0 - delta) * mCurrentEndAlphaValue;
			float startOpacity = (1.0 - delta) * mCurrentStartAlphaValue;

			// compute the bounding box
			const Ogre::Vector3& pos = mSecondPosSpline.interpolate( (float)i / (float)(numLoop-1) );

			min.makeFloor(pos);
			max.makeCeil(pos);

			maxSqrtLen = std::max(maxSqrtLen, pos.squaredLength());

			// point1
			*lockPtr++ = pos.x;
			*lockPtr++ = pos.y;
			*lockPtr++ = pos.z;

			Ogre::RGBA *pCol;
			pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(lockPtr));

			renderSystem->convertColourValue(Ogre::ColourValue(mEndColor.r * endOpacity, mEndColor.g * endOpacity,
				mEndColor.b * endOpacity, 1.0f), pCol++) ;

			// Update lock pointer
			lockPtr = static_cast<float*>(static_cast<void*>(pCol));

			*lockPtr++ = delta;
			*lockPtr++ = 1.0;

			const Ogre::Vector3& pos1 = mFirstPosSpline.interpolate( (float)i / (float)(numLoop-1) );
			min.makeFloor(pos1);
			max.makeCeil(pos1);

			maxSqrtLen = std::max(maxSqrtLen, pos1.squaredLength());

			// point2
			*lockPtr++ = pos1.x;
			*lockPtr++ = pos1.y;
			*lockPtr++ = pos1.z;

			pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(lockPtr));

			renderSystem->convertColourValue(Ogre::ColourValue(mStartColor.r * startOpacity, mStartColor.g * startOpacity,
				mStartColor.b * startOpacity, 1.0f), pCol++) ;

			// Update lock pointer
			lockPtr = static_cast<float*>(static_cast<void*>(pCol));

			*lockPtr++ = delta;
			*lockPtr++ = 0.0;

			count+=1.0f;
		}

	}
	else
	{
		float pointCount = (float)(mPoints.size());

		while ( it != mPoints.end() )
		{
			float delta = count/(pointCount-2);
			float endOpacity = (1.0 - delta) * mCurrentEndAlphaValue;
			float startOpacity = (1.0 - delta) * mCurrentStartAlphaValue;

			// compute the bounding box
			const Ogre::Vector3& pos = it->pos;

			min.makeFloor(pos);
			max.makeCeil(pos);

			maxSqrtLen = std::max(maxSqrtLen, pos.squaredLength());

			// point1
			*lockPtr++ = it->pos.x;
			*lockPtr++ = it->pos.y;
			*lockPtr++ = it->pos.z;

			Ogre::RGBA *pCol;
			pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(lockPtr));

			renderSystem->convertColourValue(Ogre::ColourValue(mEndColor.r * endOpacity, mEndColor.g * endOpacity,
				mEndColor.b * endOpacity, 1.0f), pCol++) ;

			// Update lock pointer
			lockPtr = static_cast<float*>(static_cast<void*>(pCol));

			*lockPtr++ = delta;
			*lockPtr++ = 1.0;
			++it;

			const Ogre::Vector3& pos1 = it->pos;
			min.makeFloor(pos1);
			max.makeCeil(pos1);

			maxSqrtLen = std::max(maxSqrtLen, pos1.squaredLength());

			// point2
			*lockPtr++ = it->pos.x;
			*lockPtr++ = it->pos.y;
			*lockPtr++ = it->pos.z;

			pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(lockPtr));

			renderSystem->convertColourValue(Ogre::ColourValue(mStartColor.r * startOpacity, mStartColor.g * startOpacity,
				mStartColor.b * startOpacity, 1.0f), pCol++) ;

			// Update lock pointer
			lockPtr = static_cast<float*>(static_cast<void*>(pCol));

			*lockPtr++ = delta;
			*lockPtr++ = 0.0;

			++it;

			count+=2.0f;
		}
	}

	mVertexBuffer->unlock();

	mBox.setExtents(min, max);
	mBoundingRadius = Ogre::Math::Sqrt(maxSqrtLen);
	getParentNode()->needUpdate();
}

void VAnimationRibbon::destroyRenderInstance()
{
	if (mRenderInstanceCreated)
	{
		_destroyVertexBuffer();

		if (mBaseNode != VNULL)
		{
			mBaseNode->getCreator()->destroySceneNode(mBaseNode);
			mBaseNode = VNULL;
		}

		mRenderInstanceCreated = VFALSE;
	}
}

float VAnimationRibbon::getBoundingRadius() const
{
	return mBoundingRadius;
}

float VAnimationRibbon::getSquaredViewDepth(const Ogre::Camera* cam) const
{
	Ogre::Vector3 min, max, mid, dist;

	min = mBox.getMinimum();
	max = mBox.getMaximum();
	mid = ((min - max) * 0.5) + min;
	dist = cam->getDerivedPosition() - mid;

	return dist.squaredLength();
}

void VAnimationRibbon::setPointCount(VWORD count)
{
	// 保证点的个数是2的倍数
	if ((count & 1) != 0)
		--count;

	// we need 4 point to draw a rectangle in OT_TRIANGLE_STRIP mode
	if (count < 4)
		mPointCount = 4;
	else
		mPointCount = count;

	_destroyVertexBuffer();
}

void VAnimationRibbon::setMaterialName(const VString &matName)
{
	m_strMatName = matName;

	if (mRenderInstanceCreated)
		setMaterial(m_strMatName);
}

void VAnimationRibbon::setUseInterpolation(VBOOL use)
{
	mUseInterpolation = use;
	_destroyVertexBuffer();
}

void VAnimationRibbon::setInterpolationTimes(VINT32 times)
{
	mInterpolationTimes = times;
	_destroyVertexBuffer();
}

void VAnimationRibbon::copyParameters(VAnimationRibbon &other) const
{
	other.mPointCount = mPointCount;
	other.mStartColor = mStartColor;
	other.mEndColor = mEndColor;
	other.mStartAlphaValue = mStartAlphaValue;
	other.mEndAlphaValue = mEndAlphaValue;
	other.mFadeInTime = mFadeInTime;
	other.mFadeOutTime = mFadeOutTime;
	other.m_strMatName = m_strMatName;
	other.mRibbonWidth = mRibbonWidth;
	other.mLifeTime = mLifeTime;
	other.mAttachTime = mAttachTime;
	other.mAttachPoint = mAttachPoint;
	other.mOffsetPos = mOffsetPos;
	other.mOffsetRotation = mOffsetRotation;
	other.mUseInterpolation = mUseInterpolation;
	other.mInterpolationTimes = mInterpolationTimes;
	other.mVertexLifeTime = mVertexLifeTime;
}

void VAnimationRibbon::_createVertexBuffer(VINT32 vertexCount)
{
	if (vertexCount == 0)
		return;

	assert(mRenderOp.vertexData == VNULL);
	assert((vertexCount & 1) == 0);

	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.vertexData->vertexCount = vertexCount;
	mRenderOp.vertexData->vertexStart = 0;

	// declaration
	Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

	VINT32 offset = 0;

	// position
	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	// color RGBA
	decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);

	// uv
	decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

	// create vertex buffer
	mVertexBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(0), 
		mRenderOp.vertexData->vertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

	mRenderOp.vertexData->vertexBufferBinding->setBinding(0, mVertexBuffer);

	mBufferCreated = VTRUE;
}

void VAnimationRibbon::_destroyVertexBuffer()
{
	if (mRenderOp.vertexData != VNULL)
	{
		delete mRenderOp.vertexData;
		mRenderOp.vertexData = VNULL;
	}

	mBufferCreated = VFALSE;
}
