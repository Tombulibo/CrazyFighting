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

//����vector�������͵���������vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class CrazyFighting :
    public T_Engine
{
private:
    //����ȫ���Եı���
    int scn_width, scn_height; //�������
    int wnd_width, wnd_height; //��Ϸ���ڿ��
    POINT mouse_pt; //��ǰ���λ��
    int op; //��������(op=0����; op=1���)

    //����Ҫʹ�õ������
    T_Sprite* player; //��Ϸ���
    vSpriteSet npc_set; //NPC��ɫ����
    T_Scene t_scene;

    //���幦�ܺ���
    void LoadMap(); //������Ϸ��ͼ����
    void LoadPlayer(); //������Ϸ���
    void LoadNpc(int total); //������ϷNPC
    void UpdatePlayerPos(int dir); //�������λ��
    void UpdateNpcPos(); //����NPCλ��
    void UpdateAnimation(); //������Ϸ����
    
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
};

