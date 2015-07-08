

#ifndef __VISION_XML_ATTRIBUTES_H__
#define __VISION_XML_ATTRIBUTES_H__


#include <boost/lexical_cast.hpp>
#include <boost/call_traits.hpp>
#include <map>

#include "VEngineConfig.h"

template <typename T>
struct helper
{
	typedef typename boost::call_traits<T>::value_type result_type;

	static result_type convert(const VString &val)
	{
		return boost::lexical_cast<result_type>(val);
	}
};

template <>
struct helper<VString>
{
	typedef const VString& result_type;

	static result_type convert(const VString &val)
	{
		return val;
	}
};

template <>
struct helper<VBOOL>
{
	typedef VBOOL result_type;

	static result_type convert(const VString &val)
	{
		if ("false" == val || "no" == val || "0" == val)
		{
			return VFALSE;
		}
		else if ("true" == val || "yes" == val || "1" == val)
		{
			return VTRUE;
		}
		else
		{
			return VTRUE;
		}
	}
};

class XMLAttributes
{
public:
	XMLAttributes();
	virtual ~XMLAttributes();

public:
	void setValue(const VString &name, const VString &value);
	void remove(const VString &name);
	void removeAll();
	VBOOL exists(const VString &name) const;
	size_t count() const;
	const VString &getName(size_t index) const;
	const VString &getValue(size_t index) const;
	const VString &getValue(const VString &name) const;



	template <typename T>
	typename helper<T>::result_type getValueAs(const VString &name) const
	{
		return helper<T>::convert(getValue(name));
	}

	template <typename T>
	typename helper<T>::result_type getValueAs(const VString &name, typename boost::call_traits<T>::param_type def) const
	{
		if (!exists(name))
		{
			return def;
		}

		return getValueAs<T>(name);
	}

	template <typename T>
	void setValueAs(const VString &name, T val)
	{
		setValue(name, boost::lexical_cast<VString>(val));
	}

	template <typename T>
	void setValueAs(const VString &name, T val, typename boost::call_traits<T>::param_type def)
	{
		if (val != def)
		{
			setValueAs(name, val);
		}
	}

	void setValueAs(const VString &name, VBOOL val)
	{
		static const VString true_value = "true";
		static const VString false_value = "false";
		setValue(name, val ? true_value : false_value);
	}

	void setValueAs(const VString &name, const VString &val)
	{
		setValue(name, val);
	}

	void setValueAs(const VString &name, const VString &val, const VString def)
	{
		if (val != def)
		{
			setValueAs(name, val);
		}
	}

protected:
	typedef std::map<VString, VString> AttributeMap;
	AttributeMap mAttributes;
};


#endif	/*__VISION_XML_ATTRIBUTES_H__*/

