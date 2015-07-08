
#include <tchar.h>
#include "VApplication.h"

extern VApplication *g_pApp;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	if (NULL == g_pApp)
		return -1;

	if (!g_pApp->InitInstance(hInstance))
	{
		g_pApp->ExitInstance();
		return -2;
	}

	g_pApp->Run();

	return g_pApp->ExitInstance();
}
