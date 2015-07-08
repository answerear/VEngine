

#ifndef __GAME_APP_H__
#define __GAME_APP_H__


#include <string.h>
#include "VType.h"


class GameApp
{
public:
	GameApp();
	virtual ~GameApp();

	VBOOL init();
	void run();
	void exit();
};


#endif	/*__GAME_APP_H__*/
