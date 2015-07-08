

#include "VisionXMLAttributes.h"
#include "VException.h"


XMLAttributes::XMLAttributes()
{

}

XMLAttributes::~XMLAttributes()
{

}

void XMLAttributes::setValue(const VString &name, const VString &value)
{
	std::pair<AttributeMap::iterator, VBOOL> inserted = 
		mAttributes.insert(AttributeMap::value_type(name, value));

	if (!inserted.second)
	{
		inserted.first->second = value;
	}
}

void XMLAttributes::remove(const VString &name)
{
	AttributeMap::iterator itr = mAttributes.find(name);

	if (itr == mAttributes.end())
	{
		VISION_EXCEPT(VException::VERR_ITEM_NOT_FOUND,
			"The specified attribute named '" + name + "' doesn't exists for this XMLAttributes block",
			"XMLAttributes::remove");
	}

	mAttributes.erase(itr);
}

void XMLAttributes::removeAll()
{
	mAttributes.clear();
}

VBOOL XMLAttributes::exists(const VString &name) const
{
	return (mAttributes.find(name) != mAttributes.end());
}

size_t XMLAttributes::count() const
{
	return mAttributes.size();
}

const VString &XMLAttributes::getName(size_t index) const
{
	if (index >= mAttributes.size())
	{
		VISION_EXCEPT(VException::VERR_ITEM_NOT_FOUND,
			"The specified index is out of range for this XMLAttributes block",
			"XMLAttributes::getName");
	}

	AttributeMap::const_iterator itr = mAttributes.begin();
	std::advance(itr, index);
	return itr->first;
}

const VString &XMLAttributes::getValue(size_t index) const
{
	if (index >= mAttributes.size())
	{
		VISION_EXCEPT(VException::VERR_ITEM_NOT_FOUND,
			"The specified index is out of range for this XMLAttributes block",
			"XMLAttributes::getValue");
	}

	AttributeMap::const_iterator itr = mAttributes.begin();
	std::advance(itr, index);
	return itr->second;
}

const VString &XMLAttributes::getValue(const VString &name) const
{
	AttributeMap::const_iterator itr = mAttributes.find(name);

	if (itr == mAttributes.end())
	{
		VISION_EXCEPT(VException::VERR_ITEM_NOT_FOUND,
			"No value exists for an attribute named '" + name + "'",
			"XMLAttributes::getValue");
	}

	return itr->second;
}


