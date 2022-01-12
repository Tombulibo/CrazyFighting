#include "CrazyFighting.h"

CrazyFighting::~CrazyFighting(void)
{
}

CrazyFighting::CrazyFighting(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight, COLORREF bkColor)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	wnd_width = winwidth;
	wnd_height = winheight;
}

void CrazyFighting::GameInit()
{
}

void CrazyFighting::GameLogic()
{
}

void CrazyFighting::GameEnd()
{
}

void CrazyFighting::GamePaint(HDC hdc)
{
}

void CrazyFighting::GameKeyAction(int Action)
{
}

void CrazyFighting::GameMouseAction(int x, int y, int Action)
{
}
