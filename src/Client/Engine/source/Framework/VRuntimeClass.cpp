

#include "VRuntimeClass.h"
#include "VObject.h"


VRuntimeClass* VRuntimeClass::pFirstClass = VNULL;


V_CLASSINIT::V_CLASSINIT(VRuntimeClass* pNewClass)
{
 	pNewClass->m_pNextClass = VRuntimeClass::pFirstClass;
 	VRuntimeClass::pFirstClass = pNewClass;
}

VObject* VRuntimeClass::createObject()
{
	if (m_pfnCreateObject == VNULL)
	{
		return NULL;
	}

	VObject* pObject = VNULL;
	pObject = (*m_pfnCreateObject)();

	return pObject;
}

