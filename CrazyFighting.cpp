#include "CrazyFighting.h"

//���¶���֡����
int CrazyFighting::FRAME_LEFT[20] = { 0,0,1,1,1,1,2,2,2,2,0,0,1,1,1,1,2,2,2,2 };
int CrazyFighting::FRAME_RIGHT[20] = { 3,3,4,4,4,4,5,5,5,5,3,3,4,4,4,4,5,5,5,5 };


void CrazyFighting::LoadMap()
{
	t_scene.LoadTxtMap("map\\mymap.txt");
	scn_width = t_scene.getSceneLayers()->back().layer->GetWidth();
	scn_height = t_scene.getSceneLayers()->back().layer->GetHeight();
	//��ͼ��ʼλ���Ե�ͼ��Ϊ����
	int scn_x = (wnd_width - scn_width) / 2;
	int scn_y = (wnd_width - scn_height) / 2;
	//����Ϸ��ͼ��ʼ��Ϊ��Ļ����λ��
	t_scene.InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);
	//�����е�ͼͼ�㶨λ����Ļ����
	SCENE_LAYERS::iterator p;
	for (p = t_scene.getSceneLayers()->begin(); p != t_scene.getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map")
			p->layer->SetPosition(scn_x, scn_y);
	}
}

void CrazyFighting::LoadPlayer()
{
	player = new T_Sprite(L".\\res\\sprite\\blockorange.png", 80, 80);
	SPRITEINFO spInfo;
	spInfo.Active = false;
	spInfo.Dead = false;
	spInfo.Dir = DIR_UP;
	spInfo.Rotation = TRANS_NONE;
	spInfo.Ratio = 0.4f;
	spInfo.Level = 0;
	spInfo.Score = 0;
	spInfo.Speed = 4;
	spInfo.Alpha = 255;
	spInfo.Visible = true;
	spInfo.X = wnd_width / 5;
	spInfo.Y = (wnd_height - player->GetHeight()) / 2;
	player->Initiate(spInfo);
	player->SetSequence(FRAME_LEFT, 20);
	player->SetLayerTypeID(LAYER_PLY);

	GAMELAYER gameLayer;
	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = t_scene.getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene.Append(gameLayer);
}

CrazyFighting::~CrazyFighting(void)
{
}

CrazyFighting::CrazyFighting(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight, COLORREF bkColor)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	srand((unsigned)time(NULL));
	wnd_width = winwidth;
	wnd_height = winheight;
}

//void CrazyFighting::LoadMap()
//{
//
//}
//
//void CrazyFighting::LoadPlayer()
//{
//}
//
//void CrazyFighting::LoadNpc(int total)
//{
//}
//
//void CrazyFighting::UpdatePlayerPos(int dir)
//{
//}
//
//void CrazyFighting::UpdateNpcPos()
//{
//}
//
//void CrazyFighting::UpdateAnimation()
//{
//}
//
//void CrazyFighting::LoadMyMenu()
//{
//}

void CrazyFighting::GameInit()
{
	GameState = GAME_START;
	RunMenuInit();
	StartMenuInit();
	AboutMenuInit();
	HelpMenuInit();
	MainGameInit();

	if (!ds.CreateDS(m_hWnd)) return;

	backmusic_buffer.LoadWave(ds, L".\\sound\\backmusic.wav");
	robot_buffer.LoadWave(ds, L".\\sound\\robot.wav");
	mousedown_buffer.LoadWave(ds, L".\\sound\\mousedown.wav");
	mousemove_buffer.LoadWave(ds, L".\\sound\\mouseover.wav");
	explosionSound.LoadWave(ds, L".\\sound\\blast.wav");

	backmusic_buffer.Play(true);
}

void CrazyFighting::GameLogic()
{
	if (GameState == GAME_RUN)
	{
		GameKeyAction();		//���������Ϸ����
		if (player->IsActive())	//�ƶ���ҽ�ɫ
		{
		}
	}
}

void CrazyFighting::GameEnd()
{
	t_scene.RemoveAll();
	runmenu.DestroyAll();
	gamemenu.DestroyAll();
	aboutmenu.DestroyAll();
	menuArea.Destroy();
}

void CrazyFighting::GamePaint(HDC hdc)
{
	if (GameState == GAME_START)		//��ʾ��ʼ�˵�
	{
		PaintStartMenu(hdc);
	}
	else if (GameState == GAME_ABOUT)	//��ʾ��Ϸ����
	{
		PaintAboutMenu(hdc);
	}
	else if (GameState == GAME_HELP)	//��ʾ��Ϸ����
	{
		PaintHelpMenu(hdc);
	}
	else if (GameState == GAME_RUN)		//��ʼ��Ϸ
	{
		PaintMainGame(hdc);
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
	GetCursorPos(&mouse_pt);
	ScreenToClient(m_hWnd, &mouse_pt);

	if (Action == MOUSE_MOVE)//�����ѡ�а�ťʱ����ť�ı�״̬��ͼƬ��ɫ
	{
		gamemenu.MenuMouseMove(x, y);
		aboutmenu.MenuMouseMove(x, y);
		helpmenu.MenuMouseMove(x, y);
		runmenu.MenuMouseMove(x, y);
		if (GameState == GAME_RUN && opmode == true)
		{
			int p_dir = player->GetDir(mouse_pt);
			player->SetDir(p_dir);
			switch (p_dir)
			{
			case DIR_LEFT:
				player->SetSequence(FRAME_LEFT, 20);
				break;
			case DIR_RIGHT:
				player->SetSequence(FRAME_RIGHT, 20);
				break;

			}
			player->SetActive(true);
		}
	}
	if (Action == MOUSE_LCLICK)
	{
		if (GameState == GAME_START) //��ʼ�˵�
		{
			int index = gamemenu.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index)
				{
				case 0://��Ϸ����
					MainGameInit();//���³�ʼ������Ϸ
					GameState = GAME_RUN;
					break;
				case 1://����
					GameState = GAME_ABOUT;
					break;
				case 2://����
					GameState = GAME_HELP;
					break;
				case 3://�˳���Ϸ
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		if (GameState == GAME_ABOUT)
		{
			int index = aboutmenu.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index)
				{
				case 0://���ؿ�ʼ�˵�
					GameState = GAME_START;
					break;
				case 1://��ʼ��Ϸ
					MainGameInit();//���³�ʼ������Ϸ
					GameState = GAME_RUN;
					break;
				case 2://�˳���Ϸ
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		if (GameState == GAME_HELP)
		{
			int index = helpmenu.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index)
				{
				case 0://�������Ϸ����
					GameState = GAME_START;
					break;
				case 1://��ӿ�ʼ��Ϸ����
					MainGameInit();//���³�ʼ������Ϸ
					GameState = GAME_RUN;
					break;
				case 2://����˳���Ϸ����
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		if (GameState == GAME_RUN)
		{
			int index = runmenu.MenuMouseClick(x, y);
			if (runmenu.inrange && index >= 0)//����ʾ�����в˵���������Ч
			{
				switch (index)
				{
				case 0://���ؿ�ʼ�˵�
					GameState = GAME_START;
					break;
				case 1://���¿�ʼ
					MainGameInit();//���³�ʼ������Ϸ
					break;
				case 2:
					if (opmode == true)
						opmode = false;
					else
						opmode = true;
					break;
				case 3://�˳���Ϸ
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
			if (x > wnd_width - 181 && y < 9)//�������ڣ�inrange��Ϊ�棬��֮�����
			{
				runmenu.inrange = true;
			}
			else
			{
				runmenu.inrange = false;
			}
		}
	}
}

void CrazyFighting::StartMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"��ʼ",L"����",L"����",L"�˳�" };		//��ť����
	gamemenu.SetMenuBkg(L".\\res\\beach.jpg");			//���ò˵�����ͼƬ
	//�����ΰ�ť�˵���
	btn_width = 175;										//��ť��
	btn_height = 60;										//��ť��
	normalClr = Color::White;								//��ť����״̬������ɫ
	focusClr = Color::Blue;									//��ťѡ��״̬������ɫ
	gamemenu.SetBtnBmp(L".\\res\\button_blue.png", btn_width, btn_height);//���ð�ťͼƬ
	//���ò˵���Ϣ
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //���뷽ʽ����
	menuInfo.space = MENU_SPACE;		 //�˵���֮��ļ������
	menuInfo.width = btn_width;			 //�˵����
	menuInfo.height = btn_height;		 //	�˵����
	menuInfo.fontName = L"����";		 //�˵�������
	menuInfo.isBold = true;				 //�Ƿ����
	menuInfo.normalTextColor = normalClr;//����״̬������ɫ
	menuInfo.focusTextColor = focusClr;	 //ѡ��״̬������ɫ
	gamemenu.SetMenuInfo(menuInfo);

	MENUITEM mItem1, mItem2, mItem3, mItem4;
	x = wndWidth - btn_width * 1.5;			//��ʼ��ť
	y = wndHeight / 2 - btn_height * 1.5;
	mItem1.pos.x = x;
	mItem1.pos.y = y;
	mItem1.ItemName = menuItems[0];
	gamemenu.AddMenuItem(mItem1);

	x = wndWidth - btn_width * 1.5 - 40;		//���ڰ�ť
	y = wndHeight / 2;
	mItem2.pos.x = x;
	mItem2.pos.y = y;
	mItem2.ItemName = menuItems[1];
	gamemenu.AddMenuItem(mItem2);

	x = wndWidth - btn_width * 1.5;			//������ť
	y = wndHeight / 2 + btn_height * 1.5;
	mItem3.pos.x = x;
	mItem3.pos.y = y;
	mItem3.ItemName = menuItems[2];
	gamemenu.AddMenuItem(mItem3);

	x = wndWidth - btn_width * 1.5 - 40;		//�˳���ť
	y = wndHeight / 2 + btn_height * 3;
	mItem4.pos.x = x;
	mItem4.pos.y = y;
	mItem4.ItemName = menuItems[3];
	gamemenu.AddMenuItem(mItem4);

	gamemenu.SetClickSound(&mousedown_buffer);
	gamemenu.SetMoveSound(&mousemove_buffer);
	GameState = GAME_START;
}

void CrazyFighting::AboutMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"����",L"��ʼ",L"�˳�" };
	aboutmenu.SetMenuBkg(L".\\res\\menuback01.jpg");//���ñ���ͼƬ
	//�����ΰ�ť�˵���
	btn_width = 100;			//��ť���
	btn_height = 102;			//��ť�߶�
	normalClr = Color::LightGreen;	//��ť����ʱ��������ɫΪ��ɫ
	focusClr = Color::Blue;	//����ڰ�ť��ʱ��������ɫΪ��ɫ
	aboutmenu.SetBtnBmp(L".\\res\\button_bubble.png", btn_width, btn_height);//�˵���ťͼƬ
	//���ò˵���Ϣ
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //���뷽ʽ����
	menuInfo.space = MENU_SPACE;		 //�˵���֮��ļ������
	menuInfo.width = btn_width;			 //�˵����
	menuInfo.height = btn_height;		 //	�˵����
	menuInfo.fontName = L"����";		 //�˵�������
	menuInfo.isBold = true;				 //�Ƿ����
	menuInfo.normalTextColor = normalClr;//����״̬������ɫ
	menuInfo.focusTextColor = focusClr;	 //ѡ��״̬������ɫ
	aboutmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 3; i++) {
		//���ò˵����ťλ��
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 3 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 1.5 * btn_height;
		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		aboutmenu.AddMenuItem(mItem);//����ť��ӽ��˵�
	}
	aboutmenu.SetClickSound(&mousedown_buffer);
	aboutmenu.SetMoveSound(&mousemove_buffer);
}

void CrazyFighting::HelpMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"����",L"��ʼ",L"�˳���Ϸ" };
	helpmenu.SetMenuBkg(L".\\res\\seabed.jpg");//���ñ���ͼƬ
	//�����ΰ�ť�˵���
	btn_width = 100;
	btn_height = 102;
	normalClr = Color::LightGreen;
	focusClr = Color::White;
	helpmenu.SetBtnBmp(L".\\res\\button_bubble.png", btn_width, btn_height);
	//���ò˵���Ϣ
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //���뷽ʽ����
	menuInfo.space = MENU_SPACE;		 //�˵���֮��ļ������
	menuInfo.width = btn_width;			 //�˵����
	menuInfo.height = btn_height;		 //	�˵����
	menuInfo.fontName = L"����";		 //�˵�������
	menuInfo.isBold = true;				 //�Ƿ����
	menuInfo.normalTextColor = normalClr;//����״̬������ɫ
	menuInfo.focusTextColor = focusClr;	 //ѡ��״̬������ɫ
	helpmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 3; i++) {
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 3 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 2 * btn_height;

		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		helpmenu.AddMenuItem(mItem);
	}
	helpmenu.SetClickSound(&mousedown_buffer);
	helpmenu.SetMoveSound(&mousemove_buffer);
}

void CrazyFighting::RunMenuInit()
{
	menuArea.LoadImageFile(L".\\res\\menu_area.png");   //�������в˵�����ͼƬ
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"����",L"���¿�ʼ",L"�л�����",L"�˳�" };
	//	runmenu.SetMenuBkg(L".\\res\\bkg03.jpg");//���ñ���ͼƬ
		//�����ΰ�ť�˵���
	btn_width = 175;
	btn_height = 60;
	normalClr = Color::White;
	focusClr = Color::LightGreen;
	runmenu.SetBtnBmp(L".\\res\\button_blue.png", btn_width, btn_height);
	//���ò˵���Ϣ
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //���뷽ʽ����
	menuInfo.space = MENU_SPACE;		 //�˵���֮��ļ������
	menuInfo.width = btn_width;			 //�˵����
	menuInfo.height = btn_height;		 //	�˵����
	menuInfo.fontName = L"����";		 //�˵�������
	menuInfo.isBold = true;				 //�Ƿ����
	menuInfo.normalTextColor = normalClr;//����״̬������ɫ
	menuInfo.focusTextColor = focusClr;	 //ѡ��״̬������ɫ
	runmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 4; i++) {
		x = wndWidth - btn_width;
		y = btn_height * i + 12;

		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		runmenu.AddMenuItem(mItem);
	}
	runmenu.SetClickSound(&mousedown_buffer);
	runmenu.SetMoveSound(&mousemove_buffer);
}

void CrazyFighting::PaintStartMenu(HDC hdc)
{
	gamemenu.DrawMenu(hdc);
	RectF textRec;
	textRec.X = 30.00;
	textRec.Y = 30.00;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 4;
	T_Graph::PaintText(hdc, textRec, L"ʵ��ʮ��", 36, L"����", Color::Green, FontStyleBold, StringAlignmentNear);
	textRec.X = 29;
	textRec.Y = 29;
	T_Graph::PaintText(hdc, textRec, L"ʵ��ʮ��", 36, L"����", Color::Orange, FontStyleBold, StringAlignmentNear);
	textRec.X = 100;
	textRec.Y = wnd_height / 2 - (float)wnd_height * 3 / 20;
	T_Graph::PaintText(hdc, textRec, L"������������", 14, L"����", Color::Black, FontStyleRegular, StringAlignmentNear);
	textRec.Y = wnd_height / 2 - (float)wnd_height * 2 / 20;
	T_Graph::PaintText(hdc, textRec, L"ѧ�ţ�8002119225", 14, L"����", Color::Black, FontStyleRegular, StringAlignmentNear);
	textRec.Y = wnd_height / 2 - (float)wnd_height / 20;
	T_Graph::PaintText(hdc, textRec, L"ѡ�ΰࣺѡ��1��", 14, L"����", Color::Black, FontStyleRegular, StringAlignmentNear);
	textRec.Y = wnd_height / 2;
	T_Graph::PaintText(hdc, textRec, L"ԭ�༶���������1907��", 14, L"����", Color::Black, FontStyleRegular, StringAlignmentNear);

}

void CrazyFighting::PaintAboutMenu(HDC hdc)
{
	//���ƹ��ڲ˵�
	aboutmenu.DrawMenu(hdc);

	//���ư�͸����ɫ����
	T_Graph::PaintBlank(hdc, (float)wnd_width / 8, (float)wnd_height * 1 / 16, (float)wnd_width * 3 / 4, (float)wnd_height * 4 / 7, Color::Black, 60);
	//��������
	RectF textRec;
	textRec.X = 0.00;
	textRec.Y = 20.00;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 4;
	T_Graph::PaintText(hdc, textRec, L"����", 24, L"����", Color::White, FontStyleBold, StringAlignmentCenter);

	textRec.X = wnd_width / 2 - 100;
	textRec.Y = (float)wnd_height / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"���ƣ�ʵ��ʮ��", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 2 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"ѡ�ΰࣺѡ��1��", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"ԭ�༶���������1907��", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 4 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"ѧ�ţ�8002119225", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 5 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"������������", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 6 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"ʱ�䣺2021��12��23��", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);

}

void CrazyFighting::PaintHelpMenu(HDC hdc)
{
	helpmenu.DrawMenu(hdc);
	//���ư�͸����ɫ����
	T_Graph::PaintBlank(hdc, (float)wnd_width / 8, (float)wnd_height * 1 / 16, (float)wnd_width * 3 / 4, (float)wnd_height * 4 / 7, Color::Black, 60);

	RectF textRec;
	textRec.X = 0.00;
	textRec.Y = 0.00;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 3;
	T_Graph::PaintText(hdc, textRec, L"����", 36, L"����", Color::White, FontStyleBold, StringAlignmentCenter);

	textRec.X = wnd_width / 2 - 250;
	textRec.Y = (float)wnd_height / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"Ĭ�ϼ��̲��������������ҷ����ƶ���ҽ�ɫ��", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 2 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"�����Ϸ���в˵�����ѡ���л�������ʽΪ��������", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"�������������У������¼��̣����Զ��л�Ϊ���̲�����", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 4 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"��ѡ�����¿�ʼ��Ϸ��", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 5 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"�뾡������Ϸ��ͼ��̽����", 14, L"����", Color::White, FontStyleRegular, StringAlignmentNear);

}

void CrazyFighting::MainGameInit()
{
	opmode = false;
	t_scene.getSceneLayers()->clear();
	t_scene.RemoveAll();
	LoadMap();
	LoadPlayer();
}

void CrazyFighting::PaintMainGame(HDC hdc)
{
	t_scene.Draw(hdc, 0, 0);
	menuArea.PaintImage(hdc, wnd_width - 181, 0, 181, 9, 255);//�������в˵��������
	runmenu.DrawMenu(hdc, 0, 0, 255, GAME_RUN);
}
