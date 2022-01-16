#pragma once
#include ".\\TinyEngine\\T_Engine.h"
#include ".\\TinyEngine\\T_Sprite.h"
#include ".\\TinyEngine\\T_Scene.h"
#include ".\\TinyEngine\\T_AI.h"
#include "MyMenu.h"
#include "CrazyMan.h"
#include "Horizontal_Scene.h"

//定义vector容器类型的数据类型vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;
//定义vector容器类型的数据类型vCrazyManSet
typedef vector<CrazyMan*> vCrazyManSet;

class CrazyFighting :
    public T_Engine
{
private:
    static const int NPC_NUM = 10; //NPC总数
    static const int SUPER_TIME = 3000;

    //定义要使用的类对象
    Horizontal_Scene t_scene;		//游戏场景
    CrazyMan* player;		//游戏玩家
    vCrazyManSet npc_set;   //NPC角色集合
    vCrazyManSet trap_set;        //陷阱集

    int playerType;         //玩家类型，0为女射手，1为狂战士，2为元素使
    int mapType;            //地图类型，0为田园，1为矿洞，2为地狱


    //定义全局性的变量
    int scn_width, scn_height; //场景宽高
    int wnd_width, wnd_height; //游戏窗口宽高
    POINT mouse_pt;				// 当前鼠标位置

 
    //游戏菜单类的对象
    T_Graph back;
    T_Graph menuArea;				//游戏运行菜单区域
    T_Menu startmenu;				//游戏开始菜单类对象
    T_Menu aboutmenu;				//游戏关于菜单类对象
    T_Menu helpmenu;				//游戏帮助菜单类对象
    MyMenu runmenu;					//游戏运行菜单类对象
    T_Menu playermenu;              //游戏选人菜单
    T_Menu mapmenu;                 //游戏选地图菜单
    int chooseState;                //游戏选择状态

    //………………………………………………………………………………………………………
    // 与游戏逻辑控制相关的变量
    //………………………………………………………………………………………………………
    long BonusLoadStartTime;						// 奖励加载计时
    long BubbleLoadStartTime;						// Bubble加载计时

    //int GameLevel;									// 游戏关数计数器
    int LifeCount = 3;									// 玩家生命计数器
    int ScoreCount = 0;									// 玩家得分累计
    //int LevelScore;									// 每关得分
    //int growScore;									// 玩家长大得分

    bool isDelayCollision;							// 是否延时碰撞
    bool looseBlood;								// 是否失血状态
    //bool upgradeGameLevel;							// 是否已经升级
    bool updateLifeCount;							// 是否更新玩家生命数

    //bool growUp;									// 是否成长
    //bool addLifeCount;								// 是否可以增加生命值
    //………………………………………………………………………………………………………
    // 与图片资源相关的变量
    //………………………………………………………………………………………………………
    T_Graph* lifeImg;								// 状态栏玩家生命数小图片
    T_Graph* levelImg;								// 游戏关计数小图片
    T_Graph* totalscoreBmp;							// 状态栏玩家总分数小图片
    //T_Graph* scoreImg;								// 状态栏玩家得分小图片

    //声音
    AudioDX ds;
    AudioDXBuffer openmusic_buffer;
    AudioDXBuffer startmusic_buffer;
    AudioDXBuffer mapmusic01_buffer;
    AudioDXBuffer mapmusic02_buffer;
    AudioDXBuffer mapmusic03_buffer;
    //AudioDXBuffer robot_buffer;
    AudioDXBuffer mousedown_buffer;
    AudioDXBuffer mousemove_buffer;
    //AudioDXBuffer explosionSound;
    AudioDXBuffer gameovermusic_buffer;
    AudioDXBuffer victorymusic_buffer;

    //定义功能函数
    void LoadPlayer();								// 加载游戏玩家角色
    void LoadMap();									// 加载游戏地图场景	
    void LoadNpc(int total);                        // 加载游戏NPC
    void LoadTrap(int num, int level);               // 加载地图陷阱

    void UpdatePlayerPos();							// 更新玩家角色的位置
    void updatePlayerlife();
    void UpdateNpcPos();                            // 更新NPC位置
    void UpdateAnimation();							// 更新角色动画帧序列号

    vSpriteSet arrow_set;	                // 箭容器
    void LoadArrow(int iTime);				// 加载炮弹
    void UpdateArrowPos();					// 更新炮弹位置
    void ArrowCollide(T_Sprite* arrow);		// 检测炮弹是否击中目标
    void AttackCollide();                   // 检测攻击

    void LoadImageRes();
    void DisplayInfo(HDC hdc, int game_state);
    void ClearGameLevel();

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
    void PlayerMenuInit();              //游戏选人菜单初始化
    void MapMenuInit();                 //游戏选地图菜单初始化

    void PaintStartMenu(HDC hdc);		//绘制开始菜单
    void PaintAboutMenu(HDC hdc);		//绘制关于菜单
    void PaintHelpMenu(HDC hdc);		//绘制帮助菜单
    void PaintPlayerMenu(HDC hdc);      //绘制游戏选人菜单
    void PaintMapMenu(HDC hdc);         //绘制游戏选地图菜单

    void MainGameInit();				//方块游戏初始化
    void PaintMainGame(HDC hdc);		//绘制方块游戏
};

