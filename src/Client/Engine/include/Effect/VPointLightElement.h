

#ifndef __VISION_POINT_LIGHT_ELEMENT_H__
#define __VISION_POINT_LIGHT_ELEMENT_H__


#include "VEffectElement.h"


class VPointLightElement : public VEffectElement
{
protected:
	class CmdDiffuse : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdSpecular : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdRange : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

	class CmdAttenuationConstant : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void* target, const Ogre::String &val);
	};

	class CmdAttenuationLinear : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void* target, const Ogre::String &val);
	};

	class CmdAttenuationQuadric : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VPointLightElement(const VString &type);
	~VPointLightElement();

	virtual VBOOL createRenderInstance();
	virtual void destroyRenderInstance();

	void setVisibilityFlags(VUINT32 flags);

	void setDiffuse(const Ogre::ColourValue &diffuse);
	const Ogre::ColourValue &getDiffuse() const
	{
		return mDiffuse;
	}

	void setSpecular(const Ogre::ColourValue &specular);
	const Ogre::ColourValue &getSpecular() const
	{
		return mSpecular;
	}

	void setRange(const Ogre::Real &range);
	const Ogre::Real &getRange() const
	{
		return mRange;
	}

	void setAttenuationConstant(const Ogre::Real &constant);
	const Ogre::Real &getAttenuationConstant() const
	{
		return mAttenuationConstant;
	}

	void setAttenuationLinear(const Ogre::Real &linear);
	const Ogre::Real &getAttenuationLinear() const
	{
		return mAttenuationLinear;
	}

	void setAttenuationQuadric(const Ogre::Real &quadric);
	const Ogre::Real &getAttenuationQuadric() const
	{
		return mAttenuationQuadric;
	}

protected:
	VBOOL _createLight();
	void _destroyLight();

protected:
	static CmdDiffuse	msDiffuseCmd;
	static CmdSpecular	msSpecularCmd;
	static CmdRange		msRangeCmd;
	static CmdAttenuationConstant	msAttenuationConstantCmd;
	static CmdAttenuationLinear		msAttenuationLinearCmd;
	static CmdAttenuationQuadric	msAttenuationQuadricCmd;

	Ogre::Light	*mLight;

	Ogre::ColourValue mDiffuse;
	Ogre::ColourValue mSpecular;

	Ogre::Real	mRange;
	Ogre::Real	mAttenuationConstant;
	Ogre::Real	mAttenuationLinear;
	Ogre::Real	mAttenuationQuadric;
};


#endif	/*__VISION_POINT_LIGHT_ELEMENT_H__*/
