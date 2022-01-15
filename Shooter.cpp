#include "Shooter.h"

int Shooter::FRAME_WALK[20] = { 0, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 15, 16, 17 };
int Shooter::FRAME_FALL[20] = { 18, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 23 };
int Shooter::FRAME_JUMP[20] = { 36, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 41 };
int Shooter::FRAME_SLIDE[20] = { 54, 54, 54, 54, 55, 55, 55, 56, 56, 56, 57, 57, 57, 58, 58, 58, 59, 59, 59, 59 };
int Shooter::FRAME_DIE[20];
int Shooter::FRAME_SHOOT[40] = { 90, 90, 90, 90, 91, 91, 92, 92, 92, 92, 93, 93, 94, 94, 94, 94, 95, 95, 96, 96, 96, 96, 97, 97, 98, 98, 98, 98, 99, 99, 99, 99, 100, 100, 100, 100, 101, 101, 101, 101 };
int Shooter::FRAME_KICK[20];
int Shooter::FRAME_FLY[20];

Shooter::Shooter(LPCTSTR imgPath, int frameWidth, int frameHeight, int winWidth, int winHeight)
	:CrazyMan(imgPath, frameWidth, frameHeight, winWidth, winHeight)
{
	win_width = winWidth;
	win_height = winHeight;

	shooting = false;
	shootTime = 600;
}

Shooter::~Shooter()
{
}


void Shooter::LoadArrow(int iTime, T_Scene* scene) {
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

void Shooter::UpdateArrowPos(T_Scene* scene, vSpriteSet npc_set) {
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

void Shooter::ArrowCollide(T_Sprite* arrow, T_Scene* scene, vSpriteSet npc_set) {
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