

#include "VPropertyHelper.h"

VBOOL mapEnumLabel(const VConstant *constants, VINT32 value, VString &label)
{
	assert(constants);

	for (const VConstant *i = constants; i->label; ++i)
	{
		if (i->value == value)
		{
			label = i->label;
			return VTRUE;
		}
	}

	return VFALSE;
}

VBOOL mapEnumValue(const VConstant *constants, const VString &label, VINT32 &value, VBOOL caseSensitive)
{
	assert(constants);
	typedef int (*CmpFunc)(const Char*, const Char*);
	CmpFunc cmp = caseSensitive ? strcmp : stricmp;

	for (const VConstant *i = constants; i->label; ++i)
	{
		if (cmp(i->label, label.c_str()) == 0)
		{
			value = i->value;
			return VTRUE;
		}
	}

	return VFALSE;
}

VBOOL populateFlagsLabel(const VConstant *constants, VINT32 value, VString &label)
{
	assert(constants);

	label.clear();
	for (const VConstant *i = constants; i->label; ++i)
	{
		if (i->value & value)
		{
			if (!label.empty())
				label.append(", ");
			label.append(i->label);
		}
	}

	return VTRUE;
}

VBOOL populateFlagsValue(const VConstant *constants, const VString &label, VINT32 &value, VBOOL caseSensitive)
{
	assert(constants);
	Ogre::StringVector items = Ogre::StringUtil::split(label, ",|");

	int flags = 0;
	for (Ogre::StringVector::iterator i = items.begin(); i != items.end(); ++i)
	{
		Ogre::String &item = *i;
		Ogre::StringUtil::trim(item);

		VINT32 flag;
		if (!mapEnumValue(constants, item, flag, caseSensitive))
			return VFALSE;
		flags |= flag;
	}

	value = flags;
	return VTRUE;
}

static VString getLabelList(const VConstant *constants)
{
	VString result;
	for (const VConstant *i = constants; i->label; ++i)
	{
		if (!result.empty())
			result.append(", ");
		result.append(i->label);
	}
	return result;
}

VString getEnumTypeString(const VString &type, const VConstant *constants)
{
	return "Enum " + type + " ( " + getLabelList(constants) + " )";
}

VString getFlagsTypeString(const VString &type, const VConstant *constants)
{
	return "Flags " + type + " ( " + getLabelList(constants) + " )";
}

ColorValue String2ColourRGB(const VString &val)
{
	// 如果是由color控件得到的颜色字符串，它的第一个字符为(
	if (val[0] == '(')
	{

		Ogre::String tempColorStr = val.substr(1,val.length()-2);

		Ogre::StringVector vec = Ogre::StringUtil::split(tempColorStr, ",");

		assert(vec.size() == 3);

		int r = Ogre::StringConverter::parseInt(vec[0]);
		int g = Ogre::StringConverter::parseInt(vec[1]);
		int b = Ogre::StringConverter::parseInt(vec[2]);

		return ColorValue( (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f );
	}
	
	return String2ColourARGB(val);
}

VString ColourRGB2String(const ColorValue &val)
{
	Ogre::StringUtil::StrStreamType stream;
	stream << val.r << " " << val.g << " " << val.b;
	return stream.str();
}

ColorValue String2ColourARGB(const VString &val)
{
	return Ogre::StringConverter::parseColourValue(val);
}

VString ColourARGB2String(const ColorValue &val)
{
	return Ogre::StringConverter::toString(val);
}



