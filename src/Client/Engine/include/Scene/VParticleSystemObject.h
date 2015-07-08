

#ifndef __VISION_PARTICLE_SYSTEM_OBJECT_H__
#define __VISION_PARTICLE_SYSTEM_OBJECT_H__


#include "VPlacementObject.h"
#include "VSceneObjectFactory.h"


class VParticleSystemObject : public VPlacementObject
{
	friend class VParticleSystemObjectFactory;

public:
	VParticleSystemObject();
	virtual ~VParticleSystemObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	static VSceneObjectFactory *getFactory();

protected:

protected:
	static const VString msType;
	static const VString msCategory;

	Ogre::SceneManager	*mSceneMgr;
};


#endif	/*__VISION_PARTICLE_SYSTEM_OBJECT_H__*/
