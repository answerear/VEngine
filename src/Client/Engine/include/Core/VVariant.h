

#ifndef __VISION_VARIANT_H__
#define __VISION_VARIANT_H__


#include "VEngineConfig.h"
#include <boost/any.hpp>


typedef boost::any Variant;

template<typename ValueType>
ValueType * VariantCast(Variant * operand)
{
	return boost::any_cast<ValueType>(operand);
}

template<typename ValueType>
const ValueType * VariantCast(const Variant * operand)
{
	return boost::any_cast<ValueType>(operand);
}

template<typename ValueType>
ValueType VariantCast(const Variant & operand)
{
	return boost::any_cast<ValueType>(operand);
}


#endif
