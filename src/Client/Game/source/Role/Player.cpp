

#include "Player.h"
#include "VTrace.h"
#include "GameEvent.h"


V_IMPLEMENT_DYNCREATE(Player, VObject)


V_BEGIN_EVENT_MAP(Player, VEventHandler)
	V_ON_EVENT(EV_CREATE, onCreate)
	V_ON_EVENT(EV_DESTROY, onDestroy)
V_END_EVENT_MAP()


Player::Player()
{

}

Player::~Player()
{

}


VBOOL Player::onCreate(VUINT32 param)
{
	V_TRACE("Player::onCreate()\n");
	return VTRUE;
}

VBOOL Player::onDestroy(VUINT32 param)
{
	V_TRACE("Player::onDestroy()\n");
	return VTRUE;
}
