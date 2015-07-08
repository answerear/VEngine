

#ifndef __VISION_PROJECTOR_ELEMENT_H__
#define __VISION_PROJECTOR_ELEMENT_H__


#include "VEffectElement.h"


class VProjector;


class VProjectorElement : public VEffectElement
{
protected:
	class CmdMaterialName : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdProjectionSize : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VProjectorElement(const VString &type);
	~VProjectorElement();

	virtual VBOOL createRenderInstance();
	virtual void updateRenderInstance(const Real &dt, const VTransform &xform);
	virtual void destroyRenderInstance();

	virtual void setVisibilityFlags(VUINT32 flags);

	void setMaterialName(const VString &name);
	const VString &getMaterialName() const
	{
		return mMaterialName;
	}

	void setProjectionSize(const Real &size);
	const Real getProjectionSize() const
	{
		return mProjectionSize;
	}

protected:
	static CmdMaterialName msMaterialNameCmd;
	static CmdProjectionSize msProjectionSizeCmd;


	VProjector	*mProjector;	/**< 投影对象 */

	VString		mMaterialName;	/**< 材质名称 */
	Ogre::Real	mProjectionSize;
};


#endif	/*__VISION_PROJECTOR_ELEMENT_H__*/
