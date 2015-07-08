

#include <windows.h>
#include "VCriticalSectionImp.h"


VCriticalSectionImp::VCriticalSectionImp()
{
	::InitializeCriticalSection(&mCS);
}

VCriticalSectionImp::~VCriticalSectionImp()
{
	::DeleteCriticalSection(&mCS);
}

VBOOL VCriticalSectionImp::lock(VDWORD timeout /* = -1 */)
{
	::EnterCriticalSection(&mCS);
	return VTRUE;
}

VBOOL VCriticalSectionImp::unlock()
{
	::LeaveCriticalSection(&mCS);
	return VTRUE;
}

