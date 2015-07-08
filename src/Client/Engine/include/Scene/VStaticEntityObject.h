
#ifndef __VISION_STATIC_ENTITY_H__
#define __VISION_STATIC_ENTITY_H__


#include "VPlacementObject.h"
#include "VSceneObjectFactory.h"


class VStaticEntityObject : public VPlacementObject
{
	friend class VStaticEntityObjectFactory;
	friend class VEntityMeshNameCmd;
	friend class VEntityAnimationLevelCmd;
	friend class VEntityTransparencyCmd;
	friend class VEntityCastShadowsCmd;
	friend class VEntityReceiveShadowsCmd;
	friend class VEntityReceiveDecalsCmd;

public:
	VStaticEntityObject();
	virtual ~VStaticEntityObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	void queryBakeableEntities(VEntityList &entities);
	void bakeStaticGeometry(Ogre::StaticGeometry *staticGeometry, const VEntityList &entities);

	const VString &getMeshName() const
	{
		return mMeshName;
	}

	void setMeshName(const VString &meshName);

	Real getAnimationLevel() const
	{
		return mAnimationLevel;
	}

	void setAnimationLevel(Real level);

	Real getTransparency() const
	{
		return mTransparency;
	}

	void setTransparency(Real transparency);

	VBOOL getCastShadows() const
	{
		return mCastShadows;
	}

	void setCastShadows(VBOOL castShadows);

	VBOOL getReceiveShadows() const
	{
		return mReceiveShadows;
	}

	void setReceiveShadows(VBOOL receiveShadows);

	VBOOL getReceiveDecals(void) const
	{
		return mReceiveDecals;
	}

	void setReceiveDecals(VBOOL receiveDecals);

	static VSceneObjectFactory *getFactory();

protected:
	void _createEntity();
	void _destroyEntity();

protected:
	static const VString msType;
	static const VString msCategory;

	Ogre::Entity	*mEntity;

	Ogre::String	mMeshName;

	Ogre::Real	mAnimationLevel;
	Ogre::Real	mTransparency;

	VBOOL	mCastShadows;
	VBOOL	mReceiveShadows;
	VBOOL	mReceiveDecals;
};



#endif	/*__VISION_STATIC_ENTITY_H__*/
