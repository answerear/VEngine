

#include "VObject.h"
#include "VRuntimeClass.h"
#include "VDumpContext.h"


static VCHAR szCObject[] = "VObject";
VRuntimeClass VObject::classVObject =
{ szCObject, sizeof(VObject), VNULL, VNULL };
static V_CLASSINIT _init_VObject(&VObject::classVObject);

VRuntimeClass *VObject::_getBaseClass()
{
	return VNULL;
}


VObject::VObject()
{

}

VObject::~VObject()
{

}

VRuntimeClass* VObject::getRuntimeClass() const
{
	return &VObject::classVObject;
}

VBOOL VObject::isKindOf(const VRuntimeClass* pClass) const
{
	VRuntimeClass* pClassThis = getRuntimeClass();
	while (pClassThis != VNULL)
	{
		if (pClassThis == pClass)
			return VTRUE;
		pClassThis = pClassThis->m_pfnGetBaseClass();
	}
	return VFALSE;       // walked to the top, no match
}

void VObject::assertValid() const
{

}

void VObject::dump(VDumpContext &dc) const
{
	
}
