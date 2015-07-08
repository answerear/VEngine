/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    VEngine.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/7/27 23:52:05
 * @version v1.00
 **************************************************************************************************/


#ifndef __VENGINE_H__
#define __VENGINE_H__


#include "VType.h"


class VENGINE_DLL VEngine
{
private:
	VEngine();

public:
	~VEngine();

private:
	VEngine(const VEngine &other);
	VEngine &operator =(const VEngine &other);

public:
	static VEngine &instance()
	{
		return mInstance;
	}

	VRESULT startup();
	VRESULT shutdown();
	void run();

	void update();
	void render();

protected:
	static VEngine mInstance;
};


#define VENGINE		VEngine::instance()


#endif	/*__VENGINE_H__*/

