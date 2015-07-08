

#ifndef __VISION_APPLICATION_H__
#define __VISION_APPLICATION_H__


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
	

protected:
	VCHAR		*m_lpszClassName;
	VCHAR		*m_lpszWindowName;

	int			m_nWndXPos;
	int			m_nWndYPos;
	int			m_nWndWidth;
	int			m_nWndHeight;

	VDWORD		m_dwStyle;
	VDWORD		m_dwExStyle;

	VUINT32		m_uIconID;
	VUINT32		m_uMenuID;

};


#endif	/*__VISION_APPLICATION_H__*/
