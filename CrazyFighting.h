#pragma once
#include ".\\TinyEngine\\T_Engine.h"
#include ".\\TinyEngine\\T_Sprite.h"
#include ".\\TinyEngine\\T_Scene.h"
#include ".\\TinyEngine\\T_AI.h"
#include "MyMenu.h"
#include "CrazyMan.h"
#include "Horizontal_Scene.h"

//����vector�������͵���������vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;
//����vector�������͵���������vCrazyManSet
typedef vector<CrazyMan*> vCrazyManSet;

class CrazyFighting :
    public T_Engine
{
private:
    static const int NPC_NUM = 10; //NPC����
    static const int SUPER_TIME = 3000;

    //����Ҫʹ�õ������
    Horizontal_Scene t_scene;		//��Ϸ����
    CrazyMan* player;		//��Ϸ���
    vCrazyManSet npc_set;   //NPC��ɫ����
    vCrazyManSet trap_set;        //���弯

    int playerType;         //������ͣ�0ΪŮ���֣�1Ϊ��սʿ��2ΪԪ��ʹ
    int mapType;            //��ͼ���ͣ�0Ϊ��԰��1Ϊ�󶴣�2Ϊ����


    //����ȫ���Եı���
    int scn_width, scn_height; //�������
    int wnd_width, wnd_height; //��Ϸ���ڿ��
    POINT mouse_pt;				// ��ǰ���λ��

 
    //��Ϸ�˵���Ķ���
    T_Graph back;
    T_Graph menuArea;				//��Ϸ���в˵�����
    T_Menu startmenu;				//��Ϸ��ʼ�˵������
    T_Menu aboutmenu;				//��Ϸ���ڲ˵������
    T_Menu helpmenu;				//��Ϸ�����˵������
    MyMenu runmenu;					//��Ϸ���в˵������
    T_Menu playermenu;              //��Ϸѡ�˲˵�
    T_Menu mapmenu;                 //��Ϸѡ��ͼ�˵�
    int chooseState;                //��Ϸѡ��״̬

    //������������������������������������������������������������������������������
    // ����Ϸ�߼�������صı���
    //������������������������������������������������������������������������������
    long BonusLoadStartTime;						// �������ؼ�ʱ
    long BubbleLoadStartTime;						// Bubble���ؼ�ʱ

    //int GameLevel;									// ��Ϸ����������
    int LifeCount = 3;									// �������������
    int ScoreCount = 0;									// ��ҵ÷��ۼ�
    //int LevelScore;									// ÿ�ص÷�
    //int growScore;									// ��ҳ���÷�

    bool isDelayCollision;							// �Ƿ���ʱ��ײ
    bool looseBlood;								// �Ƿ�ʧѪ״̬
    //bool upgradeGameLevel;							// �Ƿ��Ѿ�����
    bool updateLifeCount;							// �Ƿ�������������

    //bool growUp;									// �Ƿ�ɳ�
    //bool addLifeCount;								// �Ƿ������������ֵ
    //������������������������������������������������������������������������������
    // ��ͼƬ��Դ��صı���
    //������������������������������������������������������������������������������
    T_Graph* lifeImg;								// ״̬�����������СͼƬ
    T_Graph* levelImg;								// ��Ϸ�ؼ���СͼƬ
    T_Graph* totalscoreBmp;							// ״̬������ܷ���СͼƬ
    //T_Graph* scoreImg;								// ״̬����ҵ÷�СͼƬ

    //����
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

    //���幦�ܺ���
    void LoadPlayer();								// ������Ϸ��ҽ�ɫ
    void LoadMap();									// ������Ϸ��ͼ����	
    void LoadNpc(int total);                        // ������ϷNPC
    void LoadTrap(int num, int level);               // ���ص�ͼ����

    void UpdatePlayerPos();							// ������ҽ�ɫ��λ��
    void updatePlayerlife();
    void UpdateNpcPos();                            // ����NPCλ��
    void UpdateAnimation();							// ���½�ɫ����֡���к�

    vSpriteSet arrow_set;	                // ������
    void LoadArrow(int iTime);				// �����ڵ�
    void UpdateArrowPos();					// �����ڵ�λ��
    void ArrowCollide(T_Sprite* arrow);		// ����ڵ��Ƿ����Ŀ��
    void AttackCollide();                   // ��⹥��

    void LoadImageRes();
    void DisplayInfo(HDC hdc, int game_state);
    void ClearGameLevel();

public:
    virtual ~CrazyFighting(void);
    CrazyFighting(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
        WORD icon = NULL, WORD sm_icon = NULL,
        int winwidth = 800, int winheight = 600, COLORREF bkColor = WIN_BKCLR);

    // ����T_Engine���е��麯��
    void GameInit();                                    // ��Ϸ��ʼ��
    void GameLogic();                                   // ��Ϸ�߼�����
    void GameEnd();                                     // ��Ϸ��������
    void GamePaint(HDC hdc);                            // ��Ϸ��ʾ
    void GameKeyAction(int Action = KEY_SYS_NONE);      // ������Ϊ����
    void GameMouseAction(int x, int y, int Action);     // �����Ϊ����

    void StartMenuInit();				//��ʼ�˵���ʼ��
    void AboutMenuInit();				//���ڲ˵���ʼ��
    void HelpMenuInit();				//�����˵���ʼ��
    void RunMenuInit();					//��Ϸ���в˵���ʼ��
    void PlayerMenuInit();              //��Ϸѡ�˲˵���ʼ��
    void MapMenuInit();                 //��Ϸѡ��ͼ�˵���ʼ��

    void PaintStartMenu(HDC hdc);		//���ƿ�ʼ�˵�
    void PaintAboutMenu(HDC hdc);		//���ƹ��ڲ˵�
    void PaintHelpMenu(HDC hdc);		//���ư����˵�
    void PaintPlayerMenu(HDC hdc);      //������Ϸѡ�˲˵�
    void PaintMapMenu(HDC hdc);         //������Ϸѡ��ͼ�˵�

    void MainGameInit();				//������Ϸ��ʼ��
    void PaintMainGame(HDC hdc);		//���Ʒ�����Ϸ
};

