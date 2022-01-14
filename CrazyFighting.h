#pragma once
#include ".\\TinyEngine\\T_Engine.h"
#include ".\\TinyEngine\\T_Sprite.h"
#include ".\\TinyEngine\\T_Scene.h"
#include ".\\TinyEngine\\T_AI.h"
#include "MyMenu.h"
#include "CrazyMan.h"

//����vector�������͵���������vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class CrazyFighting :
    public T_Engine
{
private:
    //���¶��嶯��֡����
    static int FRAME_RIGHT[20]; //�ҷ����֡����
    static int FRAME_UP[20];
    static int FRAME_DOWN[20];

    //����Ҫʹ�õ������
    T_Scene t_scene;		//��Ϸ����
    CrazyMan* player;		//��Ϸ���


    //����ȫ���Եı���
    int scn_width, scn_height; //�������
    int wnd_width, wnd_height; //��Ϸ���ڿ��
 //   POINT mouse_pt;				// ��ǰ���λ��

 
    //��Ϸ�˵���Ķ���
    T_Graph back;
    T_Graph menuArea;				//��Ϸ���в˵�����
    T_Menu gamemenu;				//��Ϸ��ʼ�˵������
    T_Menu aboutmenu;				//��Ϸ���ڲ˵������
    T_Menu helpmenu;				//��Ϸ�����˵������
    MyMenu runmenu;					//��Ϸ���в˵������

    //����
    AudioDX ds;
    AudioDXBuffer backmusic_buffer;
    AudioDXBuffer robot_buffer;
    AudioDXBuffer mousedown_buffer;
    AudioDXBuffer mousemove_buffer;
    AudioDXBuffer explosionSound;

    //���幦�ܺ���
    void LoadPlayer();								// ������Ϸ��ҽ�ɫ
    void LoadMap();									// ������Ϸ��ͼ����	

    void UpdatePlayerPos();							// ������ҽ�ɫ��λ��
    void UpdateAnimation();							// ���½�ɫ����֡���к�
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

    void PaintStartMenu(HDC hdc);		//���ƿ�ʼ�˵�
    void PaintAboutMenu(HDC hdc);		//���ƹ��ڲ˵�
    void PaintHelpMenu(HDC hdc);		//���ư����˵�

    void MainGameInit();				//������Ϸ��ʼ��
    void PaintMainGame(HDC hdc);		//���Ʒ�����Ϸ

};

