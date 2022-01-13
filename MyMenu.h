#pragma once
#include ".\\TinyEngine\\T_Menu.h"
class MyMenu :
    public T_Menu 
{
public:
    bool inrange;
    void DrawMenu(HDC hdc, int bkgX, int bkgY, BYTE btnTrans, bool startState);
    void MenuMouseMove(int x, int y);
};

