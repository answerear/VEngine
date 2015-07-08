

#ifndef __VISION_OBJECT_H__
#define __VISION_OBJECT_H__


#include "VEngineConfig.h"
#include "VRuntimeClass.h"


class VDumpContext;



class VENGINE_DLL VObject
{
public:
	virtual ~VObject();
	virtual VRuntimeClass *getRuntimeClass() const;
	VBOOL isKindOf(const VRuntimeClass *pClass) const;
	virtual void assertValid() const;
	virtual void dump(VDumpContext &dc) const;

protected:
	VObject();

private:
	VObject(const VObject &);
	VObject &operator =(const VObject &);
	
public:
	static VRuntimeClass classVObject;
	static VRuntimeClass *STDCALL _getBaseClass();
};


#endif	/*__VISION_OBJECT_H__*/
