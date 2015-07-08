

#include "VEffectBase.h"


VEffectBase::VEffectBase()
	: mBaseNode(VNULL)
	, mVisible(VTRUE)
{

}

VEffectBase::~VEffectBase()
{
	if (mBaseNode != VNULL)
	{
		mBaseNode->getCreator()->destroySceneNode(mBaseNode);
		mBaseNode = VNULL;
	}
}

VBOOL VEffectBase::existVisibleElements() const
{
	return VFALSE;
}

void VEffectBase::setVisible(VBOOL visible)
{
	if (visible != mVisible)
	{
		mVisible = visible;

		if (mBaseNode != VNULL)
		{
			mBaseNode->setVisible(visible);
		}
	}
}

VBOOL VEffectBase::isVisible() const
{
	return mVisible;
}

void VEffectBase::setColor(const ColorValue &color)
{

}
