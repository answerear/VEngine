

#include "VParticleSystemObject.h"


const VString VParticleSystemObject::msType = "ParticleSystem";
const VString VParticleSystemObject::msCategory = "StaticEntity";


/**************************************************************************************************
 *								class VParticleSystemObjectFactory
 *************************************************************************************************/

class VParticleSystemObjectFactory : public VSceneObjectFactory
{
public:
	const VString &getType() const
	{
		return VParticleSystemObject::msType;
	}

	VSceneObject *createInstance()
	{
		return new VParticleSystemObject();
	}
};


/**************************************************************************************************
 *								Property Command of VParticleSystemObject
 *************************************************************************************************/


/**************************************************************************************************
 *									class VParticleSystemObject
 *************************************************************************************************/

VParticleSystemObject::VParticleSystemObject()
	: VPlacementObject()
	, mSceneMgr(VNULL)
{
	// TODO: Add properties definition.
}

VParticleSystemObject::~VParticleSystemObject()
{
	destroyRenderInstance();
}

const VString &VParticleSystemObject::getType() const
{
	return msType;
}

const VString &VParticleSystemObject::getCategory() const
{
	return msCategory;
}

void VParticleSystemObject::createRenderInstance()
{
	if (VNULL == mSceneMgr)
	{
	}
}

void VParticleSystemObject::destroyRenderInstance()
{
	if (mSceneMgr != VNULL)
	{
	}
}

VSceneObjectFactory *VParticleSystemObject::getFactory()
{
	static VParticleSystemObjectFactory factory;
	return &factory;
}

