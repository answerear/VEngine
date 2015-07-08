

#ifndef __VISION_PROPERTY_HELPER_H__
#define __VISION_PROPERTY_HELPER_H__


#include "VEngineConfig.h"
#include "VProperty.h"

#include <OgreStringConverter.h>

#include <boost/lexical_cast.hpp>


typedef VString::value_type Char;

struct VConstant
{
	const Char* label;
	VINT32 value;
};

VBOOL mapEnumLabel(const VConstant *constants, VINT32 value, VString &label);
VBOOL mapEnumValue(const VConstant *constants, const VString &label, VINT32 &value, VBOOL caseSensitive = VTRUE);

VBOOL populateFlagsLabel(const VConstant *constants, VINT32 value, VString &label);
VBOOL populateFlagsValue(const VConstant *constants, const VString &label, VINT32 &value, VBOOL caseSensitive = VTRUE);

VString getEnumTypeString(const VString &type, const VConstant *constants);
VString getFlagsTypeString(const VString &type, const VConstant *constants);

//////////////////////////////////////////////////////////////////////////

ColorValue String2ColourRGB(const VString& val);
VString ColourRGB2String(const ColorValue& val);

ColorValue String2ColourARGB(const VString& val);
VString ColourARGB2String(const ColorValue& val);

//////////////////////////////////////////////////////////////////////////

template <class T>
inline VString	__toString(const T &v)
{
	return Ogre::StringConverter::toString(v);
}

template <>
inline VString	__toString<VString>(const VString &v)
{
	return v;
}

//////////////////////////////////////////////////////////////////////////

template <class T>
inline T __parseValue(const VString &s)
{
	return boost::lexical_cast<T>(s);
}

template <>
inline VString __parseValue<VString>(const VString &s)
{
	return s;
}

template <>
inline VBOOL __parseValue<VBOOL>(const VString &s)
{
	return Ogre::StringConverter::parseBool(s);
}

template <>
inline Vector3 __parseValue<Vector3>(const VString &s)
{
	return Ogre::StringConverter::parseVector3(s);
}

template <>
inline Quaternion __parseValue<Quaternion>(const VString &s)
{
	return Ogre::StringConverter::parseQuaternion(s);
}

//////////////////////////////////////////////////////////////////////////

template <class Class, typename Type, Type Class::*PtrToMember>
class VObjectMemberPropertyCommand : public VPropertyCommand
{
	Variant doGet(const void* source) const
	{
		const Class* object = static_cast<const Class*>(source);
		return object->*PtrToMember;
	}

	VString doGetAsString(const void* source) const
	{
		const Class* object = static_cast<const Class*>(source);
		return __toString(object->*PtrToMember);
	}

	void doSet(void* target, const Variant& value)
	{
		Class* object = static_cast<Class*>(target);
		object->*PtrToMember = VariantCast<Type>(value);
		object->updateRenderInstance();
	}

	void doSetAsString(void* target, const VString& value)
	{
		Class* object = static_cast<Class*>(target);
		object->*PtrToMember = __parseValue<Type>(value);
		object->updateRenderInstance();
	}
};


#endif	/*__VISION_PROPERTY_HELPER_H__*/