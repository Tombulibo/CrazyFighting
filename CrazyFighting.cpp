#include "CrazyFighting.h"

//���¶���֡����
int CrazyFighting::FRAME_LEFT[20] = { 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2 };
int CrazyFighting::FRAME_RIGHT[20] = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5 };
int CrazyFighting::FRAME_UP[20] = { 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8 };
int CrazyFighting::FRAME_DOWN[20] = { 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11 };
int CrazyFighting::EXPLOSION_FRAME[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

//���ص�ͼ
void CrazyFighting::LoadMap()
{ 
	//t_scene.LoadTxtMap("map\\mymap.txt");
	//t_scene.LoadTxtMap("map\\jump_map.txt");
	t_scene.LoadTxtMap("map\\01\\background.txt");
	t_scene.LoadTxtMap("map\\01\\obstacle.txt");

	scn_width = t_scene.getSceneLayers()->back().layer->GetWidth();
	scn_height = t_scene.getSceneLayers()->back().layer->GetHeight();
	//��ͼ��ʼλ���Ե�ͼ��Ϊ����
	int scn_x = 0;
	int scn_y = wnd_height - scn_height;
	// ����Ϸ��ͼ��ʼ��Ϊ���Ͻ�
	//int scn_x = 0;
	//int scn_y = 0;

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

//�������λ��
void CrazyFighting::UpdatePlayerPos()
{
	if (player == NULL) return;
	if (player->IsDead() == false && player->IsVisible() == true && player->IsActive() == true)
	{
		// ������ҵ���Ծ�������ƶ�
		if (player->GetDir() == DIR_LEFT)
		{
			player->SetRotation(TRANS_HFLIP_NOROT);
			player->SetSequence(player->SHOOTER_WALK, 20);
			player->updatePostion(DIR_LEFT, 0, 0, t_scene.getBarrier());
			//t_scene.ScrollScene(player);//��������
		}
		else if (player->GetDir() == DIR_RIGHT)
		{
			player->SetRotation(TRANS_NONE);
			player->SetSequence(player->SHOOTER_WALK, 20);
			player->updatePostion(DIR_RIGHT, 0, 0, t_scene.getBarrier());
			//t_scene.ScrollScene(player);//��������
		}
		t_scene.ScrollScene(player);//��������
		if (player->getJumping())
		{
			if (player->getJumpSpeed() < -2)
			{
				player->SetSequence(player->SHOOTER_JUMP, 20);
			}
			if (player->getJumpSpeed() > 2)
			{
				player->SetSequence(player->SHOOTER_FALL, 20);
			}
			//player->SetSequence(FRAME_UP, 20);
			player->jumpUpDown(t_scene.getBarrier());
			//t_scene.ScrollScene(player);//��������
		}
		if (player->getShooting())
		{
			player->SetSequence(player->SHOOTER_SHOOT, 40);
			//player->LoadArrow(400, &t_scene);
		}
	}
}

//������Ϸ����
void CrazyFighting::UpdateAnimation()
{
	vSpriteSet::iterator it;
	if (npc_set.size() > 0) {
		for (it = npc_set.begin(); it != npc_set.end(); it++) {
			(*it)->LoopFrame();
		}
	}

	if (player != NULL) {
		if (player->IsVisible() == true && player->IsActive() == true) player->LoopFrame();
	}
	/*
	if (explosion_set.size() > 0)
	{
		vSpriteSet::iterator e = explosion_set.begin();
		for (; e != explosion_set.end();)
		{
			if ((*e)->LoopFrameOnce() == true)
			{
				// ɾ���������Ѿ�ʧЧ�ı�ը����	
				SCENE_LAYERS::iterator lp;
				for (lp = t_scene.getSceneLayers()->begin(); lp != t_scene.getSceneLayers()->end(); lp++)
				{
					if ((*lp).layer == (*e))
					{
						lp = t_scene.getSceneLayers()->erase(lp);
						break;
					}
				}
				delete (*e);
				e = explosion_set.erase(e);
				continue;
			}
			else
			{
				++e;
			}
		}
	}*/
}

//������ҽ�ɫ
void CrazyFighting::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO spInfo;

	// ������ҽ�ɫ
	player = new CrazyMan(L".\\res\\sprite\\Shooter\\Crazyman_lina.png", 321, 284);

	spInfo.Active = false;
	spInfo.Dead = false;
	spInfo.Dir = DIR_RIGHT;
	spInfo.Rotation = TRANS_NONE;
	spInfo.Ratio = 0.3f;
	spInfo.Level = 0;
	spInfo.Score = 0;
	spInfo.Speed = 4;
	spInfo.Alpha = 255;
	spInfo.Visible = true;
	player->SetSequence(player->SHOOTER_JUMP, 20);
	player->Initiate(spInfo);
	player->SetLayerTypeID(LAYER_PLY);

	int x = 0;
	int y = player->GetRatioSize().cy;
	player->SetPosition(x, y);

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

//NPC���س�ʼ��
//void CrazyFighting::LoadNpc(int total)
//{
//}
//
//NPC���´���
//void CrazyFighting::UpdateNpcPos()
//{
//}
//
void CrazyFighting::LoadNpc(int total)
{
	SPRITEINFO spInfo;
	for (int i = 0; i < total; i++)
	{
		spInfo.Active = true;
		spInfo.Dead = false;
		spInfo.Rotation = TRANS_NONE;
		spInfo.Ratio = 0.5f;
		spInfo.Speed = 2;
		spInfo.Alpha = 255;
		spInfo.Visible = true;
		spInfo.Level = 0;
		spInfo.Score = 0;
		int sp_width = 40;
		int sp_height = 40;
		//��ȡ�漴����NPC��ɫ�ڵ�ͼ���Ľ����ɣ�
		int sdr = rand() % 4;
		int d = rand() % 2;
		switch (sdr)
		{
		case 0: //���Ͻ�
			if (d == 0) spInfo.Dir = DIR_RIGHT;
			if (d == 1) spInfo.Dir = DIR_DOWN;
			spInfo.X = 0;
			spInfo.Y = 0;
			break;
		case 1://���Ͻ�
			if (d == 0) spInfo.Dir = DIR_LEFT;
			if (d == 1) spInfo.Dir = DIR_DOWN;
			spInfo.X = wnd_width - sp_width;
			spInfo.Y = 0;
			break;
		case 2://���½�
			if (d == 0) spInfo.Dir = DIR_RIGHT;
			if (d == 1) spInfo.Dir = DIR_UP;
			spInfo.X = 0;
			spInfo.Y = wnd_height - sp_height;
			break;
		case 3://���½�
			if (d == 0) spInfo.Dir = DIR_LEFT;
			if (d == 1) spInfo.Dir = DIR_UP;
			spInfo.X = wnd_width - sp_width;
			spInfo.Y = wnd_height - 2 * sp_height;
			break;
		}
		//��NPC�б��������µ���Ŀ
		npc_set.push_back(new T_Sprite(L".\\res\\blockgreen.png", 80, 80));
		//��ʼ�������ӵ���Ŀ
		T_Sprite* sp = npc_set.back();
		sp->Initiate(spInfo);
		switch (spInfo.Dir)
		{
		case DIR_LEFT:
			sp->SetSequence(FRAME_LEFT, 20);
			break;
		case DIR_RIGHT:
			sp->SetSequence(FRAME_RIGHT, 20);
			break;
		case DIR_UP:
			sp->SetSequence(FRAME_UP, 20);
			break;
		case DIR_DOWN:
			sp->SetSequence(FRAME_DOWN, 20);
			break;
		}
		sp->SetLayerTypeID(LAYER_NPC);

		GAMELAYER gamelayer;
		gamelayer.layer = sp;
		gamelayer.type_id = LAYER_NPC;
		gamelayer.z_order = t_scene.getSceneLayers()->size() + 1;
		gamelayer.layer->setZorder(gamelayer.z_order);
		t_scene.Append(gamelayer);

		sp = NULL;
	}
}
void CrazyFighting::UpdateNpcPos()
{
	if (npc_set.size() == 0) return;
	//����T_AI����
	T_AI* spAi = new T_AI(4);
	//����ȫ��NPC
	vSpriteSet::iterator p;
	for (p = npc_set.begin(); p != npc_set.end(); p++)
	{
		if ((*p)->IsActive() == true && (*p)->IsVisible() == true)
		{
			spAi->Evade((*p), player); //�����ҽ�ɫ
			spAi->CheckOverlay((*p), npc_set); //��ֹ�����ص�
			int npc_dir = (*p)->GetDir(); //��ȡ��ǰ����
			//���ݵ�ǰ����ʱ���¶�Ӧ��֡��������
			switch (npc_dir)
			{
			case DIR_LEFT:
				(*p)->SetSequence(FRAME_LEFT, 20);
				break;
			case DIR_RIGHT:
				(*p)->SetSequence(FRAME_RIGHT, 20);
				break;
			case DIR_UP:
				(*p)->SetSequence(FRAME_UP, 20);
				break;
			case DIR_DOWN:
				(*p)->SetSequence(FRAME_DOWN, 20);
				break;
			}
			//NPC�ڵ�ͼ������,���Զ�����ͼ�ϰ�
			spAi->Wander((*p), t_scene.getBarrier());
		}
	}
	delete spAi;
}

//��Ϸ��ʼ��
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

//��Ϸ�߼�
void CrazyFighting::GameLogic()
{
	if (GameState == GAME_RUN)
	{
		GameKeyAction();		//���������Ϸ����
		if (player->IsActive())	//�ƶ���ҽ�ɫ
		{
			UpdatePlayerPos();//�ƶ���ҽ�ɫ
		}
		UpdateNpcPos();
		player->UpdateArrowPos(&t_scene, npc_set);
		UpdateAnimation();//���¶�����
	}
}

//��Ϸ����
void CrazyFighting::GameEnd()
{
	t_scene.RemoveAll();
	runmenu.DestroyAll();
	gamemenu.DestroyAll();
	aboutmenu.DestroyAll();
	menuArea.Destroy();
}

//��Ϸ����
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

//��Ϸ���̲���
void CrazyFighting::GameKeyAction(int Action)
{
	if (Action == KEY_SYS_NONE)//����keys����״̬��������Ϊ
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
			if (CheckKey(VK_LEFT))
			{
				player->setFalling(false);
				player->SetDir(DIR_LEFT);
				player->setFaceTo(DIR_LEFT);
				player->SetActive(true);
			}

			else if (CheckKey(VK_RIGHT))
			{
				player->setFalling(false);
				player->SetDir(DIR_RIGHT);
				player->setFaceTo(DIR_RIGHT);
				player->SetActive(true);
			}
			if (CheckKey(VK_UP))
			{
				if (player->getJumping() == false)
				{
					player->SetDir(DIR_UP);
					player->SetActive(true);
					player->setJumping(true);
					player->setJumpSpeed(-20);
					player->setFallen(false);
				}
			}
			if (CheckKey(VK_SHIFT) && (CheckKey(VK_LEFT) || CheckKey(VK_RIGHT)))
			{
				if (player->GetSpeed() < 10)
				{
					player->SetSpeed(player->GetSpeed() + 1);
				}
			}
			else
			{
				if (player->GetSpeed() > 4)
				{
					player->SetSpeed(player->GetSpeed() - 1);
				}
			}
			if (CheckKey(VK_DOWN))
			{
				player->SetSequence(player->SHOOTER_SLIDE, 20);
				player->SetActive(true);
			}
			if (CheckKey(VK_SPACE))
			{
				//player->SetSequence(player->FRAME_SHOOT, 40);
				
				if (player->IsDead() == false && player->IsVisible() == true)
				{
					player->setShooting(true);
					//if (ChargeCount > 0)
					//{
					//LoadBomb(player, player_bomb_set, 400);
					player->LoadArrow(player->getShootTime(), &t_scene);
					//ChargeCount = ChargeCount - 1;
				//}
				}
				player->SetActive(true);
			}
			else player->setShooting(false);
			if (CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT) == false && CheckKey(VK_UP) == false && CheckKey(VK_DOWN) == false && CheckKey(VK_SPACE) == false)
			{
				if (player->getFalling() == false)
				{
					player->fallingDown();
				}
				if (player->getJumping() == false)
				{
					player->SetActive(false);
					player->SetSequence(player->SHOOTER_JUMP, 20);
					player->SetFrame(0);
				}

			}
		}
	}
}

//��Ϸ������
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
		if (GameState == GAME_RUN)
		{

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
				case 2://�˳���Ϸ
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

//��ʼ�˵���ʼ��
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

//���ڲ˵���ʼ��
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

//�����˵���ʼ��
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

//��Ϸ���в˵���ʼ��
void CrazyFighting::RunMenuInit()
{
	menuArea.LoadImageFile(L".\\res\\menu_area.png");   //�������в˵�����ͼƬ
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"����",L"���¿�ʼ",L"�˳�" };
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
	for (int i = 0; i < 3; i++) {
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

//��ʼ�˵�����
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

//���ڲ˵�����
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

//�����˵�����
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

//������Ϸ��ʼ��
void CrazyFighting::MainGameInit()
{
	t_scene.getSceneLayers()->clear();
	t_scene.RemoveAll();
	LoadMap();
	LoadPlayer();
	LoadNpc(NPC_NUM);
}

//��Ϸ���л���
void CrazyFighting::PaintMainGame(HDC hdc)
{	
	back.PaintBlank(hdc, 0, 0, wnd_width, wnd_height, Color(3, 127, 237));
	t_scene.Draw(hdc, 0, 0);	
	menuArea.PaintImage(hdc, wnd_width - 181, 0, 181, 9, 255);//�������в˵��������
	runmenu.DrawMenu(hdc, 0, 0, 255, GAME_RUN);
}
