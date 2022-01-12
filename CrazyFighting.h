#pragma once
#include "TinyEngine/T_Engine.h"
#include "TinyEngine/T_AI.h"
#include "TinyEngine/T_Audio.h"
#include "TinyEngine/T_Config.h"
#include "TinyEngine/T_Display.h"
#include "TinyEngine/T_Graph.h"
#include "TinyEngine/T_Layer.h"
#include "TinyEngine/T_Map.h"
#include "TinyEngine/T_Menu.h"
#include "TinyEngine/T_Music.h"
#include "TinyEngine/T_Scene.h"
#include "TinyEngine/T_Sprite.h"
#include "TinyEngine/T_Util.h"

//定义vector容器类型的数据类型vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class CrazyFighting :
    public T_Engine
{
private:
    //定义全局性的变量
    int scn_width, scn_height; //场景宽高
    int wnd_width, wnd_height; //游戏窗口宽高
    POINT mouse_pt; //当前鼠标位置
    int op; //操作类型(op=0键盘; op=1鼠标)

    //定义要使用的类对象
    T_Sprite* player; //游戏玩家
    vSpriteSet npc_set; //NPC角色集合
    T_Scene t_scene;

    //定义功能函数
    void LoadMap(); //加载游戏地图场景
    void LoadPlayer(); //加载游戏玩家
    void LoadNpc(int total); //加载游戏NPC
    void UpdatePlayerPos(int dir); //更新玩家位置
    void UpdateNpcPos(); //更新NPC位置
    void UpdateAnimation(); //更新游戏动画
    
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

