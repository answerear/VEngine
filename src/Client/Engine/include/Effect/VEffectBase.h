
#ifndef __VISION_EFFECT_BASE_H__
#define __VISION_EFFECT_BASE_H__


#include "VEngineConfig.h"
#include "VTransform.h"


typedef std::vector<VTransform> VTransforms;

/** 特效基类 */
class VEffectBase
{
public:
	VEffectBase();
	virtual ~VEffectBase();

	virtual void setTransform(const VTransform &xfrom) = 0;
	virtual VBOOL existVisibleElements() const;

	virtual void setVisible(VBOOL visible);
	virtual VBOOL isVisible() const;

	virtual void setColor(const ColorValue &color);

	virtual void setVisibilityFlags(VUINT32 flags) = 0;

protected:
	Ogre::SceneNode	*mBaseNode;	/**< 特效的Scene node */
	VBOOL	mVisible;			/**< 是否可见 */
};


#endif	/*__VISION_EFFECT_BASE_H__*/
