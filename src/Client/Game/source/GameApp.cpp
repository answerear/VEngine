

#include "GameApp.h"
#include "VUtility.h"
#include "VEngine.h"
#include "VGameStageManager.h"
#include "LoginGameStage.h"
#include "SceneGameStage.h"
#include "VException.h"


#define GAME_TITLE			"Demo"
#define GAME_VERSION		"1.00"


GameApp theApp;


GameApp::GameApp()
{

}

GameApp::~GameApp()
{

}

VBOOL GameApp::init()
{	
 	VGAME_STAGE_MANAGER.registerStage(1, LoginGameStage::createStage);
 	VGAME_STAGE_MANAGER.registerStage(2, SceneGameStage::createStage);

	VCHAR title[256];
	snprintf(title, 256, "%s %s (%s %s)", GAME_TITLE, GAME_VERSION, __DATE__, __TIME__);
	
#if _DEBUG
 	VENGINE.startup(1, title, 800, 600, "config_d.ini");
#else
	VENGINE.startup(1, title, 800, 600, "config.ini");
#endif

	return VTRUE;
}

void GameApp::run()
{
	VENGINE.run();
}

void GameApp::exit()
{
 	VENGINE.shutdown();
}


#if defined PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
#elif defined PLATFORM_LINUX || PLATFORM_MAC
int main(int argc, char *argv[])
#endif
{
	try
	{
		if (!theApp.init())
		{
			theApp.exit();
			return -1;
		}

		theApp.run();
	}
	catch (Ogre::Exception &e)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " <<
			e.getFullDescription().c_str() << std::endl;
#endif
	}
	catch (VException &e)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " <<
			e.getFullDescription().c_str() << std::endl;
#endif
	}

	theApp.exit();
	
	return 0;
}
