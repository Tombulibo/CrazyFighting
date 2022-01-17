#include "CrazyFighting.h"
#include "GameAI.h"

//加载地图
void CrazyFighting::LoadMap()
{ 

	if (mapType == 0)
	{
		t_scene.LoadTxtMap(".\\res\\map\\01\\background.txt");
		t_scene.LoadTxtMap(".\\res\\map\\01\\obstacle.txt");
		t_scene.LoadTxtMap(".\\res\\map\\01\\mask.txt");
		t_scene.LoadTxtMap(".\\res\\map\\01\\stone.txt");
		t_scene.LoadTxtMap(".\\res\\map\\01\\bush.txt");
		t_scene.LoadTxtMap(".\\res\\map\\01\\box.txt");
	}
	else if (mapType == 1)
	{
		t_scene.LoadTxtMap(".\\res\\map\\02\\gamelevel2_back.txt");
		t_scene.LoadTxtMap(".\\res\\map\\02\\gamelevel2_barrier.txt");
		t_scene.LoadTxtMap(".\\res\\map\\02\\gamelevel2_mask.txt");
	}
	else if (mapType == 2)
	{
		t_scene.LoadTxtMap(".\\res\\map\\03\\background.txt");
		t_scene.LoadTxtMap(".\\res\\map\\03\\barrier.txt");
		t_scene.LoadTxtMap(".\\res\\map\\03\\mask.txt");
	}

	scn_width = t_scene.getSceneLayers()->back().layer->GetWidth();
	scn_height = t_scene.getSceneLayers()->back().layer->GetHeight();
	//视图初始位置以地图作为参照
	int scn_x = 0;
	int scn_y = wnd_height - scn_height;
	// 将游戏地图初始化为左上角
	//int scn_x = 0;
	//int scn_y = 0;

	//将游戏地图初始化为屏幕中央位置
	t_scene.InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);
	//将所有地图图层定位在屏幕中央
	SCENE_LAYERS::iterator p;
	for (p = t_scene.getSceneLayers()->begin(); p != t_scene.getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map")
			p->layer->SetPosition(scn_x, scn_y);
	}
}

//加载玩家位置
void CrazyFighting::UpdatePlayerPos()
{
	if (player == NULL) return;
	if (player->IsDead() == false && player->IsVisible() == true && player->IsActive() == true)
	{
		// 处理玩家的跳跃和左右移动
		if (player->GetDir() == DIR_LEFT)
		{
			player->SetRotation(TRANS_HFLIP_NOROT);
			player->SetSequence(player->FRAME_WALK, 20);
			player->updatePostion(DIR_LEFT, 0, 0, t_scene.getBarrier());
			//t_scene.ScrollScene(player);//滚动背景
		}
		else if (player->GetDir() == DIR_RIGHT)
		{
			player->SetRotation(TRANS_NONE);
			player->SetSequence(player->FRAME_WALK, 20);
			player->updatePostion(DIR_RIGHT, 0, 0, t_scene.getBarrier());
			//t_scene.ScrollScene(player);//滚动背景
		}
		t_scene.ScrollScene(player);//滚动背景
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
			//t_scene.ScrollScene(player);//滚动背景
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
	if (player->getType() == 0)
	{
		femaleplayerdiedmusic_buffer.Play(false);
	}
	else if (player->getType() == 1 || player->getType() == 2)
	{
		maleplayerdiedmusic_buffer.Play(false);
	}
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


//更新游戏动画
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

//加载玩家角色
void CrazyFighting::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO spInfo;

	// 加载玩家角色
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
//NPC加载初始化
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
	if (level == 1)// 第二关有陷阱
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

			trap_Info.Y = t_scene.getSceneY() + scn_height - sp_height;
			if (a == 0)
				trap_set.push_back(new CrazyMan(L".\\res\\sprite\\trap\\1.png", 64, 64));
			else if (a == 1)
				trap_set.push_back(new CrazyMan(L".\\res\\sprite\\trap\\2.png", 64, 64));
			else if (a == 2)
				trap_set.push_back(new CrazyMan(L".\\res\\sprite\\trap\\3.png", 64, 64));

			// 初始化刚增加的项目

			T_Sprite* sp = trap_set.back();
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


//NPC更新处理
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
	vSpriteSet::iterator p;
	for (p = trap_set.begin(); p != trap_set.end(); p++) {
		if ((*p)->CollideWith(player, 0) && player->getAttacking() == false)
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

// 加载炮弹
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

		//初始化刚增加的项目
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

// 更新炮弹位置
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

			//检测炮弹是否击中目标
			ArrowCollide((*itp));
			if (GameState != GAME_RUN) return;
		}

		if ((*itp)->IsVisible() == false ||
			(*itp)->GetY() < 0 || (*itp)->GetX() < 0 ||
			(*itp)->GetY() > wnd_height || (*itp)->GetX() > wnd_width)
		{
			//删除场景中的对象	
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
// 检测炮弹是否击中目标
void CrazyFighting::ArrowCollide(T_Sprite* arrow) {
	if (arrow->IsActive() == true && arrow->IsVisible() == true)
	{
		T_Map* map;
		bool collideBarrier = arrow->CollideWith(t_scene.getBarrier());
		if (collideBarrier)
		{
			map = t_scene.getBarrier();
			//map->setTile(bomb->GetMapBlockPT().x, bomb->GetMapBlockPT().y, 0);	//获取障碍物的行列号
			arrow->SetVisible(false);			//炮弹不可见
			arrow->SetActive(false);				//炮弹不可移
			int x = arrow->GetX() - 33;
			int y = arrow->GetY() - 33;
		}

		// 如果玩家炮弹碰到了NPC
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
						npcdiedmusic_buffer.Play(false);
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
	for (p = npc_set.begin(); p != npc_set.end(); p++) // 处理与NPC碰撞
	{
		// 玩家触碰到NPC的处理
		if (player->CollideWith((*p)) && !(*p)->IsDead() && (*p)->IsVisible())
		{
			//(*p)->SetVisible(false);
			(*p)->SetActive(false);
			(*p)->SetSequence((*p)->FRAME_DIE, 70);
			(*p)->SetDead(true);
			ScoreCount = ScoreCount + 10;
			npcdiedmusic_buffer.Play(false);
		}
	}
}

//游戏初始化
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
	LastMenuInit();
	//MainGameInit();

	if (!ds.CreateDS(m_hWnd)) return;

	openmusic_buffer.LoadWave(ds, L".\\res\\sound\\GameStart.wav");
	startmusic_buffer.LoadWave(ds, L".\\res\\sound\\GameOpen.wav");
	mapmusic01_buffer.LoadWave(ds, L".\\res\\sound\\Map01.wav");
	mapmusic02_buffer.LoadWave(ds, L".\\res\\sound\\Map02.wav");
	mapmusic03_buffer.LoadWave(ds, L".\\res\\sound\\Map03.wav");
	gameovermusic_buffer.LoadWave(ds, L".\\res\\sound\\gameover.wav");
	victorymusic_buffer.LoadWave(ds, L".\\res\\sound\\victory.wav");
	shootmusic_buffer.LoadWave(ds, L".\\res\\sound\\shoot.wav");
	attackmusic_buffer.LoadWave(ds, L".\\res\\sound\\attack_s.wav");
	npcdiedmusic_buffer.LoadWave(ds, L".\\res\\sound\\npc_died.wav");
	maleplayerdiedmusic_buffer.LoadWave(ds, L".\\res\\sound\\male_player_died.wav");
	femaleplayerdiedmusic_buffer.LoadWave(ds, L".\\res\\sound\\female_player_died.wav");
	
	mousedown_buffer.LoadWave(ds, L".\\res\\sound\\mousedown.wav");
	mousemove_buffer.LoadWave(ds, L".\\res\\sound\\mouseover.wav");

	openmusic_buffer.Play(true);
}

//游戏逻辑
void CrazyFighting::GameLogic()
{
	if (GameState == GAME_RUN)
	{
		GameKeyAction();		//处理快速游戏按键
		if (player->IsActive())	//移动玩家角色
		{
			UpdatePlayerPos();//移动玩家角色
		}
		if (player->IsDead() == false) 
		{
			// 记录玩家角色生成后的时间
			player->SetEndTime(GetTickCount64());
			// 判断玩家角色无敌时间是否结束
			if (player->GetEndTime() - player->GetStartTime() >= SUPER_TIME)
			{
				player->SetStartTime(player->GetEndTime());
				// 无敌标识复位
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
		UpdateAnimation();//更新动画的
	}
}

//游戏结束
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
	playerShow1.Destroy();
	playerShow2.Destroy();
	playerShow3.Destroy();
	mapShow1.Destroy();
	mapShow2.Destroy();
	mapShow3.Destroy();
	gameoverimg.Destroy();
	gamewinimg.Destroy();
	xzh.Destroy();
	cyt.Destroy();
	jsy.Destroy();
}

//游戏绘制
void CrazyFighting::GamePaint(HDC hdc)
{
	if (GameState == GAME_START)		//显示开始菜单
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
	else if (GameState == GAME_ABOUT)	//显示游戏关于
	{
		PaintAboutMenu(hdc);
	}
	else if (GameState == GAME_HELP)	//显示游戏帮助
	{
		PaintHelpMenu(hdc);
	}
	else if (GameState == GAME_RUN)		//开始游戏
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
		PaintLastMenu(hdc);
	}
	else if (GameState == GAME_WIN)
	{
		PaintLastMenu(hdc);
	}
}

//游戏键盘操作
void CrazyFighting::GameKeyAction(int Action)
{
	if (Action == KEY_SYS_NONE)//根据keys数组状态处理按键行为
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
					shootmusic_buffer.Play(false);
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
					attackmusic_buffer.Play(false);
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

//游戏鼠标操作
void CrazyFighting::GameMouseAction(int x, int y, int Action)
{
	GetCursorPos(&mouse_pt);
	ScreenToClient(m_hWnd, &mouse_pt);

	if (Action == MOUSE_MOVE)//当鼠标选中按钮时，按钮改变状态换图片颜色
	{
		if (GameState == GAME_START)
		{
			if (chooseState == 0)
			{
				startmenu.MenuMouseMove(x, y);
			}
			else if (chooseState == 1)
			{
				playermenu.MenuMouseMove(x, y);
				int index = playermenu.GetMenuIndex();
				if (index >= 0)
				{
					switch (index)
					{
					case 0:
						playerType = 0;
						break;
					case 1:
						playerType = 1;
						break;
					case 2:
						playerType = 2;
						break;
					case 3:
						break;
					}
				}
			}
			else if (chooseState == 2)
			{
				mapmenu.MenuMouseMove(x, y);
				int index = mapmenu.GetMenuIndex();
				if (index >= 0)
				{
					switch (index)
					{
					case 0:
						mapType = 0;
						break;
					case 1:
						mapType = 1;
						break;
					case 2:
						mapType = 2;
						break;
					case 3:
						break;
					}
				}
			}
		}
		if (GameState == GAME_ABOUT) aboutmenu.MenuMouseMove(x, y);
		if (GameState == GAME_HELP) helpmenu.MenuMouseMove(x, y);
		
		if (GameState == GAME_OVER || GameState == GAME_WIN) lastmenu.MenuMouseMove(x, y);
		if (GameState == GAME_RUN) runmenu.MenuMouseMove(x, y);
		
	}
	if (Action == MOUSE_LCLICK)
	{
		if (GameState == GAME_START) //开始菜单
		{
			if (chooseState == 0)
			{
				int index = startmenu.MenuMouseClick(x, y);
				if (index >= 0)
				{
					switch (index)
					{
					case 0://游戏运行
						chooseState = 1;
						openmusic_buffer.Stop();
						startmusic_buffer.Play(true);
						break;
					case 1://关于
						GameState = GAME_ABOUT;
						break;
					case 2://帮助
						GameState = GAME_HELP;
						break;
					case 3://退出游戏
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
					case 0:// 女射手

						chooseState = 2;
						playerType = 0;
						break;
					case 1:// 狂战士
						chooseState = 2;
						playerType = 1;
						break;
					case 2:// 元素使
						chooseState = 2;
						playerType = 2;
						break;
					case 3:// 返回
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
					case 0:// 田园
						chooseState = 0;
						GameState = GAME_RUN;
						mapType = 0;
						MainGameInit();//重新初始化新游戏
						startmusic_buffer.Stop();
						mapmusic01_buffer.Play(true);
						break;
					case 1:// 矿洞
						chooseState = 0;
						GameState = GAME_RUN;
						mapType = 1;
						MainGameInit();//重新初始化新游戏
						startmusic_buffer.Stop();
						mapmusic02_buffer.Play(true);
						break;
					case 2:// 地狱
						chooseState = 0;
						GameState = GAME_RUN;
						mapType = 2;
						MainGameInit();//重新初始化新游戏
						startmusic_buffer.Stop();
						mapmusic03_buffer.Play(true);
						break;
					case 3:// 返回
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
				case 0://返回开始菜单
					GameState = GAME_START;
					break;

				case 1://退出游戏
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
				case 0://添加新游戏代码
					GameState = GAME_START;
					break;

				case 1://添加退出游戏代码
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		if (GameState == GAME_RUN)
		{
			int index = runmenu.MenuMouseClick(x, y);
			if (runmenu.inrange && index >= 0)//若显示了运行菜单，则点击有效
			{
				switch (index)
				{
				case 0://返回开始菜单
					GameEnd();
					GameInit();
					GameState = GAME_START;
					if (mapType == 0) mapmusic01_buffer.Stop();
					else if (mapType == 1) mapmusic02_buffer.Stop();
					else if (mapType == 2) mapmusic03_buffer.Stop();
					openmusic_buffer.Play(true);
					break;
				case 1://重新开始
					MainGameInit();//重新初始化新游戏
					break;
				case 2://退出游戏
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
			if (x > wnd_width - 181 && y < 9)//在区域内，inrange置为真，反之，则否
			{
				runmenu.inrange = true;
			}
			else
			{
				runmenu.inrange = false;
			}
		}
		if (GameState == GAME_OVER)
		{
			int index = lastmenu.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index)
				{
				case 0://返回开始菜单
					GameEnd();
					GameInit();
					GameState = GAME_START;
					openmusic_buffer.Play(true);
					break;
				case 1://退出游戏
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		if (GameState == GAME_WIN)
		{
			int index = lastmenu.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index)
				{
				case 0://返回开始菜单
					GameEnd();
					GameInit();
					GameState = GAME_START;
					openmusic_buffer.Play(true);
					break;
				case 1://退出游戏
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
	}
}

//开始菜单初始化
void CrazyFighting::StartMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"开始",L"关于",L"帮助",L"退出" };		//按钮文字
	startmenu.SetMenuBkg(L".\\res\\game\\start_back.jpg");			//设置菜单背景图片
	//长条形按钮菜单项
	btn_width = 218;										//按钮宽
	btn_height = 73;										//按钮高
	normalClr = Color::Brown;								//按钮正常状态文字颜色
	focusClr = Color::White;									//按钮选中状态文字颜色
	startmenu.SetBtnBmp(L".\\res\\game\\main_button.png", btn_width, btn_height);//设置按钮图片
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //对齐方式居中
	menuInfo.space = MENU_SPACE;		 //菜单项之间的间隔距离
	menuInfo.width = btn_width;			 //菜单项宽
	menuInfo.height = btn_height;		 //	菜单项高
	menuInfo.fontName = L"宋体";		 //菜单项字体
	menuInfo.isBold = true;				 //是否粗体
	menuInfo.normalTextColor = normalClr;//正常状态文字颜色
	menuInfo.focusTextColor = focusClr;	 //选中状态文字颜色
	startmenu.SetMenuInfo(menuInfo);

	MENUITEM mItem1, mItem2, mItem3, mItem4;
	x = (wndWidth - btn_width + 600) / 2 - 20;			//开始按钮
	y = wndHeight / 2 - btn_height * 1.5;
	mItem1.pos.x = x;
	mItem1.pos.y = y;
	mItem1.ItemName = menuItems[0];
	startmenu.AddMenuItem(mItem1);

	x = (wndWidth - btn_width + 600) / 2 + 20;		//关于按钮
	y = wndHeight / 2;
	mItem2.pos.x = x;
	mItem2.pos.y = y;
	mItem2.ItemName = menuItems[1];
	startmenu.AddMenuItem(mItem2);

	x = (wndWidth - btn_width + 600) / 2 - 20;			//帮助按钮
	y = wndHeight / 2 + btn_height * 1.5;
	mItem3.pos.x = x;
	mItem3.pos.y = y;
	mItem3.ItemName = menuItems[2];
	startmenu.AddMenuItem(mItem3);

	x = (wndWidth - btn_width + 600) / 2 + 20;		//退出按钮
	y = wndHeight / 2 + btn_height * 3;
	mItem4.pos.x = x;
	mItem4.pos.y = y;
	mItem4.ItemName = menuItems[3];
	startmenu.AddMenuItem(mItem4);

	startmenu.SetClickSound(&mousedown_buffer);
	startmenu.SetMoveSound(&mousemove_buffer);
	GameState = GAME_START;
}

//关于菜单初始化
void CrazyFighting::AboutMenuInit()
{
	xzh.LoadImageFile(L".\\res\\about\\xzh.jpg");
	cyt.LoadImageFile(L".\\res\\about\\cyt.jpg");
	jsy.LoadImageFile(L".\\res\\about\\jsy.jpg");
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"返回",L"退出" };
	aboutmenu.SetMenuBkg(L".\\res\\game\\about_back.jpg");//设置背景图片
	//长条形按钮菜单项
	btn_width = 218;			//按钮宽度
	btn_height = 74;			//按钮高度
	normalClr = Color::White;	//按钮正常时
	focusClr = Color::Brown;	//鼠标在按钮上时
	aboutmenu.SetBtnBmp(L".\\res\\game\\other_button.png", btn_width, btn_height);//菜单按钮图片
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //对齐方式居中
	menuInfo.space = MENU_SPACE;		 //菜单项之间的间隔距离
	menuInfo.width = btn_width;			 //菜单项宽
	menuInfo.height = btn_height;		 //	菜单项高
	menuInfo.fontName = L"宋体";		 //菜单项字体
	menuInfo.isBold = true;				 //是否粗体
	menuInfo.normalTextColor = normalClr;//正常状态文字颜色
	menuInfo.focusTextColor = focusClr;	 //选中状态文字颜色
	aboutmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 2; i++) {
		//设置菜单各项按钮位置
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 2 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 2 * btn_height;
		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		aboutmenu.AddMenuItem(mItem);//将按钮添加进菜单
	}
	aboutmenu.SetClickSound(&mousedown_buffer);
	aboutmenu.SetMoveSound(&mousemove_buffer);
}

//帮助菜单初始化
void CrazyFighting::HelpMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"返回",L"退出游戏" };
	helpmenu.SetMenuBkg(L".\\res\\game\\help_back.jpg");//设置背景图片
	//长条形按钮菜单项
	btn_width = 218;
	btn_height = 74;
	normalClr = Color::White;	//按钮正常时
	focusClr = Color::Brown;	//鼠标在按钮上时
	helpmenu.SetBtnBmp(L".\\res\\game\\other_button.png", btn_width, btn_height);
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //对齐方式居中
	menuInfo.space = MENU_SPACE;		 //菜单项之间的间隔距离
	menuInfo.width = btn_width;			 //菜单项宽
	menuInfo.height = btn_height;		 //	菜单项高
	menuInfo.fontName = L"宋体";		 //菜单项字体
	menuInfo.isBold = true;				 //是否粗体
	menuInfo.normalTextColor = normalClr;//正常状态文字颜色
	menuInfo.focusTextColor = focusClr;	 //选中状态文字颜色
	helpmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 2; i++) {
		//设置菜单各项按钮位置
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 2 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 2 * btn_height;
		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		helpmenu.AddMenuItem(mItem);//将按钮添加进菜单
	}
	helpmenu.SetClickSound(&mousedown_buffer);
	helpmenu.SetMoveSound(&mousemove_buffer);
}

//游戏运行菜单初始化
void CrazyFighting::RunMenuInit()
{
	menuArea.LoadImageFile(L".\\res\\game\\menu_area.png");   //载入运行菜单区域图片
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"返回",L"重新开始",L"退出" };
	//	runmenu.SetMenuBkg(L".\\res\\bkg03.jpg");//设置背景图片
		//长条形按钮菜单项
	btn_width = 218;
	btn_height = 74;
	normalClr = Color::White;	//按钮正常时
	focusClr = Color::Brown;	//鼠标在按钮上时
	runmenu.SetBtnBmp(L".\\res\\game\\other_button.png", btn_width, btn_height);
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //对齐方式居中
	menuInfo.space = MENU_SPACE;		 //菜单项之间的间隔距离
	menuInfo.width = btn_width;			 //菜单项宽
	menuInfo.height = btn_height;		 //	菜单项高
	menuInfo.fontName = L"黑体";		 //菜单项字体
	menuInfo.isBold = true;				 //是否粗体
	menuInfo.normalTextColor = normalClr;//正常状态文字颜色
	menuInfo.focusTextColor = focusClr;	 //选中状态文字颜色
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

//游戏选人菜单初始化
void CrazyFighting::PlayerMenuInit()
{
	playerShow1.LoadImageFile(L".\\res\\sprite\\Shooter.png");
	playerShow2.LoadImageFile(L".\\res\\sprite\\Fighter.png");
	playerShow3.LoadImageFile(L".\\res\\sprite\\Elemental.png");
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"女射手",L"狂战士",L"元素使",L"返回" };
	//playermenu.SetMenuBkg(L".\\res\\map\\01\\Background1.png");//设置背景图片
	//长条形按钮菜单项
	btn_width = 218;
	btn_height = 74;
	normalClr = Color::White;	//按钮正常时
	focusClr = Color::Brown;	//鼠标在按钮上时
	playermenu.SetBtnBmp(L".\\res\\game\\other_button.png", btn_width, btn_height);
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //对齐方式居中
	menuInfo.space = MENU_SPACE;		 //菜单项之间的间隔距离
	menuInfo.width = btn_width;			 //菜单项宽
	menuInfo.height = btn_height;		 //	菜单项高
	menuInfo.fontName = L"黑体";		 //菜单项字体
	menuInfo.isBold = true;				 //是否粗体
	menuInfo.normalTextColor = normalClr;//正常状态文字颜色
	menuInfo.focusTextColor = focusClr;	 //选中状态文字颜色
	playermenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 3; i++) {
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 3 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 3 * btn_height;

		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		playermenu.AddMenuItem(mItem);
	}

	x = (wndWidth - btn_width) / 2 + 5;
	y = wnd_height - 2 * btn_height;
	MENUITEM mItem;
	mItem.pos.x = x;
	mItem.pos.y = y;
	mItem.ItemName = menuItems[3];
	playermenu.AddMenuItem(mItem);

	playermenu.SetClickSound(&mousedown_buffer);
	playermenu.SetMoveSound(&mousemove_buffer);
}

//游戏选地图菜单初始化
void CrazyFighting::MapMenuInit()
{
	mapShow1.LoadImageFile(L".\\res\\map\\01\\Background1.png");
	mapShow2.LoadImageFile(L".\\res\\map\\02\\level2_Background.png");
	mapShow3.LoadImageFile(L".\\res\\map\\03\\Background.png");
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"田园",L"矿洞",L"地狱",L"返回" };
	//mapmenu.SetMenuBkg(L".\\res\\map\\01\\Background1.png");//设置背景图片
	//长条形按钮菜单项
	btn_width = 218;
	btn_height = 74;
	normalClr = Color::White;	//按钮正常时
	focusClr = Color::Brown;	//鼠标在按钮上时
	mapmenu.SetBtnBmp(L".\\res\\game\\other_button.png", btn_width, btn_height);
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //对齐方式居中
	menuInfo.space = MENU_SPACE;		 //菜单项之间的间隔距离
	menuInfo.width = btn_width;			 //菜单项宽
	menuInfo.height = btn_height;		 //	菜单项高
	menuInfo.fontName = L"黑体";		 //菜单项字体
	menuInfo.isBold = true;				 //是否粗体
	menuInfo.normalTextColor = normalClr;//正常状态文字颜色
	menuInfo.focusTextColor = focusClr;	 //选中状态文字颜色
	mapmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 3; i++) {
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 3 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 3 * btn_height;

		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		mapmenu.AddMenuItem(mItem);
	}

	x = (wndWidth - btn_width) / 2 + 5;
	y = wnd_height - 2 * btn_height;
	MENUITEM mItem;
	mItem.pos.x = x;
	mItem.pos.y = y;
	mItem.ItemName = menuItems[3];
	mapmenu.AddMenuItem(mItem);

	mapmenu.SetClickSound(&mousedown_buffer);
	mapmenu.SetMoveSound(&mousemove_buffer);
}

void CrazyFighting::LastMenuInit()
{
	gameoverimg.LoadImageFile(L".\\res\\game\\lose.png");
	gamewinimg.LoadImageFile(L".\\res\\game\\win.png");
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"返回",L"退出游戏" };
	//长条形按钮菜单项
	btn_width = 218;
	btn_height = 74;
	normalClr = Color::White;	//按钮正常时
	focusClr = Color::Brown;	//鼠标在按钮上时
	lastmenu.SetBtnBmp(L".\\res\\game\\other_button.png", btn_width, btn_height);
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;					 //对齐方式居中
	menuInfo.space = MENU_SPACE;		 //菜单项之间的间隔距离
	menuInfo.width = btn_width;			 //菜单项宽
	menuInfo.height = btn_height;		 //	菜单项高
	menuInfo.fontName = L"黑体";		 //菜单项字体
	menuInfo.isBold = true;				 //是否粗体
	menuInfo.normalTextColor = normalClr;//正常状态文字颜色
	menuInfo.focusTextColor = focusClr;	 //选中状态文字颜色
	lastmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 2; i++) {
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 2 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 2 * btn_height;

		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		lastmenu.AddMenuItem(mItem);
	}
	lastmenu.SetClickSound(&mousedown_buffer);
	lastmenu.SetMoveSound(&mousemove_buffer);
}

//开始菜单绘制
void CrazyFighting::PaintStartMenu(HDC hdc)
{
	//T_Graph::PaintBlank(hdc, 0, 0, (float)wnd_width, (float)wnd_height, Color::Black, 140);
	startmenu.DrawMenu(hdc);
	RectF textRec;
	textRec.X = -302;
	textRec.Y = 52;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 4;
	T_Graph::PaintText(hdc, textRec, L"疯狂大乱斗", 50, L"宋体", Color::Black, FontStyleBold, StringAlignmentCenter);
	textRec.X = -300;
	textRec.Y = 50;
	T_Graph::PaintText(hdc, textRec, L"疯狂大乱斗", 50, L"宋体", Color::Red, FontStyleBold, StringAlignmentCenter);
	textRec.X = -302;
	textRec.Y = 102;
	T_Graph::PaintText(hdc, textRec, L"CRAZY FIGHTING", 30, L"黑体", Color::Black, FontStyleBold, StringAlignmentCenter);
	textRec.X = -300;
	textRec.Y = 100;
	T_Graph::PaintText(hdc, textRec, L"CRAZY FIGHTING", 30, L"黑体", Color::Red, FontStyleBold, StringAlignmentCenter);
	textRec.X = 50;
	textRec.Y = 170;
	T_Graph::PaintText(hdc, textRec, L"版本: 1.0.0", 16, L"黑体", Color::Red, FontStyleBold, StringAlignmentNear);
	textRec.X = 50;
	textRec.Y = 200;
	T_Graph::PaintText(hdc, textRec, L"(C) 2022 xzh cyt jsy", 16, L"宋体", Color::Red, FontStyleBold, StringAlignmentNear);
}

//关于菜单绘制
void CrazyFighting::PaintAboutMenu(HDC hdc)
{
	//绘制关于菜单
	aboutmenu.DrawMenu(hdc);

	//绘制半透明黑色背景
	T_Graph::PaintBlank(hdc, (float)wnd_width / 10, (float)wnd_height * 1 / 10, (float)wnd_width * 4 / 5, (float)wnd_height * 5 / 8, Color::Black, 140);

	RectF textRec;
	textRec.X = 0.00;
	textRec.Y = -10.00;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 3;
	T_Graph::PaintText(hdc, textRec, L"关于", 25, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);

	textRec.X = wnd_width / 2 - 250;
	textRec.Y = (float)wnd_height * 1 / 30 + 15;
	T_Graph::PaintText(hdc, textRec, L"疯狂大乱斗", 16, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
	textRec.X = wnd_width / 2 + 100;
	textRec.Y = (float)wnd_height * 1 / 30 + 15;
	T_Graph::PaintText(hdc, textRec, L"版本: 1.0.0", 14, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
	textRec.X = wnd_width / 2 - 250;
	textRec.Y = (float)wnd_height * 2 / 30 + 15;
	T_Graph::PaintText(hdc, textRec, L"Copyright 2022 xzh cyt jsy", 14, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 30 + 15;
	T_Graph::PaintText(hdc, textRec, L"All Rights Reserved", 14, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
	textRec.X = wnd_width / 2 + 100;
	textRec.Y = (float)wnd_height * 3 / 30 + 15;
	T_Graph::PaintText(hdc, textRec, L"保留所有权利", 14, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);


	textRec.X = wnd_width / 2 - 300;
	textRec.Y = (float)wnd_height * 2 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"姓名：程元通", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"学号：8002119224", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 4 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"班级：软工1907班", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 5 / 30 + 230;
	T_Graph::PaintText(hdc, textRec, L"分工：田园地图设计，\n角色设计，\n游戏功能实现，\n菜单设计。", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);

	textRec.X = wnd_width / 2 - 50;
	textRec.Y = (float)wnd_height * 2 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"姓名：夏正豪", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"学号：8002119225", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 4 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"班级：软工1907班", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 5 / 30 + 230;
	T_Graph::PaintText(hdc, textRec, L"分工：矿洞地图设计，\n角色设计，\n游戏功能实现，\n菜单设计。", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	
	textRec.X = wnd_width / 2 + 200;
	textRec.Y = (float)wnd_height * 2 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"姓名：姜恕宇", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"学号：8002119227", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 4 / 30 + 200;
	T_Graph::PaintText(hdc, textRec, L"班级：软工1907班", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 5 / 30 + 230;
	T_Graph::PaintText(hdc, textRec, L"分工：地狱地图设计，\n角色设计，\n游戏功能实现，\n菜单设计。", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);

	cyt.PaintImage(hdc, (wnd_width - 90) / 2 - 250, 240, 90, 120);
	xzh.PaintImage(hdc, (wnd_width - 90) / 2, 240, 90, 120);
	jsy.PaintImage(hdc, (wnd_width - 90) / 2 + 250, 240, 90, 120);
}

//帮助菜单绘制
void CrazyFighting::PaintHelpMenu(HDC hdc)
{
	helpmenu.DrawMenu(hdc);
	//绘制半透明黑色背景
	T_Graph::PaintBlank(hdc, (float)wnd_width / 10, (float)wnd_height * 1 / 10, (float)wnd_width * 4 / 5, (float)wnd_height * 5 / 8, Color::Black, 140);

	RectF textRec;
	textRec.X = 0.00;
	textRec.Y = 30.00;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 3;
	T_Graph::PaintText(hdc, textRec, L"帮助", 36, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);

	textRec.X = wnd_width / 2 - 350;
	textRec.Y = (float)wnd_height / 20 + 80;
	T_Graph::PaintText(hdc, textRec, L"按左、右方向键移动玩家角色，按上方向键进行玩家跳跃。", 20, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 2 / 20 + 80;
	T_Graph::PaintText(hdc, textRec, L"移动过程中按shift键可以加速。", 20, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 20 + 80;
	T_Graph::PaintText(hdc, textRec, L"按空格键普通攻击，按C键远程攻击。", 20, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 4 / 20 + 80;
	T_Graph::PaintText(hdc, textRec, L"如果玩家被哥布林和幽灵碰到或者掉入陷阱，玩家生命值减1。", 20, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 5 / 20 + 80;
	T_Graph::PaintText(hdc, textRec, L"当玩家生命值归0时，游戏结束。", 20, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 6 / 20 + 80;
	T_Graph::PaintText(hdc, textRec, L"玩家的普通攻击和远程攻击都可以消灭哥布林和幽灵。", 20, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 7 / 20 + 80;
	T_Graph::PaintText(hdc, textRec, L"当消灭完全部的哥布林和幽灵后，挑战成功。", 20, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
}

// 选人菜单绘制
void CrazyFighting::PaintPlayerMenu(HDC hdc)
{
	if (mapType == 0)
	{
		mapShow1.PaintImage(hdc, 0, 0, 1024, 768);

	}
	else if (mapType == 1)
	{
		mapShow2.PaintImage(hdc, 0, 0, 1024, 768);
	}
	else if (mapType == 2)
	{
		mapShow3.PaintImage(hdc, 0, 0, 1024, 768);
	}

	if (playerType == 0)
	{
		playerShow1.PaintFrame(playerShow1.GetBmpHandle(), hdc, (wnd_width - 321 * 1.5) / 2, (wnd_height - 500 * 1.5) / 2 + 60, 0, 18, 321, 284, 1.5);

	}
	else if (playerType == 1)
	{
		playerShow2.PaintFrame(playerShow2.GetBmpHandle(), hdc, (wnd_width - 321 * 1.5) / 2, (wnd_height - 500 * 1.5) / 2 + 60, 0, 18, 321, 284, 1.5);
	}
	else if (playerType == 2)
	{
		playerShow3.PaintFrame(playerShow3.GetBmpHandle(), hdc, (wnd_width - 321 * 1.5) / 2, (wnd_height - 500 * 1.5) / 2 + 60, 0, 18, 321, 284, 1.5);
	}

	RectF textRec;
	textRec.X = 0;
	textRec.Y = 32;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 4;
	T_Graph::PaintText(hdc, textRec, L"选择人物", 40, L"宋体", Color::Black, FontStyleBold, StringAlignmentCenter);
	textRec.X = 0;
	textRec.Y = 30;
	T_Graph::PaintText(hdc, textRec, L"选择人物", 40, L"宋体", Color::Brown, FontStyleBold, StringAlignmentCenter);

	playermenu.DrawMenu(hdc);
}

// 选地图菜单绘制
void CrazyFighting::PaintMapMenu(HDC hdc)
{
	if (mapType == 0)
	{
		mapShow1.PaintImage(hdc, 0, 0, 1024, 768);

	}
	else if (mapType == 1)
	{
		mapShow2.PaintImage(hdc, 0, 0, 1024, 768);
	}
	else if (mapType == 2)
	{
		mapShow3.PaintImage(hdc, 0, 0, 1024, 768);
	}

	if (playerType == 0)
	{
		playerShow1.PaintFrame(playerShow1.GetBmpHandle(), hdc, (wnd_width - 321 * 1.5) / 2, (wnd_height - 500 * 1.5) / 2 + 60, 0, 18, 321, 284, 1.5);

	}
	else if (playerType == 1)
	{
		playerShow2.PaintFrame(playerShow2.GetBmpHandle(), hdc, (wnd_width - 321 * 1.5) / 2, (wnd_height - 500 * 1.5) / 2 + 60, 0, 18, 321, 284, 1.5);
	}
	else if (playerType == 2)
	{
		playerShow3.PaintFrame(playerShow3.GetBmpHandle(), hdc, (wnd_width - 321 * 1.5) / 2, (wnd_height - 500 * 1.5) / 2 + 60, 0, 18, 321, 284, 1.5);
	}

	RectF textRec;
	textRec.X = 0;
	textRec.Y = 32;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 4;
	T_Graph::PaintText(hdc, textRec, L"选择地图", 40, L"宋体", Color::Black, FontStyleBold, StringAlignmentCenter);
	textRec.X = 0;
	textRec.Y = 30;
	T_Graph::PaintText(hdc, textRec, L"选择地图", 40, L"宋体", Color::Brown, FontStyleBold, StringAlignmentCenter);


	mapmenu.DrawMenu(hdc);
}

void CrazyFighting::PaintLastMenu(HDC hdc)
{
	if (GameState == GAME_OVER)
	{
		PaintMainGame(hdc);
		RectF textRec;
		textRec.X = 0.00;
		textRec.Y = 100.00;
		textRec.Width = (float)wnd_width;
		textRec.Height = (float)wnd_height;
		T_Graph::PaintBlank(
			hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT,
			Color(150, 0, 0, 0));
		T_Graph::PaintText(hdc, textRec, L"你死了", 60, L"宋体", Color::Red, FontStyleBold, StringAlignmentCenter);
		gameoverimg.PaintFrame(gameoverimg.GetBmpHandle(), hdc, (wnd_width - 128 * 2) / 2, (wnd_height - 200 * 2) / 2, 0, 1, 128, 128, 2);
	}
	else if (GameState == GAME_WIN)
	{
		PaintMainGame(hdc);
		RectF textRec;
		textRec.X = 0.00;
		textRec.Y = 100.00;
		textRec.Width = (float)wnd_width;
		textRec.Height = (float)wnd_height;
		T_Graph::PaintBlank(
			hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT,
			Color(150, 255, 255, 255));
		T_Graph::PaintText(hdc, textRec, L"挑战成功", 60, L"宋体", Color::Red, FontStyleBold, StringAlignmentCenter);
		gamewinimg.PaintFrame(gamewinimg.GetBmpHandle(), hdc, (wnd_width - 128 * 2) / 2, (wnd_height - 200 * 2) / 2, 0, 1, 128, 128, 2);
	}
	lastmenu.DrawMenu(hdc);
}

// 加载游戏图片资源
void CrazyFighting::LoadImageRes()
{
	if (lifeImg == NULL) lifeImg = new T_Graph(L".\\res\\game\\life.png");
	if (levelImg == NULL) levelImg = new T_Graph(L".\\res\\game\\Level.png");
	if (totalscoreBmp == NULL) totalscoreBmp = new T_Graph(L".\\res\\game\\totalscore.png");
}

// 清除关卡数据
void CrazyFighting::ClearGameLevel()
{
	npc_set.clear();		npc_set.swap(vector<CrazyMan*>());
	arrow_set.clear();		arrow_set.swap(vector<T_Sprite*>());
	trap_set.clear();		trap_set.swap(vector<T_Sprite*>());

	t_scene.getSceneLayers()->clear();
	t_scene.RemoveAll();
}

//运行游戏初始化
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

//游戏运行绘制
void CrazyFighting::PaintMainGame(HDC hdc)
{	
	//back.PaintBlank(hdc, 0, 0, wnd_width, wnd_height, Color(3, 127, 237));
	t_scene.Draw(hdc, 0, 0);	
	menuArea.PaintImage(hdc, wnd_width - 181, 0, 181, 9, 255);//绘制运行菜单点击区域
	runmenu.DrawMenu(hdc, 0, 0, 255, GAME_RUN);
}

// 游戏状态显示
void CrazyFighting::DisplayInfo(HDC hdc, int game_state)
{
	int FontHeight = 0;//字号
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
	wstring LeveNum = L"当前关: ";
	LeveNum.append(T_Util::int_to_wstring(mapType + 1));
	rect.X = (REAL)(x + iconW);
	T_Graph::PaintText(hdc, rect, LeveNum.c_str(), (REAL)FontHeight, L"宋体",
		Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);

	x = x + strLen;
	lifeImg->PaintImage(
		hdc, x, y - 4, lifeImg->GetImageWidth(),
		lifeImg->GetImageHeight(), 200
	);
	wstring lifeleft = L"生命值: ";
	lifeleft.append(T_Util::int_to_wstring(LifeCount));
	rect.X = (REAL)(x + iconW);
	T_Graph::PaintText(hdc, rect, lifeleft.c_str(), (REAL)FontHeight, L"宋体",
		Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);

	x = x + strLen;
	totalscoreBmp->PaintImage(
		hdc, x, y - 4, totalscoreBmp->GetImageWidth(),
		totalscoreBmp->GetImageHeight(), 200
	);
	wstring ScoreNum = L"总得分: ";
	ScoreNum.append(T_Util::int_to_wstring(ScoreCount));
	rect.X = (REAL)(x + iconW);
	T_Graph::PaintText(hdc, rect, ScoreNum.c_str(), (REAL)FontHeight, L"宋体",
		Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);
}