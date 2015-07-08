

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "VObject.h"
#include "VEventHandler.h"


class Player
	: public VObject
	, public VEventHandler
{
	V_DECLARE_DYNCREATE(Player);
	V_DECLARE_EVENT_MAP();

public:
	Player();
	virtual ~Player();

protected:
	VBOOL onCreate(VUINT32 param);
	VBOOL onDestroy(VUINT32 param);
};


#endif	/*__PLAYER_H__*/

