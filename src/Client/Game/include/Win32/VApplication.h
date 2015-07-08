

#ifndef __VISION_APPLICATION_H__
#define __VISION_APPLICATION_H__


#include <windows.h>
#include "VType.h"


class VApplication
{
public:
	VApplication();
	virtual ~VApplication();

public:
	BOOL InitInstance(HINSTANCE hInstance);
	int Run();
	int ExitInstance();
	void setTitle(VCHAR *title);

protected:
	virtual void preInitInstance();

	virtual VBOOL init()	{ return VTRUE; }
	virtual VBOOL update()	{ return VTRUE; }
	virtual VBOOL release()	{ return VTRUE; }

	virtual LRESULT MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	HWND GetSafeHwnd()	
	{ 
		if (::IsWindow(m_hWnd)) 
			return m_hWnd; 
		return NULL; 
	}

	HINSTANCE GetInstance()	
	{ 
		return m_hInstance; 
	}

private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

protected:
	TCHAR		*m_lpszClassName;
	TCHAR		*m_lpszWindowName;

	int			m_nWndXPos;
	int			m_nWndYPos;
	int			m_nWndWidth;
	int			m_nWndHeight;

	DWORD		m_dwStyle;
	DWORD		m_dwExStyle;

	UINT		m_uIconID;
	UINT		m_uMenuID;

	HMENU		m_hMenu;
};


#endif	/*__VISION_APPLICATION_H__*/
