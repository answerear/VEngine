

#include "VEffectElementFactory.h"
#include "VParticleElement.h"
#include "VBillboardSetElement.h"
#include "VBeamElement.h"
#include "VPointLightElement.h"
#include "VProjectorElement.h"

VEffectElementFactory::VEffectElementFactory()
{

}

VEffectElementFactory::~VEffectElementFactory()
{

}

void VEffectElementFactory::destroyElement(VEffectElement *element)
{
	assert(element != VNULL);

	VElements::iterator itr = mElements.begin();
	while (itr != mElements.end())
	{
		if ((*itr) == element)
		{
			delete element;
			element = VNULL;
			itr = mElements.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////

VParticleElementFactory::VParticleElementFactory()
{

}

VParticleElementFactory::~VParticleElementFactory()
{

}

VString VParticleElementFactory::getName() const
{
	return "Particle";
}

VEffectElement *VParticleElementFactory::createElement()
{
	VEffectElement *element = new VParticleElement("Particle");
	assert(element != VNULL);
	mElements.push_back(element);
	return element;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VBillboardSetElementFactory::VBillboardSetElementFactory()
{

}

VBillboardSetElementFactory::~VBillboardSetElementFactory()
{

}

VString VBillboardSetElementFactory::getName() const
{
	return "BillboardSet";
}

VEffectElement *VBillboardSetElementFactory::createElement()
{
	VEffectElement *element = new VBillboardSetElement("BillbaordSet");
	assert(element != VNULL);
	mElements.push_back(element);
	return element;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VProjectorElementFactory::VProjectorElementFactory()
{

}

VProjectorElementFactory::~VProjectorElementFactory()
{

}

VString VProjectorElementFactory::getName() const
{
	return "Projector";
}

VEffectElement *VProjectorElementFactory::createElement()
{
	VEffectElement *element = new VProjectorElement("Projector");
	assert(element != VNULL);
	mElements.push_back(element);
	return element;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VBeamElementFactory::VBeamElementFactory()
{

}

VBeamElementFactory::~VBeamElementFactory()
{

}

VString VBeamElementFactory::getName() const
{
	return "Beam";
}

VEffectElement *VBeamElementFactory::createElement()
{
	VEffectElement *element = new VBeamElement("Beam");
	assert(element != VNULL);
	mElements.push_back(element);
	return element;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VPointLightElementFactory::VPointLightElementFactory()
{

}

VPointLightElementFactory::~VPointLightElementFactory()
{

}

VString VPointLightElementFactory::getName() const
{
	return "PointLight";
}

VEffectElement *VPointLightElementFactory::createElement()
{
	VEffectElement *element = new VPointLightElement("PointLight");
	assert(element != VNULL);
	mElements.push_back(element);
	return element;
}
