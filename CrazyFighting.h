#pragma once
#include ".//TinyEngine/T_Engine.h"
class CrazyFighting :
    public T_Engine
{
private:
    //定义全局性的变量
    int scn_width, scn_height; //场景宽高
    int wnd_width, wnd_height; //游戏窗口宽高
    
public:
    virtual ~CrazyFighting(void);
    CrazyFighting(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
        WORD icon = NULL, WORD sm_icon = NULL,
        int winwidth = 800, int winheight = 600, COLORREF bkColor = WIN_BKCLR);

    // 重载T_Engine类中的虚函数
    void GameInit();                                    // 游戏初始化
    void GameLogic();                                   // 游戏逻辑处理
    void GameEnd();                                     // 游戏结束处理
    void GamePaint(HDC hdc);                            // 游戏显示
    void GameKeyAction(int Action = KEY_SYS_NONE);      // 按键行为处理
    void GameMouseAction(int x, int y, int Action);     // 鼠标行为处理
};

