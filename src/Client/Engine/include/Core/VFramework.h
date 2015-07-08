

#ifndef __VISION_FRAMEWORK_H__
#define __VISION_FRAMEWORK_H__


#include "VEngineConfig.h"


class VEventManager;
class VGameStageManager;


class VENGINE_DLL VFramework
{
public:
	VFramework();
	~VFramework();

private:
	VFramework(const VFramework &);
	VFramework &operator =(const VFramework &);

public:
	void startup(VUINT32 initStageID);
	void shutdown();
	VBOOL update(const Real &time, const Real &dt);

protected:
	VEventManager		*mEventMgr;
	VGameStageManager	*mGameStageMgr;
};


#endif	/*__VISION_FRAMEWORK_H__*/
