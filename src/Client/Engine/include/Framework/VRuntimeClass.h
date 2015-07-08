/**************************************************************************************************
 * Copyright (C) 2011. Vision Studio.
 * All rights reserved.
 *
 * @file    VRuntimeClass.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/8/28 23:16:53
 * @version v1.00
 **************************************************************************************************/


#ifndef __VISION_RUNTIME_CLASS_H__
#define __VISION_RUNTIME_CLASS_H__


#include "VEngineConfig.h"


class VObject;


struct VENGINE_DLL VRuntimeClass
{
	VCHAR			*m_lpszClassName;
	VINT32			m_nObjectSize;
	VObject			*(STDCALL* m_pfnCreateObject)();
	VRuntimeClass	*(STDCALL* m_pfnGetBaseClass)();
	VRuntimeClass	*m_pNextClass;

	VObject			*createObject();

	static VRuntimeClass *pFirstClass;
};


struct VENGINE_DLL V_CLASSINIT
{
	V_CLASSINIT(VRuntimeClass *pNewClass);
};


/**< 根据类名获取定义类 */
#define VRUNTIME_CLASS(class_name)	((VRuntimeClass*)(&class_name::class##class_name))


/**< 类定义声明宏 */
#define V_DECLARE_DYNAMIC(class_name) \
protected: \
	static VRuntimeClass* STDCALL _getBaseClass(); \
public:	\
	static VRuntimeClass class##class_name; \
	virtual VRuntimeClass* getRuntimeClass() const;


/**< 类定义实现宏 */
#define V_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, pfnNew) \
	VRuntimeClass* STDCALL class_name::_getBaseClass() \
	{ return VRUNTIME_CLASS(base_class_name); } \
	VRuntimeClass class_name::class##class_name = \
	{ #class_name, sizeof(class_name), pfnNew, &class_name::_getBaseClass, NULL}; \
	static V_CLASSINIT _init##class_name(&class_name::class##class_name); \
	VRuntimeClass* class_name::getRuntimeClass() const \
	{ return VRUNTIME_CLASS(class_name); }

#define V_IMPLEMENT_DYNAMIC(class_name, base_class_name) \
	V_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, NULL)


#define V_DECLARE_DYNCREATE(class_name) \
	V_DECLARE_DYNAMIC(class_name) \
	static VObject* STDCALL createObject();


#define V_IMPLEMENT_DYNCREATE(class_name, base_class_name) \
	VObject* STDCALL class_name::createObject() \
	{ return new class_name(); } \
	V_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, class_name::createObject)


#endif	/*__VISION_RUNTIME_CLASS_H__*/

