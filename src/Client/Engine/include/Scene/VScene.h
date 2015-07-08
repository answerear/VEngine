

#ifndef __VISION_SCENE_H__
#define __VISION_SCENE_H__


#include "VEngineConfig.h"
#include "VSceneInfo.h"
#include "VSceneObject.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>


class VTerrain;
class XMLParser;
class XMLWriter;


struct by_sequenced;
struct by_identity;
struct by_category;
struct by_type;
struct by_name;

struct ObjectSetIndices : boost::multi_index::indexed_by<
	boost::multi_index::sequenced<
		boost::multi_index::tag<by_sequenced>
	>,
	boost::multi_index::ordered_unique<
		boost::multi_index::tag<by_identity>,
		boost::multi_index::identity<VSceneObjectPtr>
	>,
	boost::multi_index::ordered_non_unique<
		boost::multi_index::tag<by_category>,
		BOOST_MULTI_INDEX_CONST_MEM_FUN(VSceneObject, const VString &, getCategory)
	>,
	boost::multi_index::ordered_non_unique<
		boost::multi_index::tag<by_type>,
		BOOST_MULTI_INDEX_CONST_MEM_FUN(VSceneObject, const VString &, getType)
	>,
	boost::multi_index::ordered_unique<
		boost::multi_index::tag<by_name>,
		BOOST_MULTI_INDEX_MEMBER(VSceneObject, VString, mName)
	>
>
{};


class VENGINE_DLL VScene : public VSceneInfo
{
	struct Objects :
		boost::multi_index::multi_index_container<
		VSceneObjectPtr,
		ObjectSetIndices
		>
	{};

	typedef std::map<VString, VString> Attributes;

	typedef Objects::index<by_type>::type::iterator ObjectsByTypeIterator;
	typedef std::pair<ObjectsByTypeIterator, ObjectsByTypeIterator> ObjectsByTypeRange;
	typedef Objects::index<by_category>::type::iterator ObjectsByCategoryIterator;
	typedef std::pair<ObjectsByCategoryIterator, ObjectsByCategoryIterator> ObjectsByCategoryRange;

	typedef std::list<Ogre::StaticGeometry*> VStaticGeometryList;

	friend class VSceneXMLHandler;

public:
	VScene();
	virtual ~VScene();

private:
	VScene(const VScene &);
	VScene &operator =(const VScene &);

public:
	// 读取场景数据
	void load(XMLParser *parser, const VString &filename, const VString &groupName, VBOOL completely);
	// 保存场景数据
	void save(XMLWriter *writer, const VString &filename) const;

	// 创建场景
	void create();
	// 销毁场景
	void destroy();

	void reset();

	void setAutoNamePrefix(const VString &newAutoNamePrefix);
	const VString &getAutoNamePrefix() const         { return mAutoNamePrefix; }

	VString generateAutoName(const VSceneObjectPtr &object);
	VBOOL isAutoName(const VString &name) const;

	void addObject(const VSceneObjectPtr &object);
	void removeObject(const VString &name);
	void removeObject(const VSceneObjectPtr &object);
	void renameObject(const VString &oldName, const VString &newName);
	void renameObject(const VSceneObjectPtr &object, const VString &newName);

	const VSceneObjectPtr &findObjectByName(const VString &name) const;
	ObjectsByTypeRange findObjectsByType(const VString &type) const;
	ObjectsByCategoryRange findObjectsByCategory(const VString &category) const;

	const Objects &getObjects() const
	{
		return mObjects;
	}

	VBOOL existAttribute(const VString &name);
	const VString &getAttribute(const VString &name) const;
	void setAttribute(const VString &name, const VString &value);
	void removeAttribute(const VString &name);

	const Attributes &getAttributes() const
	{
		return mAttributes;
	}

protected:
	void _bakeStaticGeometries();
	void _clearStaticGeometries();

protected:
	static const VString DEFAULT_AUTO_NAME_PREFIX;

	VTerrain	*mTerrain;		/**< 地形对象 */

	Objects		mObjects;
	Attributes	mAttributes;

	VString		mAutoNamePrefix;
	VINT32		mAutoNameIndex;

	VStaticGeometryList	mStaticGeometries;
};


#endif	/*__VISION_SCENE_H__*/
