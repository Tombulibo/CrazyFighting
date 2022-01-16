#include "CrazyFighting.h"
#include "GameAI.h"

//���ص�ͼ
void CrazyFighting::LoadMap()
{ 

	if (mapType == 0)
	{
		t_scene.LoadTxtMap("map\\01\\background.txt");
		t_scene.LoadTxtMap("map\\01\\obstacle.txt");
		t_scene.LoadTxtMap("map\\01\\mask.txt");
		t_scene.LoadTxtMap("map\\01\\stone.txt");
		t_scene.LoadTxtMap("map\\01\\bush.txt");
		t_scene.LoadTxtMap("map\\01\\box.txt");
	}
	else if (mapType == 1)
	{
		t_scene.LoadTxtMap("map\\02\\gamelevel2_back.txt");
		t_scene.LoadTxtMap("map\\02\\gamelevel2_barrier.txt");
		t_scene.LoadTxtMap("map\\02\\gamelevel2_mask.txt");
	}
	else if (mapType == 2)
	{
		t_scene.LoadTxtMap("map\\03\\background.txt");
		t_scene.LoadTxtMap("map\\03\\barrier.txt");
		t_scene.LoadTxtMap("map\\03\\mask.txt");
	}

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
			player->SetSequence(player->FRAME_WALK, 20);
			player->updatePostion(DIR_LEFT, 0, 0, t_scene.getBarrier());
			//t_scene.ScrollScene(player);//��������
		}
		else if (player->GetDir() == DIR_RIGHT)
		{
			player->SetRotation(TRANS_NONE);
			player->SetSequence(player->FRAME_WALK, 20);
			player->updatePostion(DIR_RIGHT, 0, 0, t_scene.getBarrier());
			//t_scene.ScrollScene(player);//��������
		}
		t_scene.ScrollScene(player);//��������
		if (player->getJumping())
		{
			if (player->getJumpSpeed() < -2)
			{
				player->SetSequence(player->FRAME_JUMP, 20);
			}
			if (player->getJumpSpeed() > 2)
			{
				player->SetSequence(player->FRAME_FALL, 20);
			}
			player->jumpUpDown(t_scene.getBarrier());
			//t_scene.ScrollScene(player);//��������
		}
		if (player->getShooting())
		{
			player->SetSequence(player->FRAME_SHOOT, 40);
		}
		if (player->getAttacking())
		{
			player->SetSequence(player->FRAME_ATTACK, 20);
		}
		else
		{
			vCrazyManSet::iterator it;
			for (it = npc_set.begin(); it != npc_set.end(); it++) {
				if (player->CollideWith((*it),-50) && !(*it)->IsDead() && (*it)->IsVisible()) {
					if (isDelayCollision == false) {
						updateLifeCount = true;
						looseBlood = true;
						
					}
				}
			}
		}
	}
}

void CrazyFighting::updatePlayerlife() {
	LifeCount = LifeCount - 1;
	player->SetStartTime(GetTickCount64());

	isDelayCollision = true;
	updateLifeCount = false;

	player->SetVisible(true);
	player->SetActive(false);

	if (LifeCount == 0)
	{
		player->SetActive(false);
		player->SetVisible(false);
		player->SetDead(true);
		//player->SetSequence(player->FRAME_DIE, 70);
		GameState = GAME_OVER;
		if (mapType == 0) mapmusic01_buffer.Stop();
		else if (mapType == 1) mapmusic02_buffer.Stop();
		else if (mapType == 2) mapmusic03_buffer.Stop();
		gameovermusic_buffer.Play(false);
	}
}


//������Ϸ����
void CrazyFighting::UpdateAnimation()
{
	vCrazyManSet::iterator it;
	if (npc_set.size() > 0) {
		for (it = npc_set.begin(); it != npc_set.end(); it++) {

			if ((*it)->IsDead())
			{
				if ((*it)->LoopFrameOnce())
				{
					(*it)->SetVisible(false);
				}
			}
			else if ((*it)->IsVisible() == true && (*it)->IsActive() == true) (*it)->LoopFrame();
		}
	}

	if (player != NULL) {
		if (player->IsVisible() == true && player->IsActive() == true)
		{
			player->LoopFrame();
		}
	}
}

//������ҽ�ɫ
void CrazyFighting::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO spInfo;

	// ������ҽ�ɫ
	if (playerType == 0)
	{
		player = new CrazyMan(L".\\res\\sprite\\Shooter.png", 321, 284);
	}
	else if (playerType == 1) 
	{
		player = new CrazyMan(L".\\res\\sprite\\Fighter.png", 321, 284);
	}
	else if (playerType == 2)
	{
		player = new CrazyMan(L".\\res\\sprite\\Elemental.png", 321, 284);
	}

	player->setType(playerType);

	spInfo.Active = false;
	spInfo.Dead = false;
	spInfo.Dir = DIR_DOWN;
	spInfo.Rotation = TRANS_NONE;
	spInfo.Ratio = 0.3f;
	spInfo.Level = 0;
	spInfo.Score = 0;
	spInfo.Speed = 4;
	spInfo.Alpha = 255;
	spInfo.Visible = true;
	player->SetSequence(player->FRAME_JUMP, 20);
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
	ClearGameLevel();
}

CrazyFighting::CrazyFighting(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight, COLORREF bkColor)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	srand((unsigned)time(NULL));
	wnd_width = winwidth;
	wnd_height = winheight;
	chooseState = 0;
}
//NPC���س�ʼ��
void CrazyFighting::LoadNpc(int total)
{
	SPRITEINFO spInfo;
	for (int i = 0; i < total; i++) 
	{
		spInfo.Active = true;
		spInfo.Dead = false;
		spInfo.Rotation = TRANS_NONE;
		spInfo.Ratio = 0.3f;
		spInfo.Level = 0;
		spInfo.Score = 0;
		spInfo.Speed = 4;
		spInfo.Alpha = 255;
		spInfo.Visible = true;
		int sp_width = 321;
		int sp_height = 284;

		int t = rand() % 2;
		if (t == 0)
		{
			spInfo.Dir = DIR_LEFT;
		}
		else spInfo.Dir = DIR_RIGHT;

		int w = rand() % 5;
		if (mapType == 0)
		{
			switch (w) {
			case 0:
				spInfo.X = t_scene.getSceneX() + scn_width - 36 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 5 * 0.3 * sp_height;
				break;
			case 1:
				spInfo.X = t_scene.getSceneX() + scn_width - 30 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 9 * 0.3 * sp_height;
				break;
			case 2:
				spInfo.X = t_scene.getSceneX() + scn_width - 25 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 5 * 0.3 * sp_height;
				break;
			case 3:
				spInfo.X = t_scene.getSceneX() + scn_width - 22 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 3 * 0.3 * sp_height;
				break;
			case 4:
				spInfo.X = t_scene.getSceneX() + scn_width - 10 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 3 * 0.3 * sp_height;
				break;
			}
		}
		else if (mapType == 1)
		{
			switch (w) {
			case 0:
				spInfo.X = t_scene.getSceneX() + scn_width - 38 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 6 * 0.3 * sp_height;
				break;
			case 1:
				spInfo.X = t_scene.getSceneX() + scn_width - 25 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 6 * 0.3 * sp_height;
				break;
			case 2:
				spInfo.X = t_scene.getSceneX() + scn_width - 20 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 7 * 0.3 * sp_height;
				break;
			case 3:
				spInfo.X = t_scene.getSceneX() + scn_width - 15 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 8 * 0.3 * sp_height;
				break;
			case 4:
				spInfo.X = t_scene.getSceneX() + scn_width - 10 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 7 * 0.3 * sp_height;
				break;
			}
		}
		else if (mapType == 2)
		{
			switch (w) {
			case 0:
				spInfo.X = t_scene.getSceneX() + scn_width - 34 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 10 * 0.3 * sp_height;
				break;
			case 1:
				spInfo.X = t_scene.getSceneX() + scn_width - 25 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 5 * 0.3 * sp_height;
				break;
			case 2:
				spInfo.X = t_scene.getSceneX() + scn_width - 25 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 10 * 0.3 * sp_height;
				break;
			case 3:
				spInfo.X = t_scene.getSceneX() + scn_width - 17 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 5 * 0.3 * sp_height;
				break;
			case 4:
				spInfo.X = t_scene.getSceneX() + scn_width - 10 * 0.3 * sp_width;
				spInfo.Y = t_scene.getSceneY() + scn_height - 10 * 0.3 * sp_height;
				break;

			}
		}
		int npc_type = rand() % 3 + 3;
		if (npc_type == 3) npc_set.push_back(new CrazyMan(L".\\res\\sprite\\NPC1.png", 321, 284));
		else if (npc_type == 4) npc_set.push_back(new CrazyMan(L".\\res\\sprite\\NPC2.png", 321, 284));
		else if (npc_type == 5) npc_set.push_back(new CrazyMan(L".\\res\\sprite\\NPC3.png", 321, 284));
		
		CrazyMan* sp = npc_set.back();
		sp->setType(npc_type);
		sp->Initiate(spInfo);
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

void CrazyFighting::LoadTrap(int num, int level)
{
	if (level == 1)// �ڶ���������
	{
		SPRITEINFO trap_Info;
		for (int i = 0; i < num * 5; i++)
		{
			trap_Info.Active = true;

			trap_Info.Dead = false;

			trap_Info.Rotation = TRANS_NONE;

			trap_Info.Ratio = 1.0f;

			trap_Info.Level = 0;

			trap_Info.Score = 0;

			trap_Info.Speed = 2;

			trap_Info.Alpha = 255;

			trap_Info.Visible = true;

			int sp_width = 64;

			int sp_height = 64;

			trap_Info.Dir = DIR_UP;


			int a = rand() % 3;
			trap_Info.X = t_scene.getSceneX() + scn_width - (80 - 2 * i) * sp_width;

			trap_Info.Y = t_scene.getSceneY() + scn_height - 1.4 * sp_height;
			if (a == 0)
				trap_set.push_back(new CrazyMan(L".\\res\\sprite\\trap\\1.png", 64, 64));
			else if (a == 1)
				trap_set.push_back(new CrazyMan(L".\\res\\sprite\\trap\\2.png", 64, 64));
			else if (a == 2)
				trap_set.push_back(new CrazyMan(L".\\res\\sprite\\trap\\3.png", 64, 64));

			// ��ʼ�������ӵ���Ŀ

			CrazyMan* sp = trap_set.back();
			sp->Initiate(trap_Info);
			sp->SetLayerTypeID(LAYER_NPC);

			GAMELAYER gameLayer;
			gameLayer.layer = sp;
			gameLayer.type_id = LAYER_NPC;
			gameLayer.z_order = t_scene.getSceneLayers()->size() + 1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene.Append(gameLayer);

			sp = NULL;

		}
	}
}


//NPC���´���
void CrazyFighting::UpdateNpcPos()
{
	if (npc_set.size() == 0) {
		return;
	}
	GameAI* spAi = new GameAI(4);
	T_AI* fspAi = new T_AI(4);
	vCrazyManSet::iterator it;
	for (it = npc_set.begin(); it != npc_set.end(); it++) {
		if ((*it)->IsDead() == false && (*it)->IsActive() == true && (*it)->IsVisible() == true)
		{
			if ((*it)->getType() == 3 || (*it)->getType() == 4)
			{
				(*it)->SetActive(true);
				(*it)->setJumping(true);
				(*it)->setJumpSpeed(20);
				(*it)->setFallen(false);

				if ((*it)->GetDir() == DIR_LEFT)
				{
					(*it)->SetRotation(TRANS_HFLIP_NOROT);
					(*it)->SetSequence((*it)->FRAME_WALK, 20);
					(*it)->updatePostion(DIR_LEFT, 0, 0, t_scene.getBarrier(), 1);
				}
				else if ((*it)->GetDir() == DIR_RIGHT)
				{
					(*it)->SetRotation(TRANS_NONE);
					(*it)->SetSequence((*it)->FRAME_WALK, 20);
					(*it)->updatePostion(DIR_RIGHT, 0, 0, t_scene.getBarrier(), 1);
				}
				if ((*it)->getJumping())
				{
					(*it)->SetSequence((*it)->FRAME_WALK, 20);
					(*it)->jumpUpDown(t_scene.getBarrier(), 1);
					(*it)->setFallen(true);
				}
				if ((*it)->getFalling() == false)
				{
					(*it)->fallingDown();
				}
				spAi->Wander((*it), t_scene.getBarrier(),0);
				spAi->Chase((*it), player, t_scene.getBarrier());
			}
			else if ((*it)->getType() == 5)
			{
				int npc_dir = (*it)->GetDir();
				switch (npc_dir) {
				case DIR_LEFT:
					(*it)->SetRotation(TRANS_HFLIP_NOROT);
					(*it)->SetSequence((*it)->FRAME_WALK, 20);
					break;
				case DIR_RIGHT:
					(*it)->SetRotation(TRANS_NONE);
					(*it)->SetSequence((*it)->FRAME_WALK, 20);
					break;
				}
				fspAi->Wander((*it), t_scene.getBarrier());
			}
			if ((*it)->CollideWith(player, -50) && player->getAttacking() == false) {
				if (isDelayCollision == false) {
					updateLifeCount = true;
					looseBlood = true;
				}
			}
		}
	}
	for (it = trap_set.begin(); it != trap_set.end(); it++) {
		if ((*it)->CollideWith(player, 0) && player->getAttacking() == false)
		{
			if (isDelayCollision == false) {
				updateLifeCount = true;
				looseBlood = true;
			}
		}
	}
	delete spAi;
	delete fspAi;
}

// �����ڵ�
void CrazyFighting::LoadArrow(int iTime) {
	player->SetEndTime(GetTickCount64());
	if (player->GetEndTime() - player->GetStartTime() >= (DWORD)iTime)
	{
		player->SetStartTime(player->GetEndTime());

		GAMELAYER gameLayer;
		SPRITEINFO arrowInfo;
		int m_dir = player->getFaceTo();

		if (playerType == 0) arrow_set.push_back(new T_Sprite(L".\\res\\sprite\\ShooterArrow.png", 320, 128));
		else if (playerType == 1) arrow_set.push_back(new T_Sprite(L".\\res\\sprite\\FighterWeapon.png", 320, 128));
		else if (playerType == 2) arrow_set.push_back(new T_Sprite(L".\\res\\sprite\\ElementalSword.png", 320, 128));

		//��ʼ�������ӵ���Ŀ
		T_Sprite* arrow = arrow_set.back();
		arrowInfo.Active = true;
		arrowInfo.Dead = false;
		arrowInfo.Dir = m_dir;
		arrowInfo.Ratio = 0.1f;
		arrowInfo.Level = 0;
		arrowInfo.Score = 0;
		arrowInfo.Speed = 50;
		arrowInfo.Alpha = 255;

		switch (m_dir)
		{
		case DIR_LEFT:
			arrowInfo.Rotation = TRANS_HFLIP_NOROT;
			arrowInfo.X = player->GetX() + player->GetRatioSize().cx / 2 - 2;
			arrowInfo.Y = player->GetY() + player->GetRatioSize().cy - 25;
			break;
		case DIR_RIGHT:
			arrowInfo.Rotation = TRANS_NONE;
			arrowInfo.X = player->GetX() + player->GetRatioSize().cx / 2 - 2;
			arrowInfo.Y = player->GetY() + player->GetRatioSize().cy - 25;
			break;
		}

		arrowInfo.Visible = true;
		arrow->Initiate(arrowInfo);

		gameLayer.layer = arrow;

		gameLayer.type_id = LAYER_PLY_BOMB;
		gameLayer.layer->SetLayerTypeID(LAYER_PLY_BOMB);

		gameLayer.z_order = t_scene.GetTotalLayers() + 1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		t_scene.Append(gameLayer);
	}
}

// �����ڵ�λ��
void CrazyFighting::UpdateArrowPos() {
	if (arrow_set.size() == 0) return;

	vSpriteSet::iterator itp = arrow_set.begin();
	for (; itp != arrow_set.end();)
	{
		if ((*itp)->IsVisible() == true && (*itp)->IsActive() == true)
		{
			int SpeedX = 0, SpeedY = 0;
			switch ((*itp)->GetDir())
			{
			case DIR_LEFT:
				SpeedX = -(*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_RIGHT:
				SpeedX = (*itp)->GetSpeed();
				SpeedY = 0;
				break;
			}
			(*itp)->Move(SpeedX, SpeedY);

			//����ڵ��Ƿ����Ŀ��
			ArrowCollide((*itp));
			if (GameState != GAME_RUN) return;
		}

		if ((*itp)->IsVisible() == false ||
			(*itp)->GetY() < 0 || (*itp)->GetX() < 0 ||
			(*itp)->GetY() > wnd_height || (*itp)->GetX() > wnd_width)
		{
			//ɾ�������еĶ���	
			SCENE_LAYERS::iterator p;
			for (p = t_scene.getSceneLayers()->begin();
				p != t_scene.getSceneLayers()->end(); p++)
			{
				if ((*p).layer == (*itp))
				{
					p = t_scene.getSceneLayers()->erase(p);
					break;
				}
			}
			delete (*itp);
			itp = arrow_set.erase(itp);
		}

		if (itp == arrow_set.end())
		{
			break;
		}

		itp++;
	}
}
// ����ڵ��Ƿ����Ŀ��
void CrazyFighting::ArrowCollide(T_Sprite* arrow) {
	if (arrow->IsActive() == true && arrow->IsVisible() == true)
	{
		T_Map* map;
		bool collideBarrier = arrow->CollideWith(t_scene.getBarrier());
		if (collideBarrier)
		{
			map = t_scene.getBarrier();
			//map->setTile(bomb->GetMapBlockPT().x, bomb->GetMapBlockPT().y, 0);	//��ȡ�ϰ�������к�
			arrow->SetVisible(false);			//�ڵ����ɼ�
			arrow->SetActive(false);				//�ڵ�������
			int x = arrow->GetX() - 33;
			int y = arrow->GetY() - 33;
		}

		// �������ڵ�������NPC
		if (arrow->GetLayerTypeID() == LAYER_PLY_BOMB)
		{
			if (npc_set.size() > 0)
			{
				for (vCrazyManSet::iterator sp = npc_set.begin(); sp != npc_set.end(); sp++)
				{
					if (arrow->CollideWith((*sp)) && !((*sp)->IsDead()) &&
						((*sp)->IsVisible()) && ((*sp)->IsActive()))
					{
						(*sp)->SetActive(false);
						(*sp)->SetSequence((*sp)->FRAME_DIE, 70);
						(*sp)->SetDead(true);
						ScoreCount = ScoreCount + 10;
						arrow->SetActive(false);
						arrow->SetVisible(false);
						break;
					}

				}
			}
		}
	}
}
void CrazyFighting::AttackCollide()
{
	vCrazyManSet::iterator p;
	for (p = npc_set.begin(); p != npc_set.end(); p++) // ������NPC��ײ
	{
		// ��Ҵ�����NPC�Ĵ���
		if (player->CollideWith((*p)) && !(*p)->IsDead() && (*p)->IsVisible())
		{
			//(*p)->SetVisible(false);
			(*p)->SetActive(false);
			(*p)->SetSequence((*p)->FRAME_DIE, 70);
			(*p)->SetDead(true);
			ScoreCount = ScoreCount + 10;
		}
	}
}

//��Ϸ��ʼ��
void CrazyFighting::GameInit()
{
	lifeImg = NULL;
	levelImg = NULL;
	totalscoreBmp = NULL;
	//scoreImg = NULL;
	GameState = GAME_START;
	LoadImageRes();
	RunMenuInit();
	StartMenuInit();
	PlayerMenuInit();
	MapMenuInit();
	AboutMenuInit();
	HelpMenuInit();
	//MainGameInit();

	if (!ds.CreateDS(m_hWnd)) return;

	openmusic_buffer.LoadWave(ds, L".\\sound\\GameStart.wav");
	startmusic_buffer.LoadWave(ds, L".\\sound\\GameOpen.wav");
	mapmusic01_buffer.LoadWave(ds, L".\\sound\\Map01.wav");
	mapmusic02_buffer.LoadWave(ds, L".\\sound\\Map02.wav");
	mapmusic03_buffer.LoadWave(ds, L".\\sound\\Map03.wav");
	gameovermusic_buffer.LoadWave(ds, L".\\sound\\gameover.wav");
	victorymusic_buffer.LoadWave(ds, L".\\sound\\victory.wav");
	
	//robot_buffer.LoadWave(ds, L".\\sound\\robot.wav");
	mousedown_buffer.LoadWave(ds, L".\\sound\\mousedown.wav");
	mousemove_buffer.LoadWave(ds, L".\\sound\\mouseover.wav");
	//explosionSound.LoadWave(ds, L".\\sound\\blast.wav");

	openmusic_buffer.Play(true);
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
		if (player->IsDead() == false) 
		{
			// ��¼��ҽ�ɫ���ɺ��ʱ��
			player->SetEndTime(GetTickCount64());
			// �ж���ҽ�ɫ�޵�ʱ���Ƿ����
			if (player->GetEndTime() - player->GetStartTime() >= SUPER_TIME)
			{
				player->SetStartTime(player->GetEndTime());
				// �޵б�ʶ��λ
				isDelayCollision = false;
			}
			if (updateLifeCount == true) {
				updatePlayerlife();
			}
			if (ScoreCount == 100) {
				GameState = GAME_WIN;
				if (mapType == 0) mapmusic01_buffer.Stop();
				else if (mapType == 1) mapmusic02_buffer.Stop();
				else if (mapType == 2) mapmusic03_buffer.Stop();
				victorymusic_buffer.Play(false);
			}
		}
		UpdateNpcPos();
		UpdateArrowPos();
		UpdateAnimation();//���¶�����
	}
}

//��Ϸ����
void CrazyFighting::GameEnd()
{
	ClearGameLevel();
	t_scene.RemoveAll();
	runmenu.DestroyAll();
	startmenu.DestroyAll();
	playermenu.DestroyAll();
	mapmenu.DestroyAll();
	aboutmenu.DestroyAll();
	menuArea.Destroy();
}

//��Ϸ����
void CrazyFighting::GamePaint(HDC hdc)
{
	if (GameState == GAME_START)		//��ʾ��ʼ�˵�
	{
		if (chooseState == 0)
		{
			PaintStartMenu(hdc);
		}
		else if (chooseState == 1)
		{
			PaintPlayerMenu(hdc);
		}
		else if (chooseState == 2)
		{
			PaintMapMenu(hdc);
		}
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
		if (looseBlood == true)
		{
			T_Graph::PaintBlank(
				hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT,
				Color(150, 255, 0, 0));
			if (GetTickCount64() - player->GetStartTime() > 200)
			{
				looseBlood = false;
			}
		}
		else if (isDelayCollision == true)
		{
			T_Graph::PaintBlank(
				hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT,
				Color(50, 255, 215, 0));
		}
		DisplayInfo(hdc, GameState);
	}
	else if (GameState == GAME_OVER)
	{
		PaintMainGame(hdc);
		RectF textRec;
		textRec.X = 0.00;
		textRec.Y = 0.00;
		textRec.Width = (float)wnd_width;
		textRec.Height = (float)wnd_height;
		T_Graph::PaintBlank(
			hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT,
			Color(150, 0, 0, 0));
		T_Graph::PaintText(hdc, textRec, L"������", 100, L"����", Color::Red, FontStyleBold, StringAlignmentCenter);
	}
	else if (GameState == GAME_WIN)
	{
		PaintMainGame(hdc);
		RectF textRec;
		textRec.X = 0.00;
		textRec.Y = 0.00;
		textRec.Width = (float)wnd_width;
		textRec.Height = (float)wnd_height;
		T_Graph::PaintBlank(
			hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT,
			Color(150, 255, 255, 255));
		T_Graph::PaintText(hdc, textRec, L"��ս�ɹ�", 100, L"����", Color::Red, FontStyleBold, StringAlignmentCenter);
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
			if (CheckKey(67))
			{
				//player->SetSequence(player->FRAME_SHOOT, 40);
				
				if (player->IsDead() == false && player->IsVisible() == true && player->getType() != 1)
				{
					player->setShooting(true);
					//if (ChargeCount > 0)
					//{
					//LoadBomb(player, player_bomb_set, 400);
					LoadArrow(player->getShootTime());
					//ChargeCount = ChargeCount - 1;
				//}
				}
				player->SetActive(true);
			}
			else player->setShooting(false);

			if (CheckKey(VK_SPACE))
			{
				//player->SetSequence(player->FRAME_SHOOT, 40);

				if (player->IsDead() == false && player->IsVisible() == true)
				{
					player->setAttacking(true);
					AttackCollide();
				}
				player->SetActive(true);
			}
			else player->setAttacking(false);
			
			if (CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT) == false && CheckKey(VK_UP) == false && CheckKey(67) == false && CheckKey(VK_SPACE) == false)
			{
				if (player->getFalling() == false)
				{
					player->fallingDown();
				}
				if (player->getJumping() == false && player->getShooting() == false)
				{
					player->SetActive(false);
					player->SetSequence(player->FRAME_JUMP, 20);
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
		startmenu.MenuMouseMove(x, y);
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
			if (chooseState == 0)
			{
				int index = startmenu.MenuMouseClick(x, y);
				if (index >= 0)
				{
					switch (index)
					{
					case 0://��Ϸ����
						chooseState = 1;
						openmusic_buffer.Stop();
						startmusic_buffer.Play(true);
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
			else if (chooseState == 1)
			{
				int index = playermenu.MenuMouseClick(x, y);
				if (index >= 0)
				{
					switch (index)
					{
					case 0:// Ů����

						chooseState = 2;
						playerType = 0;
						break;
					case 1:// ��սʿ
						chooseState = 2;
						playerType = 1;
						break;
					case 2:// Ԫ��ʹ
						chooseState = 2;
						playerType = 2;
						break;
					case 3:// ����
						chooseState = 0;
						startmusic_buffer.Stop();
						openmusic_buffer.Play(true);
						break;
					}
				}
			}
			else if (chooseState == 2)
			{
				int index = playermenu.MenuMouseClick(x, y);
				if (index >= 0)
				{
					switch (index)
					{
					case 0:// ��԰
						chooseState = 0;
						GameState = GAME_RUN;
						mapType = 0;
						MainGameInit();//���³�ʼ������Ϸ
						startmusic_buffer.Stop();
						mapmusic01_buffer.Play(true);
						break;
					case 1:// ��
						chooseState = 0;
						GameState = GAME_RUN;
						mapType = 1;
						MainGameInit();//���³�ʼ������Ϸ
						startmusic_buffer.Stop();
						mapmusic02_buffer.Play(true);
						break;
					case 2:// ����
						chooseState = 0;
						GameState = GAME_RUN;
						mapType = 2;
						MainGameInit();//���³�ʼ������Ϸ
						startmusic_buffer.Stop();
						mapmusic03_buffer.Play(true);
						break;
					case 3:// ����
						chooseState = 1;
						break;
					}
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

				case 1://�˳���Ϸ
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

				case 1://����˳���Ϸ����
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
					GameEnd();
					GameInit();
					GameState = GAME_START;
					if (mapType == 0) mapmusic01_buffer.Stop();
					else if (mapType == 1) mapmusic02_buffer.Stop();
					else if (mapType == 2) mapmusic03_buffer.Stop();
					openmusic_buffer.Play(true);
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
	startmenu.SetMenuBkg(L".\\res\\beach.jpg");			//���ò˵�����ͼƬ
	//�����ΰ�ť�˵���
	btn_width = 175;										//��ť��
	btn_height = 60;										//��ť��
	normalClr = Color::White;								//��ť����״̬������ɫ
	focusClr = Color::Blue;									//��ťѡ��״̬������ɫ
	startmenu.SetBtnBmp(L".\\res\\button_blue.png", btn_width, btn_height);//���ð�ťͼƬ
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
	startmenu.SetMenuInfo(menuInfo);

	MENUITEM mItem1, mItem2, mItem3, mItem4;
	x = wndWidth - btn_width * 1.5;			//��ʼ��ť
	y = wndHeight / 2 - btn_height * 1.5;
	mItem1.pos.x = x;
	mItem1.pos.y = y;
	mItem1.ItemName = menuItems[0];
	startmenu.AddMenuItem(mItem1);

	x = wndWidth - btn_width * 1.5 - 40;		//���ڰ�ť
	y = wndHeight / 2;
	mItem2.pos.x = x;
	mItem2.pos.y = y;
	mItem2.ItemName = menuItems[1];
	startmenu.AddMenuItem(mItem2);

	x = wndWidth - btn_width * 1.5;			//������ť
	y = wndHeight / 2 + btn_height * 1.5;
	mItem3.pos.x = x;
	mItem3.pos.y = y;
	mItem3.ItemName = menuItems[2];
	startmenu.AddMenuItem(mItem3);

	x = wndWidth - btn_width * 1.5 - 40;		//�˳���ť
	y = wndHeight / 2 + btn_height * 3;
	mItem4.pos.x = x;
	mItem4.pos.y = y;
	mItem4.ItemName = menuItems[3];
	startmenu.AddMenuItem(mItem4);

	startmenu.SetClickSound(&mousedown_buffer);
	startmenu.SetMoveSound(&mousemove_buffer);
	GameState = GAME_START;
}

//���ڲ˵���ʼ��
void CrazyFighting::AboutMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"����",L"�˳�" };
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
	for (int i = 0; i < 2; i++) {
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
	wstring menuItems[] = { L"����",L"�˳���Ϸ" };
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
	for (int i = 0; i < 2; i++) {
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

//��Ϸѡ�˲˵���ʼ��
void CrazyFighting::PlayerMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"Ů����",L"��սʿ",L"Ԫ��ʹ",L"����" };
	playermenu.SetMenuBkg(L".\\res\\seabed.jpg");//���ñ���ͼƬ
	//�����ΰ�ť�˵���
	btn_width = 100;
	btn_height = 102;
	normalClr = Color::LightGreen;
	focusClr = Color::White;
	playermenu.SetBtnBmp(L".\\res\\button_bubble.png", btn_width, btn_height);
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
	playermenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 4; i++) {
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 3 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 2 * btn_height;

		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		playermenu.AddMenuItem(mItem);
	}
	playermenu.SetClickSound(&mousedown_buffer);
	playermenu.SetMoveSound(&mousemove_buffer);
}

//��Ϸѡ��ͼ�˵���ʼ��
void CrazyFighting::MapMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"��԰",L"��",L"����",L"����" };
	mapmenu.SetMenuBkg(L".\\res\\seabed.jpg");//���ñ���ͼƬ
	//�����ΰ�ť�˵���
	btn_width = 100;
	btn_height = 102;
	normalClr = Color::LightGreen;
	focusClr = Color::White;
	mapmenu.SetBtnBmp(L".\\res\\button_bubble.png", btn_width, btn_height);
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
	mapmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 4; i++) {
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 3 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 2 * btn_height;

		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		mapmenu.AddMenuItem(mItem);
	}
	mapmenu.SetClickSound(&mousedown_buffer);
	mapmenu.SetMoveSound(&mousemove_buffer);
}

//��ʼ�˵�����
void CrazyFighting::PaintStartMenu(HDC hdc)
{
	startmenu.DrawMenu(hdc);
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

void CrazyFighting::PaintPlayerMenu(HDC hdc)
{
	playermenu.DrawMenu(hdc);
}

void CrazyFighting::PaintMapMenu(HDC hdc)
{
	mapmenu.DrawMenu(hdc);
}

// ������ϷͼƬ��Դ
void CrazyFighting::LoadImageRes()
{
	if (lifeImg == NULL) lifeImg = new T_Graph(L".\\res\\game\\life.png");
	//if (scoreImg == NULL) scoreImg = new T_Graph(L".\\res\\game\\score.png");
	if (levelImg == NULL) levelImg = new T_Graph(L".\\res\\game\\Level.png");
	if (totalscoreBmp == NULL) totalscoreBmp = new T_Graph(L".\\res\\game\\totalscore.png");
}

// ����ؿ�����
void CrazyFighting::ClearGameLevel()
{
	npc_set.clear();		npc_set.swap(vector<CrazyMan*>());
	arrow_set.clear();		arrow_set.swap(vector<T_Sprite*>());
	/*
	vCrazyManSet::iterator it;
	for (it = npc_set.begin(); it != npc_set.end(); it++)
	{
		delete *it;
		(*it) = NULL;
	}
	vSpriteSet::iterator p;
	for (p = arrow_set.begin(); p != arrow_set.end(); p++)
	{
		delete* p;
		(*p) = NULL;
	}*/
	//npc_set.clear();
	//arrow_set.clear();
	t_scene.getSceneLayers()->clear();
	t_scene.RemoveAll();
}

//������Ϸ��ʼ��
void CrazyFighting::MainGameInit()
{
	ClearGameLevel();
	LoadMap();
	LoadPlayer();
	LoadNpc(NPC_NUM);
	LoadTrap(NPC_NUM, mapType);
	LifeCount = 3;
	ScoreCount = 0;
	isDelayCollision = true;


	player->SetActive(true);
	player->setJumping(true);
	player->setJumpSpeed(0);
	player->setFallen(false);
}

//��Ϸ���л���
void CrazyFighting::PaintMainGame(HDC hdc)
{	
	back.PaintBlank(hdc, 0, 0, wnd_width, wnd_height, Color(3, 127, 237));
	t_scene.Draw(hdc, 0, 0);	
	menuArea.PaintImage(hdc, wnd_width - 181, 0, 181, 9, 255);//�������в˵��������
	runmenu.DrawMenu(hdc, 0, 0, 255, GAME_RUN);
}

// ��Ϸ״̬��ʾ
void CrazyFighting::DisplayInfo(HDC hdc, int game_state)
{
	int FontHeight = 0;//�ֺ�
	Gdiplus::RectF rect;
	wstring Content = L"";
	rect.X = 50.00;
	rect.Width = 768.00;

	int strLen = 220;
	int iconW = 40;
	int x = 12;
	int y = 16;
	rect.Y = (REAL)y;
	rect.Height = (REAL)30;
	rect.Width = (REAL)200;
	FontHeight = 20;

	levelImg->PaintImage(
		hdc, x, y - 4, levelImg->GetImageWidth(),
		levelImg->GetImageHeight(), 200
	);
	wstring LeveNum = L"��ǰ��: ";
	LeveNum.append(T_Util::int_to_wstring(mapType + 1));
	rect.X = (REAL)(x + iconW);
	T_Graph::PaintText(hdc, rect, LeveNum.c_str(), (REAL)FontHeight, L"����",
		Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);

	x = x + strLen;
	lifeImg->PaintImage(
		hdc, x, y - 4, lifeImg->GetImageWidth(),
		lifeImg->GetImageHeight(), 200
	);
	wstring lifeleft = L"����ֵ: ";
	lifeleft.append(T_Util::int_to_wstring(LifeCount));
	rect.X = (REAL)(x + iconW);
	T_Graph::PaintText(hdc, rect, lifeleft.c_str(), (REAL)FontHeight, L"����",
		Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);

	x = x + strLen;
	totalscoreBmp->PaintImage(
		hdc, x, y - 4, totalscoreBmp->GetImageWidth(),
		totalscoreBmp->GetImageHeight(), 200
	);
	wstring ScoreNum = L"�ܵ÷�: ";
	ScoreNum.append(T_Util::int_to_wstring(ScoreCount));
	rect.X = (REAL)(x + iconW);
	T_Graph::PaintText(hdc, rect, ScoreNum.c_str(), (REAL)FontHeight, L"����",
		Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);
}