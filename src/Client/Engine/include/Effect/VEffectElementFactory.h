

#ifndef __VISION_EFFECT_ELEMENT_FACTORY_H__
#define __VISION_EFFECT_ELEMENT_FACTORY_H__


#include "VEngineConfig.h"


class VEffectElement;


class VEffectElementFactory
{
public:
	VEffectElementFactory();
	virtual ~VEffectElementFactory();

	virtual VString getName() const = 0;
	virtual VEffectElement *createElement() = 0;
	virtual void destroyElement(VEffectElement *element);

protected:
	typedef std::vector<VEffectElement*> VElements;
	VElements mElements;
};


class VParticleElementFactory : public VEffectElementFactory
{
public:
	VParticleElementFactory();
	~VParticleElementFactory();

	virtual VString getName() const;
	virtual VEffectElement *createElement();
};


class VBillboardSetElementFactory : public VEffectElementFactory
{
public:
	VBillboardSetElementFactory();
	~VBillboardSetElementFactory();

	virtual VString getName() const;
	virtual VEffectElement *createElement();
};

class VProjectorElementFactory : public VEffectElementFactory
{
public:
	VProjectorElementFactory();
	~VProjectorElementFactory();

	virtual VString getName() const;
	virtual VEffectElement *createElement();
};

class VBeamElementFactory : public VEffectElementFactory
{
public:
	VBeamElementFactory();
	~VBeamElementFactory();

	virtual VString getName() const;
	virtual VEffectElement *createElement(); 
};

class VPointLightElementFactory : public VEffectElementFactory
{
public:
	VPointLightElementFactory();
	~VPointLightElementFactory();

	virtual VString getName() const;
	virtual VEffectElement *createElement();
};


#endif	/*__VISION_EFFECT_ELEMENT_FACTORY_H__*/
