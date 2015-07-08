

#ifndef __VISION_ACTOR_OBJECT_H__
#define __VISION_ACTOR_OBJECT_H__


#include "VPlacementObject.h"
#include "VSceneObjectFactory.h"


class VActor;


class VActorObject : public VPlacementObject
{
	friend class VActorObjectFactory;

public:
	VActorObject();
	virtual ~VActorObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	VActor *getActor() const
	{
		return mActor;
	}

	const VString &getActorName() const
	{
		return mActorName;
	}
	void setActorName(const VString &actorName);

	Variant getProperty(const VString &name) const;
	VString getPropertyAsString(const VString &name) const;

	static VSceneObjectFactory *getFactory();

protected:
	void _createActor();
	void _destroyActor();

protected:
	static const VString msType;
	static const VString msCategory;

	VActor	*mActor;
	VString	mActorName;
};


#endif	/*__VISION_ACTOR_OBJECT_H__*/
