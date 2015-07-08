
#include "VEffect.h"
#include "VEffectElement.h"
#include "VFlags.h"
#include "VEffectManager.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VEffect::CmdLifeTime::doGet(const void *target) const
{
	return Ogre::StringConverter::toString(static_cast<const VEffect*>(target)->getLifeTime());
}

void VEffect::CmdLifeTime::doSet(void *target, const Ogre::String &val)
{
	static_cast<VEffect*>(target)->setLifeTime(Ogre::StringConverter::parseReal(val));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// VEffect
////////////////////////////////////////////////////////////////////////////////////////////////////

VEffect::CmdLifeTime VEffect::msLifeTimeCmd;
Real VEffect::msDefaultIterationsInterval = 0;

VEffect::VEffect(const VString &templateName)
	: VEffectBase()
	, mTemplateName(templateName)
	, mTransform()
	, mElements()
	, mLifeTime(0.0f)
	, mElapseLifeTime(0.0f)
	, mIterationInterval(msDefaultIterationsInterval)
	, mRemainIterationTime(0.0f)
	, mVisibilityFlags(Vision::OVF_DEFAULT)
	, mIsAlive(VTRUE)
	, mDeleteLaterFlag(VFALSE)
	, mRemoveParentNodeFlag(VFALSE)
	, mNeedSetVisibilityFlag(VFALSE)
	, mUseFreePool(VTRUE)
{
	_initDefaults();
}

VEffect::~VEffect()
{
	destroyAllElements();
}

VEffect::VEffect(const VEffect &other)
{
	_copy(other);
}

VEffect &VEffect::operator =(const VEffect &other)
{
	_copy(other);
	return *this;
}

void VEffect::update(const Real &dt)
{
	// 如果特效还处于活动状态，才更新
	if (mIsAlive)
	{
		if (mIterationInterval > 0)
		{
			// 下次迭代时间间隔存在，先计算是否到下次迭代时间
			mRemainIterationTime += dt;

			while (mRemainIterationTime >= mIterationInterval)
			{
				// 迭代计时到了，需要更新自己和所有子特效
				_updateEffect(mIterationInterval);
				_updateElements(mIterationInterval);
				mRemainIterationTime -= mIterationInterval;
			}
		}
		else
		{
			// 不存在迭代时间间隔，直接更新
			_updateEffect(dt);
			_updateElements(dt);
		}

		if (mNeedSetVisibilityFlag)
		{
			// 对所有的元素进行visible设置
			VElementIterator itr = mElements.begin();
			while (itr != mElements.end())
			{
				VEffectElement *element = *itr;
				assert(element);
				if (element->isActive() && !element->isDestroy())
				{
					element->setVisibilityFlags(mVisibilityFlags);
				}
				++itr;
			}

			mNeedSetVisibilityFlag = VFALSE;
		}
	}
}

void VEffect::setTransform(const VTransform &xform)
{
	mTransform = xform;

	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		VEffectElement *element = *itr;
		assert(element);
		if (element->isActive() && !element->isDestroy())
		{
			element->setTransform(mTransform);
		}
		++itr;
	}
}

void VEffect::setExtraTransforms(const VTransforms &xforms)
{
	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		VEffectElement *element = *itr;
		assert(element);
		element->updateExtraTransforms(xforms);
		++itr;
	}
}

void VEffect::reborn()
{
	mElapseLifeTime = 0.0f;
	mIsAlive = VTRUE;

	mDeleteLaterFlag = VFALSE;
	mRemoveParentNodeFlag = VFALSE;

	mNeedSetVisibilityFlag = VFALSE;
	mVisibilityFlags = Vision::OVF_DEFAULT;

	VElementIterator itr = mElements.begin();

	while (itr != mElements.end())
	{
		VEffectElement *element = *itr;
		assert(element);

		element->active();

		// 设置visible属性为默认值true
		element->setVisible(VTRUE);

		// 设置visible flag为默认值
		element->setVisibilityFlags(Vision::OVF_DEFAULT);

		++itr;
	}
}

void VEffect::addElement(VEffectElement *element)
{
	assert(element);
	mElements.push_back(element);
	element->setParent(this);
}

void VEffect::addElement(const VString &type)
{
	VEffectElement *element = VEffectManager::instance().createElement(type);
	assert(element != VNULL);
	element->createSceneNode(mBaseNode);
	addElement(element);
}

void VEffect::removeElement(VEffectElement *element)
{
	assert(element != VNULL);
	element->sleep();
}

void VEffect::removeAllElements()
{
	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		if (*itr)
		{
			removeElement(*itr);
		}
		++itr;
	}
}

void VEffect::removeFromParentSceneNode()
{
	mBaseNode->getParent()->removeChild(mBaseNode);
}

void VEffect::destroyElement(VEffectElement *element)
{
	assert(element != VNULL);
	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		if ((*itr) == element)
		{
			delete (*itr);
			*itr = VNULL;
			mElements.erase(itr);
			break;
		}

		++itr;
	}
}

void VEffect::destroyAllElements()
{
	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		delete (*itr);
		*itr = VNULL;
		++itr;
	}
}

VEffectElement *VEffect::getElement(VWORD index)
{
	assert(index < mElements.size());
	return mElements[index];
}

Ogre::SceneNode *VEffect::createSceneNode()
{
	if (mBaseNode != VNULL)
		return mBaseNode;

	VGraphicsSystem *gfxSystem = VENGINE.getGfxSystem();
	assert(gfxSystem != VNULL);
	Ogre::SceneNode *baseNode = gfxSystem->getBaseSceneNode();
	assert(baseNode != VNULL);

	mBaseNode = baseNode->createChildSceneNode();
	assert(mBaseNode != VNULL);

	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		if (*itr)
		{
			(*itr)->createSceneNode(mBaseNode);
		}
		++itr;
	}

	return mBaseNode;
}

void VEffect::createSceneNode(Ogre::SceneNode *parentNode)
{
	assert(parentNode != VNULL);

	if (mBaseNode != VNULL)
	{
		if (mBaseNode->getParent())
		{
			mBaseNode->getParent()->removeChild(mBaseNode);
		}

		parentNode->addChild(mBaseNode);
	}
	else
	{
		mBaseNode = parentNode->createChildSceneNode();

		VElementIterator itr = mElements.begin();
		while (itr != mElements.end())
		{
			if (*itr)
			{
				(*itr)->createSceneNode(mBaseNode);
			}
			++itr;
		}
	}
}

void VEffect::shutdown()
{
	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		if (*itr)
		{
			(*itr)->shutdown();
		}
		++itr;
	}
}

VBOOL VEffect::existVisibleElements() const
{
	VElementList::const_iterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		if (*itr)
		{
			if ((*itr)->existVisibleElements())
				return VTRUE;
		}
		++itr;
	}

	return VFALSE;
}

void VEffect::setVisible(VBOOL visible)
{
	VEffectBase::setVisible(visible);

	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		VEffectElement *element = *itr;

		if (element != VNULL && element->isActive() && !element->isDestroy())
		{
			element->setVisible(visible);
		}

		++itr;
	}
}

void VEffect::setVisibilityFlags(VUINT32 flags)
{
	mVisibilityFlags = flags;
	mNeedSetVisibilityFlag = VTRUE;
}

void VEffect::setColor(const ColorValue &color)
{
	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		VEffectElement *element = *itr;

		if (element != VNULL && element->isActive() && !element->isDestroy())
		{
			element->setColor(color);
		}

		++itr;
	}
}

VBOOL VEffect::_initDefaults()
{
	if (createParamDictionary("Effect"))
	{
		Ogre::ParamDictionary* dict = getParamDictionary();

		dict->addParameter(Ogre::ParameterDef("LifeTime", 
			"the life time of this effect.",
			Ogre::PT_REAL),&msLifeTimeCmd);		

		return VTRUE;
	}

	return VFALSE;
}

void VEffect::_updateEffect(const Real &dt)
{
	if (mLifeTime > 0.0f)
	{
		Ogre::Real elapseTime = mElapseLifeTime + dt;

		if (elapseTime > mLifeTime)
		{
			removeAllElements();
			mIsAlive = VFALSE;
		}
		else
		{
			mElapseLifeTime = elapseTime;
		}
	}
}

void VEffect::_updateElements(const Real &dt)
{
	VElementIterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		VEffectElement *element = *itr;

		if (element != VNULL && element->isActive())
		{
			if (element->isDestroy())
			{
				removeElement(element);
			}
			else
			{
				element->update(dt, mTransform);
			}
		}

		++itr;
	}
}

void VEffect::_destroyElement(VWORD index)
{

}

void VEffect::_copy(const VEffect &rhs)
{
	destroyAllElements();

	VElementList::const_iterator itr = rhs.mElements.begin();
	// copy elements
	while (itr != rhs.mElements.end())
	{
		VEffectElement *temElement = (*itr);
		assert (temElement != VNULL);

		VEffectElement *element = VEffectManager::instance().createElement(temElement->getType());
		temElement->copyParametersTo(element);

		addElement(element);

		++itr;
	}

	// copy effect parameter
	rhs.copyParametersTo(this);
}
