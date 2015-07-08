

#ifndef __VISION_SYSTEM_H__
#define __VISION_SYSTEM_H__


#include "VSystemConfig.h"
#include "VSingleton.h"


class ISystemFactory;
class VConsole;
class VLogManager;
class VTextCodec;


class VSYSTEM_DLL VSystem : public VSingleton<VSystem>
{
public:
	VSystem();
	~VSystem();

private:
	VSystem(const VSystem &);
	VSystem &operator =(const VSystem &);

public:
	static VSystem &instance()
	{
		return VSingleton<VSystem>::instance();
	}

	VRESULT startup(VCHAR *filename = VNULL, VBOOL isFile = VFALSE);
	VRESULT shutdown();

	ISystemFactory &getSystemFactory()
	{
		return (*mSystemFactory);
	}

private:
	ISystemFactory	*mSystemFactory;
	VConsole		*mConsole;
	VLogManager		*mLogMgr;
	VTextCodec		*mTextCodec;
};


#define VSYSTEM				(VSystem::instance())
#define VSYSTEM_FACTORY		(VSYSTEM.getSystemFactory())


#endif	/*__VISION_SYSTEM_H__*/
