

#include "VProperty.h"


VPropertyDictionaryMap VPropertyInterface::msDictionary;


void VPropertyInterface::defineProperty(const VString &name, const VString &description,
	const VString &type, VPropertyCommand *propertyCmd, VUINT32 flags)
{
	getPropertyDictionary()->addProperty(
		VPropertyInfo(name, 
					  description, 
					  type, 
					  propertyCmd ? propertyCmd->doGetAsString(this) : VString(),
					  flags), 
		propertyCmd);
}

const VPropertyList &VPropertyInterface::getProperties() const
{
	const VPropertyDictionary *dict = getPropertyDictionary();
	if (VNULL != dict)
	{
		return dict->getProperties();
	}

	static const VPropertyList emptyList;
	return emptyList;
}

VBOOL VPropertyInterface::hasProperty(const VString &name) const
{
	const VPropertyDictionary *dict = getPropertyDictionary();
	if (VNULL != dict)
	{
		const VPropertyCommand *cmd = dict->getPropertyCommand(name);
		if (VNULL != cmd)
		{
			return VTRUE;
		}
	}
	return !getProperty(name).empty();
}

VBOOL VPropertyInterface::setProperty(const VString &name, const Variant &value)
{
	VPropertyDictionary *dict = getPropertyDictionary();

	if (VNULL != dict)
	{
		VPropertyCommand *cmd = dict->getPropertyCommand(name);
		if (VNULL != cmd)
		{
			cmd->doSet(this, value);
			return VTRUE;
		}
	}
	return VFALSE;
}

VBOOL VPropertyInterface::setPropertyAsString(const VString &name, const VString &value)
{
	VPropertyDictionary *dict = getPropertyDictionary();

	if (VNULL != dict)
	{
		VPropertyCommand *cmd = dict->getPropertyCommand(name);
		if (VNULL != cmd)
		{
			cmd->doSetAsString(this, value);
			return VTRUE;
		}
	}
	return VFALSE;
}

void VPropertyInterface::setPropertyList(const VPropertyNameVariantPairList &propertyList)
{
	VPropertyNameVariantPairList::const_iterator itr;
	for (itr = propertyList.begin(); itr != propertyList.end(); ++itr)
	{
		setProperty(itr->first, itr->second);
	}
}

void VPropertyInterface::setPropertyList(const VPropertyNameStringPairList &propertyList)
{
	VPropertyNameStringPairList::const_iterator itr;
	for (itr = propertyList.begin(); itr != propertyList.end(); ++itr)
	{
		setPropertyAsString(itr->first, itr->second);
	}
}

Variant VPropertyInterface::getProperty(const VString &name) const
{
	const VPropertyDictionary *dict = getPropertyDictionary();
	if (VNULL != dict)
	{
		const VPropertyCommand *cmd = dict->getPropertyCommand(name);
		if (VNULL != cmd)
		{
			return cmd->doGet(this);
		}
	}
	return Variant();
}

VString VPropertyInterface::getPropertyAsString(const VString &name) const
{
	const VPropertyDictionary *dict = getPropertyDictionary();
	if (VNULL != dict)
	{
		const VPropertyCommand *cmd = dict->getPropertyCommand(name);
		if (VNULL != cmd)
		{
			return cmd->doGetAsString(this);
		}
	}
	return VString("");
}

VUINT32 VPropertyInterface::getPropertyFlags(const VString &name) const
{
	const VPropertyDictionary *dict = getPropertyDictionary();
	if (VNULL != dict)
	{
		const VPropertyInfo *info = dict->getProperty(name);
		if (VNULL != info)
		{
			return info->flags;
		}
	}
	return 0;
}

void VPropertyInterface::copyPropertiesTo(VPropertyInterface *dest) const
{
	const VPropertyDictionary *dict = getPropertyDictionary();
	if (VNULL != dict)
	{
		VPropertyList::const_iterator itr;
		for (itr = dict->mProperties.begin(); itr != dict->mProperties.end(); ++itr)
		{
			dest->setProperty(itr->name, getProperty(itr->name));
		}
	}
}

void VPropertyInterface::cleanupDictionary()
{
	msDictionary.clear();
}

