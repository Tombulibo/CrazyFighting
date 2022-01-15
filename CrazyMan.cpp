#include "CrazyMan.h"

int CrazyMan::SHOOTER_WALK[20] = { 0, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 15, 16, 17 };
int CrazyMan::SHOOTER_FALL[20] = { 18, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 23 };
int CrazyMan::SHOOTER_JUMP[20] = { 36, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 41 };
int CrazyMan::SHOOTER_SLIDE[20] = { 54, 54, 54, 54, 55, 55, 55, 56, 56, 56, 57, 57, 57, 58, 58, 58, 59, 59, 59, 59 };
int CrazyMan::SHOOTER_DIE[20];
int CrazyMan::SHOOTER_SHOOT[40] = { 90, 90, 90, 90, 91, 91, 92, 92, 92, 92, 93, 93, 94, 94, 94, 94, 95, 95, 96, 96, 96, 96, 97, 97, 98, 98, 98, 98, 99, 99, 99, 99, 100, 100, 100, 100, 101, 101, 101, 101 };
int CrazyMan::SHOOTER_KICK[20];
int CrazyMan::SHOOTER_FLY[20];

CrazyMan::CrazyMan(LPCTSTR imgPath, int frameWidth, int frameHeight, int winWidth, int winHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	win_width = winWidth;
	win_height = winHeight;

	jumpSpeed = -20;
	gravity = 1;
	jumping = false;
	falling = false;
	fallen = true;
	faceTo = DIR_RIGHT;
	shooting = false;
	shootTime = 600;
}

CrazyMan::~CrazyMan()
{
}
// 更新玩家位置
void CrazyMan::updatePostion(int xDir, int yDir, int jumpDir, T_Map* pBarrier)
{
	int nextStepX, nextStepY;
	int SpeedX = 0, SpeedY = 0;

	int movingSpeed = 0;
	if (jumpDir == DIR_DOWN || jumpDir == DIR_UP)// 上跳、下跳
	{
		if (jumpDir == DIR_UP)
		{
			movingSpeed = -jumpSpeed;
		}
		if (jumpDir == DIR_DOWN)
		{
			movingSpeed = jumpSpeed;
		}
	}
	else
	{
		movingSpeed = GetSpeed();
	}

	if (yDir == DIR_UP)
	{
		SpeedX = 0;
		SpeedY = -movingSpeed;
		//计算下一步移动是否超过边界
		nextStepY = GetY() + SpeedY;
		if (nextStepY <= 0)
		{
			SpeedY = 0 - GetY();
		}
	}

	if (yDir == DIR_DOWN)
	{
		SpeedX = 0;
		SpeedY = movingSpeed;
		//计算下一步移动是否超过边界
		nextStepY = GetY() + GetRatioSize().cy + SpeedY;
		if (nextStepY >= win_height)
		{
			SpeedY = win_height - GetRatioSize().cy - GetY();
		}
	}

	if (xDir == DIR_LEFT)
	{
		SpeedX = -movingSpeed;
		SpeedY = 0;
		//计算下一步移动是否超过边界
		nextStepX = GetX() + SpeedX;
		if (nextStepX <= 0)
		{
			SpeedX = 0 - GetX();
		}
		if (falling == false)
		{
			fallingDown();
		}
	}
	if (xDir == DIR_RIGHT)
	{
		SpeedX = movingSpeed;
		SpeedY = 0;
		//计算下一步移动是否超过边界
		nextStepX = GetX() + GetRatioSize().cx + SpeedX;
		if (nextStepX >= win_width)
		{
			SpeedX = win_width - GetRatioSize().cx - GetX();
		}
		if (falling == false)
		{
			fallingDown();
		}
	}

	int x = GetX();// 获得移动前的x位置
	int y = GetY();// 获得移动前的y位置

	if (!CollideWith(pBarrier))
	{
		Move(SpeedX, SpeedY);	// 继续移动
	}
	if (CollideWith(pBarrier))
	{
		SetPosition(x, y);		// 还原移动前的位置
		if (yDir == DIR_UP)
		{
			jumpSpeed = -jumpSpeed; //设置跳的速度
		}
		if (yDir == DIR_DOWN)
		{
			jumpSpeed = 0; //设置跳的速度
			jumping = false; //设置跳的状态
			fallen = true;
		}
	}
}
// 下落
void CrazyMan::fallingDown()
{
	if (jumping == false)
	{
		jumpSpeed = 0;
		jumping = true;
		falling = true;
	}
}
// 处理上跳及下落
void CrazyMan::jumpUpDown(T_Map* pBarrier)
{
	// 为速度加上重力影响
	jumpSpeed = jumpSpeed + gravity;
	if (jumpSpeed < 0)   // 往上跳
	{
		//SetDir(DIR_UP);
		// 由于只处理上下方向，所以xDir设为一个不存在的方向，如-1
		updatePostion(-1, DIR_UP, DIR_UP, pBarrier);
	}
	else if (jumpSpeed > 0)  // 往下自由落体
	{
		SetDir(DIR_DOWN);
		// 由于只处理上下方向，所以xDir设为一个不存在的方向，如-1
		updatePostion(-1, DIR_DOWN, DIR_DOWN, pBarrier);
	}

}

// 重写碰撞检测，适配角色素材
bool CrazyMan::CollideWith(IN T_Map* map)
{
	// 如果背景为图片则不检测地图碰撞
	if (map->getMapRows() == 0 && map->getMapCols() == 0)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// 如果地图不可见或角色不可见不检测地图碰撞
	if ((!(map->IsVisible())) || (!(this->IsVisible())))
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// 计算当前地图图层的矩形范围
	int mapLeft = map->GetX();
	int mapTop = map->GetY();
	int mapRight = mapLeft + map->GetWidth();
	int mapBottom = mapTop + map->GetHeight();

	// 获得地图图层中使用的图块的宽高
	int tW = map->getTileWidth();
	int tH = map->getTileHeight();

	// 计算当前角色的矩形范围
	int spLeft = this->GetCollideRect()->left + 30;
	int spTop = this->GetCollideRect()->top + 30;
	int spRight = this->GetCollideRect()->right - 30;
	int spBottom = this->GetCollideRect()->bottom;

	// 获得当前地图中图块的总列数
	int tNumCols = map->getMapCols();
	// 获得当前地图中图块的总行数
	int tNumRows = map->getMapRows();

	// 计算当前角色的实际的宽高
	int spW = spRight - spLeft;
	int spH = spBottom - spTop;

	RECT lprcDst;
	// 根据以上计算的图层的矩形范围和角色的矩形范围构造两个矩形对象
	RECT mapRect = { mapLeft, mapTop, mapRight, mapBottom };
	RECT spRect = { spLeft, spTop, spRight, spBottom };
	// 如果两个矩形对象没有发生任何碰撞，则退出函数
	if ((IntersectRect(&lprcDst, &mapRect, &spRect)) == FALSE)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// 如果两个矩形对象发生了碰撞，首先计算角色矩形上、下、左、右的矩形区域
	int startRow = (spTop <= mapTop) ? 0 : (spTop - mapTop) / tH;
	int endRow = (spBottom < mapBottom) ? (spBottom - 1 - mapTop) / tH : tNumRows - 1;
	int startCol = (spLeft <= mapLeft) ? 0 : (spLeft - mapLeft) / tW;
	int endCol = (spRight < mapRight) ? (spRight - 1 - mapLeft) / tW : tNumCols - 1;

	// 根据角色矩形上、下、左、右的矩形区域判断哪个矩形区域为障碍
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			// 如果当前矩形所在的图块在地图数据中为非0，就表示是障碍
			if (map->getTile(col, row) != 0)
			{
				mapBlockPT.x = col;	// 记录当前障碍图块的列
				mapBlockPT.y = row;	// 记录当前障碍图块的行

				// 根据角色当前的方向计算碰撞前的位置
				int x = GetX(), y = GetY();
				switch (GetDir())
				{
				case DIR_LEFT:
					x = GetX() + GetSpeed();
					y = GetY();
					break;
				case DIR_RIGHT:
					x = GetX() - GetSpeed();
					y = GetY();
					break;
				case DIR_UP:
					x = GetX();
					y = GetY() + GetSpeed();
					break;
				case DIR_DOWN:
					x = GetX();
					y = GetY() - GetSpeed();
					break;
				}
				// 将角色定位在撞前的位置
				SetPosition(x, y);
				return true;
			}
		}
	}
	return false;
}
// 加载炮弹
void CrazyMan::LoadArrow(int iTime, T_Scene* scene) {
	SetEndTime(GetTickCount64());
	if (GetEndTime() - GetStartTime() >= (DWORD)iTime)
	{
		SetStartTime(GetEndTime());

		GAMELAYER gameLayer;
		SPRITEINFO arrowInfo;
		int m_dir = faceTo;

		arrow_set.push_back(new T_Sprite(L".\\res\\sprite\\Shooter\\Arrow.png", 320, 128));

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
			arrowInfo.X = GetX() + GetRatioSize().cx / 2 - 2;
			arrowInfo.Y = GetY() + GetRatioSize().cy - 25;
			break;
		case DIR_RIGHT:
			arrowInfo.Rotation = TRANS_NONE;
			arrowInfo.X = GetX() + GetRatioSize().cx / 2 - 2;
			arrowInfo.Y = GetY() + GetRatioSize().cy - 25;
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
		gameLayer.z_order = scene->GetTotalLayers() + 1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		scene->Append(gameLayer);
	}
}
// 更新炮弹位置
void CrazyMan::UpdateArrowPos(T_Scene* scene, vSpriteSet npc_set) {
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
			ArrowCollide((*itp), scene, npc_set);
			//if (GameState != GAME_RUN) return;
		}

		if ((*itp)->IsVisible() == false ||
			(*itp)->GetY() < 0 || (*itp)->GetX() < 0 ||
			(*itp)->GetY() > win_height || (*itp)->GetX() > win_width)
		{
			//删除场景中的对象	
			SCENE_LAYERS::iterator p;
			for (p = scene->getSceneLayers()->begin();
				p != scene->getSceneLayers()->end(); p++)
			{
				if ((*p).layer == (*itp))
				{
					p = scene->getSceneLayers()->erase(p);
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
void CrazyMan::ArrowCollide(T_Sprite* arrow, T_Scene* scene, vSpriteSet npc_set) {
	if (arrow->IsActive() == true && arrow->IsVisible() == true)
	{
		T_Map* map;
		bool collideBarrier = arrow->CollideWith(scene->getBarrier());
		if (collideBarrier)
		{
			map = scene->getBarrier();
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
				for (vSpriteSet::iterator sp = npc_set.begin(); sp != npc_set.end(); sp++)
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
/*
void CrazyFighting::LoadExplosion(int x, int y) {
	SPRITEINFO explInfo;
	GAMELAYER gameLayer;

	explosion_set.push_back(new T_Sprite(L".\\res\\explosion.png", 66, 66));
	T_Sprite* sp_explosion = explosion_set.back();

	explInfo.Active = true;
	explInfo.Dead = false;
	explInfo.Dir = 0;
	explInfo.Rotation = TRANS_NONE;
	explInfo.Ratio = 1;
	explInfo.Level = 0;
	explInfo.Score = 0;
	explInfo.Speed = 5;
	explInfo.Alpha = 255;
	explInfo.X = x;
	explInfo.Y = y;
	explInfo.Visible = true;

	sp_explosion->Initiate(explInfo);
	sp_explosion->SetSequence(EXPLOSION_FRAME, 8);
	sp_explosion->SetLayerTypeID(LAYER_EXPLOSION);

	gameLayer.layer = sp_explosion;
	gameLayer.type_id = LAYER_EXPLOSION;
	gameLayer.z_order = t_scene.getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene.Append(gameLayer);
}
*/