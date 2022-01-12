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

void CrazyFighting::LoadMap()
{
}

void CrazyFighting::LoadPlayer()
{
}

void CrazyFighting::LoadNpc(int total)
{
}

void CrazyFighting::UpdatePlayerPos(int dir)
{
}

void CrazyFighting::UpdateNpcPos()
{
}

void CrazyFighting::UpdateAnimation()
{
}

void CrazyFighting::LoadMyMenu()
{
}

void CrazyFighting::GameInit()
{
	GameState = GAME_START;
}

void CrazyFighting::GameLogic()
{
	if (GameState == GAME_RUN)
	{
	}
}

void CrazyFighting::GameEnd()
{
}

void CrazyFighting::GamePaint(HDC hdc)
{
	if (GameState == GAME_START)
	{
	}
	else if (GameState == GAME_ABOUT)
	{
	}
	else if (GameState == GAME_HELP)
	{
	}
	else if (GameState == GAME_RUN)
	{
	}
}

void CrazyFighting::GameKeyAction(int Action)
{
	if (GameState == GAME_START)
	{
	}
	else if (GameState == GAME_ABOUT)
	{
	}
	else if (GameState == GAME_HELP)
	{
	}
	else if (GameState == GAME_RUN)
	{
	}
}

void CrazyFighting::GameMouseAction(int x, int y, int Action)
{
	if (GameState == GAME_START)
	{
	}
	else if (GameState == GAME_ABOUT)
	{
	}
	else if (GameState == GAME_HELP)
	{
	}
	else if (GameState == GAME_RUN)
	{
	}
}
