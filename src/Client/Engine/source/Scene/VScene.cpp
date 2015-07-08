

#include "VScene.h"
#include "VSceneInfo.h"
#include "VTerrain.h"
#include "VSceneSerializer.h"
#include "VisionXMLParser.h"
#include "VisionXMLWriter.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"
#include "VStaticEntityObject.h"

#include <boost/lambda/lambda.hpp>


const VString VScene::DEFAULT_AUTO_NAME_PREFIX = "Unnamed_";


VScene::VScene()
	: mTerrain(VNULL)
	, mObjects()
	, mAttributes()
	, mAutoNamePrefix(DEFAULT_AUTO_NAME_PREFIX)
	, mAutoNameIndex(0)
{
	mTerrain = new VTerrain();
}


VScene::~VScene()
{
	delete mTerrain;
	mTerrain = VNULL;
}


// 读取场景数据
void VScene::load(XMLParser *parser, const VString &filename, const VString &groupName, VBOOL completely)
{
	assert(parser);

	if (filename.empty())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
			"Filename supplied for terrain loading must be valid",
			"VScene::load");
	}

	reset();

	// setup handler object
	VSceneXMLHandler handler(this);
	
	// parse the scene XML file
	parser->parseXMLFile(handler, filename, groupName);

	// load terrain data
	assert(mTerrain);
	mTerrain->load(parser, mTerrainFilename, groupName, completely);
}

// 保存场景数据
void VScene::save(XMLWriter *writer, const VString &filename) const
{

}

// 创建场景
void VScene::create()
{
	mTerrain->create();

	for (Objects::const_iterator itr = mObjects.begin(); itr != mObjects.end(); ++itr)
	{
		const VSceneObjectPtr& object = *itr;
		if (object->getType() != "SkyDome")
		{
			object->createRenderInstance();
		}
	}

	_bakeStaticGeometries();
}

// 销毁场景
void VScene::destroy()
{
	mTerrain->destroy();
}

void VScene::reset()
{
	VSceneInfo::reset();

	mObjects.clear();
	mAttributes.clear();

	mAutoNamePrefix = DEFAULT_AUTO_NAME_PREFIX;
	mAutoNameIndex = 0;
}

void VScene::setAutoNamePrefix(const VString &newAutoNamePrefix)
{
	Objects::index<by_sequenced>::type &indices = mObjects.get<by_sequenced>();
	Objects::index<by_sequenced>::type::iterator itr;
	for (itr = indices.begin(); itr != indices.end(); ++itr)
	{
		if (isAutoName((*itr)->getName()))
		{
			VString newName = (*itr)->getName();
			newName.replace(0, mAutoNamePrefix.length(), newAutoNamePrefix);
			mObjects.get<by_name>().modify_key(mObjects.project<by_name>(itr), boost::lambda::_1 = newName);
		}
	}

	mAutoNamePrefix = newAutoNamePrefix;
}

VString VScene::generateAutoName(const VSceneObjectPtr &object)
{
	assert(object);
	VString objectType = object->getType();
	assert(!objectType.empty());
	return mAutoNamePrefix + objectType + "_" + Ogre::StringConverter::toString(mAutoNameIndex++, 5, '0');
}

VBOOL VScene::isAutoName(const VString &name) const
{
	return Ogre::StringUtil::startsWith(name, mAutoNamePrefix, false);
}

void VScene::addObject(const VSceneObjectPtr &object)
{
	if (!mObjects.get<by_identity>().insert(object).second)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
			"The given object with name '" + object->getName() + "' already contain by this scene",
			"VScene::addObject");
	}
}

void VScene::removeObject(const VString &name)
{
	if (mObjects.get<by_name>().erase(name) <= 0)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"The given object with name '" + name + "' doesn't contain by this scene",
			"VScene::removeObject");
	}
}

void VScene::removeObject(const VSceneObjectPtr &object)
{
	if (mObjects.get<by_identity>().erase(object) <= 0)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"The given object with name '" + object->getName() + "' doesn't contain by this scene",
			"VScene::removeObject");
	}
}

void VScene::renameObject(const VString &oldName, const VString &newName)
{
	Objects::index<by_name>::type& name_index = mObjects.get<by_name>();
	Objects::index<by_name>::type::iterator it = name_index.find(oldName);
	if (it == name_index.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"Can't found object with name '" + oldName + "'",
			"VScene::renameObject");
	}

	name_index.modify_key(it, boost::lambda::_1 = newName);
}

void VScene::renameObject(const VSceneObjectPtr &object, const VString &newName)
{
	Objects::index<by_identity>::type& identity_index = mObjects.get<by_identity>();
	Objects::index<by_identity>::type::iterator it = identity_index.find(object);
	if (it == identity_index.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"The given object with name '" + object->getName() + "' doesn't contain by this scene",
			"VScene::renameObject");
	}

	mObjects.get<by_name>().modify_key(mObjects.project<by_name>(it), boost::lambda::_1 = newName);
}

const VSceneObjectPtr &VScene::findObjectByName(const VString &name) const
{
	const Objects::index<by_name>::type &name_index = mObjects.get<by_name>();
	Objects::index<by_name>::type::const_iterator itr = name_index.find(name);
	if (itr == name_index.end())
	{
		static const VSceneObjectPtr tmp;
		return tmp;
	}

	return *itr;
}

VScene::ObjectsByTypeRange VScene::findObjectsByType(const VString &type) const
{
	const Objects::index<by_type>::type& type_index = mObjects.get<by_type>();
	return ObjectsByTypeRange(type_index.lower_bound(type), type_index.upper_bound(type));
}

VScene::ObjectsByCategoryRange VScene::findObjectsByCategory(const VString &category) const
{
	const Objects::index<by_category>::type& category_index = mObjects.get<by_category>();
	return ObjectsByCategoryRange(category_index.lower_bound(category), category_index.upper_bound(category));
}

VBOOL VScene::existAttribute(const VString &name)
{
	return mAttributes.find(name) != mAttributes.end();
}

const VString &VScene::getAttribute(const VString &name) const
{
	Attributes::const_iterator itr = mAttributes.find(name);
	if (itr == mAttributes.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"The attribute with name '" + name + "' doesn't exists",
			"VScene::getAttribute");
	}

	return itr->second;
}

void VScene::setAttribute(const VString &name, const VString &value)
{
	mAttributes[name] = value;
}

void VScene::removeAttribute(const VString &name)
{
	Attributes::iterator itr = mAttributes.find(name);
	if (itr == mAttributes.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"The attribute with name '" + name + "' doesn't exists",
			"VScene::removeAttribute");
	}

	mAttributes.erase(itr);
}

void VScene::_bakeStaticGeometries()
{
	_clearStaticGeometries();

	VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();
	Ogre::SceneManager *sceneManager = pGfxSystem->getSceneManager();

	Ogre::StaticGeometry *sg = VNULL;
// 	sg = sceneManager->createStaticGeometry("Fairy/StaticGeometry/0");
// 	mStaticGeometries.push_back(sg);
// 	sg->setRegionDimensions(Ogre::Vector3(2000, 2000, 2000));
// 
// 	const Objects& objects = mObjects;
// 	for (Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
// 	{
// 		const VSceneObjectPtr& object = *it;
// 		VEntityList entities;
// 		object->queryBakeableEntities(entities);
// 		if (!entities.empty())
// 		{
// 			object->bakeStaticGeometry(sg, entities);
// 		}
// 	}
// 
// 	sg->build();
// 
// 	return;

	Real size = mTerrain->mInfo->getTileSize() * mTerrain->mInfo->getScale().x;

	// 1.build mesh bucket
	typedef std::multimap<Ogre::ResourceHandle, VSceneObjectPtr> MeshBucketMap;
	MeshBucketMap meshBucket;
	ObjectsByTypeRange objests = findObjectsByType(VStaticEntityObject::getFactory()->getType());
	for (ObjectsByTypeIterator itr = objests.first; itr != objests.second; ++ itr)
	{
		const VSceneObjectPtr &object = *itr;
		VEntityList entities;
		object->queryBakeableEntities(entities);

		if (!entities.empty())
		{
			Ogre::ResourceHandle handle = entities.front()->getMesh()->getHandle();
			meshBucket.insert(MeshBucketMap::value_type(handle, object));
		}
	}

	// 2.bake StaticGeometry
	Ogre::ResourceHandle last = 0;
	VINT32 i = 0;
	MeshBucketMap::iterator it;
	for (it = meshBucket.begin(); it != meshBucket.end(); ++it)
	{
		if (it->first != last) // key changed
		{
			// build old
			if (sg)
			{
				sg->build();
			}

			// create new
			sg = sceneManager->createStaticGeometry("StaticGeometry" + Ogre::StringConverter::toString(i));
			sg->setRegionDimensions(Ogre::Vector3(size, size, size));
			mStaticGeometries.push_back(sg);
			++ i;
		}

		// bake
		VSceneObjectPtr &object = it->second;
		VEntityList entities;
		object->queryBakeableEntities(entities);
		object->bakeStaticGeometry(sg, entities);

		last = it->first;
	}

	// 最后一个,bulid
	if (sg)
	{
		sg->build();
	}
}

void VScene::_clearStaticGeometries()
{
	VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();
	Ogre::SceneManager *sceneManager = pGfxSystem->getSceneManager();

	VStaticGeometryList::const_iterator it;
	for (it = mStaticGeometries.begin(); it != mStaticGeometries.end(); ++it)
	{
		Ogre::StaticGeometry *sg = *it;
		sceneManager->destroyStaticGeometry(sg->getName());
	}

	mStaticGeometries.clear();
}
