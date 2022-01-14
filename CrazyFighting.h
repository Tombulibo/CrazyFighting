#pragma once
#include ".\\TinyEngine\\T_Engine.h"
#include ".\\TinyEngine\\T_Sprite.h"
#include ".\\TinyEngine\\T_Scene.h"
#include ".\\TinyEngine\\T_AI.h"
#include "MyMenu.h"
#include "CrazyMan.h"

//定义vector容器类型的数据类型vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class CrazyFighting :
    public T_Engine
{
private:
    //重新定义动画帧序列
    static int FRAME_RIGHT[20]; //右方向的帧动画
    static int FRAME_UP[20];
    static int FRAME_DOWN[20];

    //定义要使用的类对象
    T_Scene t_scene;		//游戏场景
    CrazyMan* player;		//游戏玩家


    //定义全局性的变量
    int scn_width, scn_height; //场景宽高
    int wnd_width, wnd_height; //游戏窗口宽高
 //   POINT mouse_pt;				// 当前鼠标位置

 
    //游戏菜单类的对象
    T_Graph back;
    T_Graph menuArea;				//游戏运行菜单区域
    T_Menu gamemenu;				//游戏开始菜单类对象
    T_Menu aboutmenu;				//游戏关于菜单类对象
    T_Menu helpmenu;				//游戏帮助菜单类对象
    MyMenu runmenu;					//游戏运行菜单类对象

    //声音
    AudioDX ds;
    AudioDXBuffer backmusic_buffer;
    AudioDXBuffer robot_buffer;
    AudioDXBuffer mousedown_buffer;
    AudioDXBuffer mousemove_buffer;
    AudioDXBuffer explosionSound;

    //定义功能函数
    void LoadPlayer();								// 加载游戏玩家角色
    void LoadMap();									// 加载游戏地图场景	

    void UpdatePlayerPos();							// 更新玩家角色的位置
    void UpdateAnimation();							// 更新角色动画帧序列号
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

    void StartMenuInit();				//开始菜单初始化
    void AboutMenuInit();				//关于菜单初始化
    void HelpMenuInit();				//帮助菜单初始化
    void RunMenuInit();					//游戏运行菜单初始化

    void PaintStartMenu(HDC hdc);		//绘制开始菜单
    void PaintAboutMenu(HDC hdc);		//绘制关于菜单
    void PaintHelpMenu(HDC hdc);		//绘制帮助菜单

    void MainGameInit();				//方块游戏初始化
    void PaintMainGame(HDC hdc);		//绘制方块游戏

};

