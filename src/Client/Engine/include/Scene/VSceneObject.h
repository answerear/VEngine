

#ifndef __VISION_SCENE_OBJECT_H__
#define __VISION_SCENE_OBJECT_H__


#include "VEngineConfig.h"
#include "VProperty.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


typedef std::list<Ogre::Entity*> VEntityList;


class VSceneObject 
	: public VPropertyInterface
	, public boost::enable_shared_from_this<VSceneObject>
{
	friend struct ObjectSetIndices;
	
public:
	VSceneObject();
	virtual ~VSceneObject();

	const VString &getName() const
	{
		return mName;
	}
	void setName(const VString &name)
	{
		mName = name;
	}

	VULONG getData() const
	{
		return mData;
	}
	void setData(VULONG data)
	{
		mData = data;
	}

	virtual const VString &getType() const = 0;
	virtual const VString &getCategory() const = 0;

	virtual void createRenderInstance() = 0;
	virtual void destroyRenderInstance() = 0;

	virtual void queryBakeableEntities(VEntityList &entities);
	virtual void bakeStaticGeometry(Ogre::StaticGeometry *staticGeometry, const VEntityList &entities);

public:
	VString	mName;
	VULONG	mData;
};


typedef boost::shared_ptr<VSceneObject> VSceneObjectPtr;


#endif	/*__VISION_SCENE_OBJECT_H__*/
