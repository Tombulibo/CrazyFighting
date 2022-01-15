#include "CrazyFighting.h"

//重新定义帧序列
int CrazyFighting::FRAME_LEFT[20] = { 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2 };
int CrazyFighting::FRAME_RIGHT[20] = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5 };
int CrazyFighting::FRAME_UP[20] = { 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8 };
int CrazyFighting::FRAME_DOWN[20] = { 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11 };

//加载地图
void CrazyFighting::LoadMap()
{ 

	if (mapType == 0)
	{
		t_scene.LoadTxtMap("map\\01\\background.txt");
		t_scene.LoadTxtMap("map\\01\\obstacle.txt");
		t_scene.LoadTxtMap("map\\01\\mask.txt");
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
		//t_scene.LoadTxtMap("map\\03\\mask.txt");
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
			//player->SetSequence(FRAME_UP, 20);
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
	}
}

//更新游戏动画
void CrazyFighting::UpdateAnimation()
{
	vCrazyManSet::iterator it;
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
				// 删除场景中已经失效的爆炸对象	
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
//void CrazyFighting::LoadNpc(int total)
//{
//}
//
//NPC更新处理
//void CrazyFighting::UpdateNpcPos()
//{
//}
//
//NPC加载初始化
void CrazyFighting::LoadNpc(int total)
{
	SPRITEINFO spInfo;
	for (int i = 0; i < total; i++) {
		spInfo.Active = true;
		spInfo.Dead = false;
		spInfo.Rotation = TRANS_NONE;
		spInfo.Ratio = 0.3f;
		spInfo.Level = 0;
		spInfo.Score = 0;
		spInfo.Speed = 8;
		spInfo.Alpha = 255;
		spInfo.Visible = true;
		int sp_width = 40;
		int sp_height = 40;
		int d = rand() % 2;
		switch (d) {
		case 0:
			spInfo.Dir = DIR_RIGHT;
			spInfo.X = t_scene.getSceneY() + scn_height - 5.5 * sp_height;
			spInfo.Y = t_scene.getSceneY() + scn_height - 5 * sp_height;
			break;
		case 1:
			spInfo.Dir = DIR_LEFT;
			spInfo.X = t_scene.getSceneY() + scn_height - 5.5 * sp_height;
			spInfo.Y = t_scene.getSceneY() + scn_height - 5 * sp_height;
			break;
		}
		npc_set.push_back(new CrazyMan(L".\\res\\sprite\\NPC1.png", 321, 284));
		CrazyMan* sp = npc_set.back();
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

//NPC更新处理
void CrazyFighting::UpdateNpcPos()
{
	if (npc_set.size() == 0) {
		return;
	}
	T_AI* spAi = new T_AI(4);
	vCrazyManSet::iterator it;
	for (it = npc_set.begin(); it != npc_set.end(); it++) {
		if ((*it)->IsActive() == true && (*it)->IsVisible() == true) {
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
			spAi->Wander((*it), t_scene.getBarrier());
		}
	}
	delete spAi;
}

// 加载炮弹
void CrazyFighting::Archery(int iTime) {
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
		//if (bombSet == player_bomb_set)
		//{
		gameLayer.type_id = LAYER_PLY_BOMB;
		gameLayer.layer->SetLayerTypeID(LAYER_PLY_BOMB);
		//}
		/*if (bombSet == npc_bomb_set)
		{
			gameLayer.type_id = LAYER_NPC_BOMB;
			gameLayer.layer->SetLayerTypeID(LAYER_NPC_BOMB);
		}*/
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
			//if (GameState != GAME_RUN) return;
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
			//LoadExplosion(x, y);

			//explosionSound.Restore();
			//explosionSound.Play(false);
		}

		// 如果NPC炮弹碰到了玩家
		/*if (bomb->GetLayerTypeID() == LAYER_NPC_BOMB &&
			bomb->CollideWith(player) &&
			player->IsDead() == false && player->IsVisible() == true)
		{

			bomb->SetVisible(false);
			bomb->SetActive(false);

			int x = bomb->GetX() - player->GetWidth() / 2;
			int y = bomb->GetY() - player->GetHeight() / 2;

			LoadExplosion(x, y);
			explosionSound.Play(false);

			if (isDelayCollision == false)
			{
				looseBlood = true;
				updateLifeCount = true;
			}
		}*/

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
						(*sp)->SetVisible(false);
						(*sp)->SetDead(true);
						arrow->SetActive(false);
						arrow->SetVisible(false);
						//NPC数目更新
						//NpcNumber = NpcNumber - 1;
						//LoadExplosion((*sp)->GetX(), (*sp)->GetY());
						//explosionSound.Play(false);
						//游戏过关升级处理
						/*if (NpcNumber == 0)
						{
							GameState = GAME_UPGRADE;
							upgradeGameLevel = true;
						}*/

						break;
					}

				}
			}
		}
	}
}
void CrazyFighting::Attack()
{
	vCrazyManSet::iterator p;
	for (p = npc_set.begin(); p != npc_set.end(); p++) // 处理与NPC碰撞
	{
		// 玩家触碰到NPC的处理
		if (player->CollideWith((*p)) && !(*p)->IsDead() && (*p)->IsVisible())
		{
			(*p)->SetVisible(false);
			(*p)->SetActive(false);
			(*p)->SetDead(true);
		}
	}
}

//游戏初始化
void CrazyFighting::GameInit()
{
	GameState = GAME_START;
	RunMenuInit();
	StartMenuInit();
	PlayerMenuInit();
	MapMenuInit();
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
		UpdateNpcPos();
		UpdateArrowPos();
		UpdateAnimation();//更新动画的
	}
}

//游戏结束
void CrazyFighting::GameEnd()
{
	t_scene.RemoveAll();
	runmenu.DestroyAll();
	startmenu.DestroyAll();
	aboutmenu.DestroyAll();
	menuArea.Destroy();
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
			if (CheckKey(VK_DOWN))
			{
				player->SetSequence(player->FRAME_ATTACK, 20);
				player->SetActive(true);
			}
			if (CheckKey(67))
			{
				//player->SetSequence(player->FRAME_SHOOT, 40);
				
				if (player->IsDead() == false && player->IsVisible() == true)
				{
					player->setShooting(true);
					//if (ChargeCount > 0)
					//{
					//LoadBomb(player, player_bomb_set, 400);
					Archery(player->getShootTime());
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
					Attack();
				}
				player->SetActive(true);
			}
			else player->setAttacking(false);
			
			if (CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT) == false && CheckKey(VK_UP) == false && CheckKey(VK_DOWN) == false && CheckKey(67) == false && CheckKey(VK_SPACE) == false)
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
						break;
					case 1:// 矿洞
						chooseState = 0;
						GameState = GAME_RUN;
						mapType = 1;
						MainGameInit();//重新初始化新游戏
						break;
					case 2:// 地狱
						chooseState = 0;
						GameState = GAME_RUN;
						mapType = 2;
						MainGameInit();//重新初始化新游戏
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
				case 1://开始游戏
					MainGameInit();//重新初始化新游戏
					GameState = GAME_RUN;
					break;
				case 2://退出游戏
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
				case 1://添加开始游戏代码
					MainGameInit();//重新初始化新游戏
					GameState = GAME_RUN;
					break;
				case 2://添加退出游戏代码
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
					GameState = GAME_START;
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
	}
}

//开始菜单初始化
void CrazyFighting::StartMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"开始",L"关于",L"帮助",L"退出" };		//按钮文字
	startmenu.SetMenuBkg(L".\\res\\beach.jpg");			//设置菜单背景图片
	//长条形按钮菜单项
	btn_width = 175;										//按钮宽
	btn_height = 60;										//按钮高
	normalClr = Color::White;								//按钮正常状态文字颜色
	focusClr = Color::Blue;									//按钮选中状态文字颜色
	startmenu.SetBtnBmp(L".\\res\\button_blue.png", btn_width, btn_height);//设置按钮图片
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
	startmenu.SetMenuInfo(menuInfo);

	MENUITEM mItem1, mItem2, mItem3, mItem4;
	x = wndWidth - btn_width * 1.5;			//开始按钮
	y = wndHeight / 2 - btn_height * 1.5;
	mItem1.pos.x = x;
	mItem1.pos.y = y;
	mItem1.ItemName = menuItems[0];
	startmenu.AddMenuItem(mItem1);

	x = wndWidth - btn_width * 1.5 - 40;		//关于按钮
	y = wndHeight / 2;
	mItem2.pos.x = x;
	mItem2.pos.y = y;
	mItem2.ItemName = menuItems[1];
	startmenu.AddMenuItem(mItem2);

	x = wndWidth - btn_width * 1.5;			//帮助按钮
	y = wndHeight / 2 + btn_height * 1.5;
	mItem3.pos.x = x;
	mItem3.pos.y = y;
	mItem3.ItemName = menuItems[2];
	startmenu.AddMenuItem(mItem3);

	x = wndWidth - btn_width * 1.5 - 40;		//退出按钮
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
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"返回",L"退出" };
	aboutmenu.SetMenuBkg(L".\\res\\menuback01.jpg");//设置背景图片
	//长条形按钮菜单项
	btn_width = 100;			//按钮宽度
	btn_height = 102;			//按钮高度
	normalClr = Color::LightGreen;	//按钮正常时，文字颜色为蓝色
	focusClr = Color::Blue;	//鼠标在按钮上时，文字颜色为白色
	aboutmenu.SetBtnBmp(L".\\res\\button_bubble.png", btn_width, btn_height);//菜单按钮图片
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
	aboutmenu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 2; i++) {
		//设置菜单各项按钮位置
		x = i * (btn_width + MENU_SPACE) + (wndWidth - 3 * btn_width - MENU_SPACE) / 2;
		y = wnd_height - 1.5 * btn_height;
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
	helpmenu.SetMenuBkg(L".\\res\\seabed.jpg");//设置背景图片
	//长条形按钮菜单项
	btn_width = 100;
	btn_height = 102;
	normalClr = Color::LightGreen;
	focusClr = Color::White;
	helpmenu.SetBtnBmp(L".\\res\\button_bubble.png", btn_width, btn_height);
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

//游戏运行菜单初始化
void CrazyFighting::RunMenuInit()
{
	menuArea.LoadImageFile(L".\\res\\menu_area.png");   //载入运行菜单区域图片
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"返回",L"重新开始",L"退出" };
	//	runmenu.SetMenuBkg(L".\\res\\bkg03.jpg");//设置背景图片
		//长条形按钮菜单项
	btn_width = 175;
	btn_height = 60;
	normalClr = Color::White;
	focusClr = Color::LightGreen;
	runmenu.SetBtnBmp(L".\\res\\button_blue.png", btn_width, btn_height);
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
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"女射手",L"狂战士",L"元素使",L"返回" };
	playermenu.SetMenuBkg(L".\\res\\seabed.jpg");//设置背景图片
	//长条形按钮菜单项
	btn_width = 100;
	btn_height = 102;
	normalClr = Color::LightGreen;
	focusClr = Color::White;
	playermenu.SetBtnBmp(L".\\res\\button_bubble.png", btn_width, btn_height);
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

//游戏选地图菜单初始化
void CrazyFighting::MapMenuInit()
{
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"田园",L"矿洞",L"地狱",L"返回" };
	mapmenu.SetMenuBkg(L".\\res\\seabed.jpg");//设置背景图片
	//长条形按钮菜单项
	btn_width = 100;
	btn_height = 102;
	normalClr = Color::LightGreen;
	focusClr = Color::White;
	mapmenu.SetBtnBmp(L".\\res\\button_bubble.png", btn_width, btn_height);
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

//开始菜单绘制
void CrazyFighting::PaintStartMenu(HDC hdc)
{
	startmenu.DrawMenu(hdc);
	RectF textRec;
	textRec.X = 30.00;
	textRec.Y = 30.00;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 4;
	T_Graph::PaintText(hdc, textRec, L"实验十四", 36, L"黑体", Color::Green, FontStyleBold, StringAlignmentNear);
	textRec.X = 29;
	textRec.Y = 29;
	T_Graph::PaintText(hdc, textRec, L"实验十四", 36, L"黑体", Color::Orange, FontStyleBold, StringAlignmentNear);
	textRec.X = 100;
	textRec.Y = wnd_height / 2 - (float)wnd_height * 3 / 20;
	T_Graph::PaintText(hdc, textRec, L"姓名：夏正豪", 14, L"黑体", Color::Black, FontStyleRegular, StringAlignmentNear);
	textRec.Y = wnd_height / 2 - (float)wnd_height * 2 / 20;
	T_Graph::PaintText(hdc, textRec, L"学号：8002119225", 14, L"黑体", Color::Black, FontStyleRegular, StringAlignmentNear);
	textRec.Y = wnd_height / 2 - (float)wnd_height / 20;
	T_Graph::PaintText(hdc, textRec, L"选课班：选课1班", 14, L"黑体", Color::Black, FontStyleRegular, StringAlignmentNear);
	textRec.Y = wnd_height / 2;
	T_Graph::PaintText(hdc, textRec, L"原班级：软件工程1907班", 14, L"黑体", Color::Black, FontStyleRegular, StringAlignmentNear);

}

//关于菜单绘制
void CrazyFighting::PaintAboutMenu(HDC hdc)
{
	//绘制关于菜单
	aboutmenu.DrawMenu(hdc);

	//绘制半透明黑色背景
	T_Graph::PaintBlank(hdc, (float)wnd_width / 8, (float)wnd_height * 1 / 16, (float)wnd_width * 3 / 4, (float)wnd_height * 4 / 7, Color::Black, 60);
	//绘制文字
	RectF textRec;
	textRec.X = 0.00;
	textRec.Y = 20.00;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 4;
	T_Graph::PaintText(hdc, textRec, L"关于", 24, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);

	textRec.X = wnd_width / 2 - 100;
	textRec.Y = (float)wnd_height / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"名称：实验十三", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 2 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"选课班：选课1班", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"原班级：软件工程1907班", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 4 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"学号：8002119225", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 5 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"姓名：夏正豪", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 6 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"时间：2021年12月23日", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);

}

//帮助菜单绘制
void CrazyFighting::PaintHelpMenu(HDC hdc)
{
	helpmenu.DrawMenu(hdc);
	//绘制半透明黑色背景
	T_Graph::PaintBlank(hdc, (float)wnd_width / 8, (float)wnd_height * 1 / 16, (float)wnd_width * 3 / 4, (float)wnd_height * 4 / 7, Color::Black, 60);

	RectF textRec;
	textRec.X = 0.00;
	textRec.Y = 0.00;
	textRec.Width = (float)wnd_width;
	textRec.Height = (float)wnd_height / 3;
	T_Graph::PaintText(hdc, textRec, L"帮助", 36, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);

	textRec.X = wnd_width / 2 - 250;
	textRec.Y = (float)wnd_height / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"默认键盘操作，按上下左右方向移动玩家角色。", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 2 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"点击游戏运行菜单，可选择切换操作方式为鼠标操作。", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 3 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"在鼠标操作过程中，若按下键盘，则自动切换为键盘操作。", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 4 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"可选择重新开始游戏。", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);
	textRec.Y = (float)wnd_height * 5 / 20 + 50;
	T_Graph::PaintText(hdc, textRec, L"请尽情在游戏地图中探索。", 14, L"黑体", Color::White, FontStyleRegular, StringAlignmentNear);

}

void CrazyFighting::PaintPlayerMenu(HDC hdc)
{
	playermenu.DrawMenu(hdc);
}

void CrazyFighting::PaintMapMenu(HDC hdc)
{
	mapmenu.DrawMenu(hdc);
}

//运行游戏初始化
void CrazyFighting::MainGameInit()
{
	t_scene.getSceneLayers()->clear();
	t_scene.RemoveAll();
	LoadMap();
	LoadPlayer();
	LoadNpc(NPC_NUM);

	player->SetActive(true);
	player->setJumping(true);
	player->setJumpSpeed(-20);
	player->setFallen(false);
}

//游戏运行绘制
void CrazyFighting::PaintMainGame(HDC hdc)
{	
	back.PaintBlank(hdc, 0, 0, wnd_width, wnd_height, Color(3, 127, 237));
	t_scene.Draw(hdc, 0, 0);	
	menuArea.PaintImage(hdc, wnd_width - 181, 0, 181, 9, 255);//绘制运行菜单点击区域
	runmenu.DrawMenu(hdc, 0, 0, 255, GAME_RUN);
}
