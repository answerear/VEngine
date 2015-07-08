

#ifndef __VISION_PROPERTY_H__
#define __VISION_PROPERTY_H__


#include "VEngineConfig.h"


enum VPropertyFlags
{
	PF_PRIMARY      = 1 << 0,   /// The property is primary, the value must be set after object created and before any other method being call.
	PF_DERIVED      = 1 << 1,   /// The property is derived, so setting the value of it just ignored, and is will never use to serialization.
	PF_READONLY     = 1 << 2,   /// The property is read only, it a hint for property editor.
};


/**
 * @class VPropertyInfo
 * @brief 属性项，属性名称、描述、类型、值等信息
 */
class VPropertyInfo
{
public:
	VPropertyInfo(const VString &name, 
		const VString &description, 
		const VString &type,
		const VString &defaultValue = Ogre::StringUtil::BLANK,
		VUINT32 flags = 0)
		: name(name)
		, description(description)
		, type(type)
		, defaultValue(defaultValue)
		, flags(flags)
	{
	}

	VString	name;
	VString	description;
	VString	type;
	VString	defaultValue;
	VUINT32	flags;
};

typedef std::vector<VPropertyInfo> VPropertyList;


/**
 * @class VPropertyCommand
 * @brief 属性对应的命令抽象类，对属性值设置和获取定义接口
 */
class VPropertyCommand
{
public:
	virtual Variant doGet(const void *source) const = 0;
	virtual void doSet(void *target, const Variant &value) = 0;
	virtual VString doGetAsString(const void *source) const = 0;
	virtual void doSetAsString(void *target, const VString &value) = 0;
};

typedef std::map<VString, VPropertyCommand*> VPropertyCommandMap;


/**
 * @class VPropertyDictionary
 * @brief 属性字典类，
 */
class VPropertyDictionary
{
	friend class VPropertyInterface;

protected:
	VPropertyList		mProperties;
	VPropertyCommandMap	mPropertyCommands;

	VPropertyCommand *getPropertyCommand(const VString &name)
	{
		VPropertyCommandMap::iterator itr = mPropertyCommands.find(name);
		if (itr != mPropertyCommands.end())
		{
			return itr->second;
		}
		return VNULL;
	}

	const VPropertyCommand *getPropertyCommand(const VString &name) const
	{
		VPropertyCommandMap::const_iterator itr = mPropertyCommands.find(name);
		if (itr != mPropertyCommands.end())
		{
			return itr->second;
		}
		return VNULL;
	}

	const VPropertyInfo *getProperty(const VString &name) const
	{
		VPropertyList::const_iterator itr;
		for (itr = mProperties.begin(); itr != mProperties.end(); ++itr)
		{
			const VPropertyInfo &property = *itr;
			if (property.name == name)
			{
				return &property;
			}
		}
		return VNULL;
	}

public:
	void addProperty(const VPropertyInfo &property, VPropertyCommand *command)
	{
		mProperties.push_back(property);
		mPropertyCommands[property.name] = command;
	}

	const VPropertyList &getProperties() const
	{
		return mProperties;
	}
};

typedef std::map<VString, VPropertyDictionary> VPropertyDictionaryMap;

typedef std::map<VString, VString> VPropertyNameStringPairList;
typedef std::map<VString, Variant> VPropertyNameVariantPairList;

class VENGINE_DLL VPropertyInterface
{
protected:
	static VPropertyDictionaryMap msDictionary;

	VString mPropertyDictName;

	VBOOL createPropertyDictionary(const VString &className)
	{
		mPropertyDictName = className;
		if (msDictionary.find(className) == msDictionary.end())
		{
			msDictionary[className] = VPropertyDictionary();
			return VTRUE;
		}

		return VFALSE;
	}

	void defineProperty(const VString &name, const VString &description, const VString &type,
		VPropertyCommand *propertyCmd, VUINT32 flags = 0);

public:
	virtual ~VPropertyInterface() {}

	VPropertyDictionary *getPropertyDictionary()
	{
		VPropertyDictionaryMap::iterator itr = msDictionary.find(mPropertyDictName);
		if (itr != msDictionary.end())
		{
			return &(itr->second);
		}

		return VNULL;
	}

	const VPropertyDictionary *getPropertyDictionary() const
	{
		VPropertyDictionaryMap::const_iterator itr = msDictionary.find(mPropertyDictName);
		if (itr != msDictionary.end())
		{
			return &(itr->second);
		}

		return VNULL;
	}

	const VPropertyList &getProperties() const;

	virtual VBOOL hasProperty(const VString &name) const;

	virtual VBOOL setProperty(const VString &name, const Variant &value);

	virtual VBOOL setPropertyAsString(const VString &name, const VString &value);

	virtual void setPropertyList(const VPropertyNameVariantPairList &propertyList);

	virtual void setPropertyList(const VPropertyNameStringPairList &propertyList);

	virtual Variant getProperty(const VString &name) const;

	virtual VString getPropertyAsString(const VString &name) const;

	virtual VUINT32 getPropertyFlags(const VString &name) const;

	virtual void copyPropertiesTo(VPropertyInterface *dest) const;

	static void cleanupDictionary();
};


#endif	/*__VISION_PROPERTY_H__*/
