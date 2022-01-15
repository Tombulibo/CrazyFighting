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
// �������λ��
void CrazyMan::updatePostion(int xDir, int yDir, int jumpDir, T_Map* pBarrier)
{
	int nextStepX, nextStepY;
	int SpeedX = 0, SpeedY = 0;

	int movingSpeed = 0;
	if (jumpDir == DIR_DOWN || jumpDir == DIR_UP)// ����������
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
		//������һ���ƶ��Ƿ񳬹��߽�
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
		//������һ���ƶ��Ƿ񳬹��߽�
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
		//������һ���ƶ��Ƿ񳬹��߽�
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
		//������һ���ƶ��Ƿ񳬹��߽�
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

	int x = GetX();// ����ƶ�ǰ��xλ��
	int y = GetY();// ����ƶ�ǰ��yλ��

	if (!CollideWith(pBarrier))
	{
		Move(SpeedX, SpeedY);	// �����ƶ�
	}
	if (CollideWith(pBarrier))
	{
		SetPosition(x, y);		// ��ԭ�ƶ�ǰ��λ��
		if (yDir == DIR_UP)
		{
			jumpSpeed = -jumpSpeed; //���������ٶ�
		}
		if (yDir == DIR_DOWN)
		{
			jumpSpeed = 0; //���������ٶ�
			jumping = false; //��������״̬
			fallen = true;
		}
	}
}
// ����
void CrazyMan::fallingDown()
{
	if (jumping == false)
	{
		jumpSpeed = 0;
		jumping = true;
		falling = true;
	}
}
// ��������������
void CrazyMan::jumpUpDown(T_Map* pBarrier)
{
	// Ϊ�ٶȼ�������Ӱ��
	jumpSpeed = jumpSpeed + gravity;
	if (jumpSpeed < 0)   // ������
	{
		//SetDir(DIR_UP);
		// ����ֻ�������·�������xDir��Ϊһ�������ڵķ�����-1
		updatePostion(-1, DIR_UP, DIR_UP, pBarrier);
	}
	else if (jumpSpeed > 0)  // ������������
	{
		SetDir(DIR_DOWN);
		// ����ֻ�������·�������xDir��Ϊһ�������ڵķ�����-1
		updatePostion(-1, DIR_DOWN, DIR_DOWN, pBarrier);
	}

}

// ��д��ײ��⣬�����ɫ�ز�
bool CrazyMan::CollideWith(IN T_Map* map)
{
	// �������ΪͼƬ�򲻼���ͼ��ײ
	if (map->getMapRows() == 0 && map->getMapCols() == 0)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// �����ͼ���ɼ����ɫ���ɼ�������ͼ��ײ
	if ((!(map->IsVisible())) || (!(this->IsVisible())))
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// ���㵱ǰ��ͼͼ��ľ��η�Χ
	int mapLeft = map->GetX();
	int mapTop = map->GetY();
	int mapRight = mapLeft + map->GetWidth();
	int mapBottom = mapTop + map->GetHeight();

	// ��õ�ͼͼ����ʹ�õ�ͼ��Ŀ��
	int tW = map->getTileWidth();
	int tH = map->getTileHeight();

	// ���㵱ǰ��ɫ�ľ��η�Χ
	int spLeft = this->GetCollideRect()->left + 30;
	int spTop = this->GetCollideRect()->top + 30;
	int spRight = this->GetCollideRect()->right - 30;
	int spBottom = this->GetCollideRect()->bottom;

	// ��õ�ǰ��ͼ��ͼ���������
	int tNumCols = map->getMapCols();
	// ��õ�ǰ��ͼ��ͼ���������
	int tNumRows = map->getMapRows();

	// ���㵱ǰ��ɫ��ʵ�ʵĿ��
	int spW = spRight - spLeft;
	int spH = spBottom - spTop;

	RECT lprcDst;
	// �������ϼ����ͼ��ľ��η�Χ�ͽ�ɫ�ľ��η�Χ�����������ζ���
	RECT mapRect = { mapLeft, mapTop, mapRight, mapBottom };
	RECT spRect = { spLeft, spTop, spRight, spBottom };
	// ����������ζ���û�з����κ���ײ�����˳�����
	if ((IntersectRect(&lprcDst, &mapRect, &spRect)) == FALSE)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// ����������ζ���������ײ�����ȼ����ɫ�����ϡ��¡����ҵľ�������
	int startRow = (spTop <= mapTop) ? 0 : (spTop - mapTop) / tH;
	int endRow = (spBottom < mapBottom) ? (spBottom - 1 - mapTop) / tH : tNumRows - 1;
	int startCol = (spLeft <= mapLeft) ? 0 : (spLeft - mapLeft) / tW;
	int endCol = (spRight < mapRight) ? (spRight - 1 - mapLeft) / tW : tNumCols - 1;

	// ���ݽ�ɫ�����ϡ��¡����ҵľ��������ж��ĸ���������Ϊ�ϰ�
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			// �����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
			if (map->getTile(col, row) != 0)
			{
				mapBlockPT.x = col;	// ��¼��ǰ�ϰ�ͼ�����
				mapBlockPT.y = row;	// ��¼��ǰ�ϰ�ͼ�����

				// ���ݽ�ɫ��ǰ�ķ��������ײǰ��λ��
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
				// ����ɫ��λ��ײǰ��λ��
				SetPosition(x, y);
				return true;
			}
		}
	}
	return false;
}
// �����ڵ�
void CrazyMan::LoadArrow(int iTime, T_Scene* scene) {
	SetEndTime(GetTickCount64());
	if (GetEndTime() - GetStartTime() >= (DWORD)iTime)
	{
		SetStartTime(GetEndTime());

		GAMELAYER gameLayer;
		SPRITEINFO arrowInfo;
		int m_dir = faceTo;

		arrow_set.push_back(new T_Sprite(L".\\res\\sprite\\Shooter\\Arrow.png", 320, 128));

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
// �����ڵ�λ��
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

			//����ڵ��Ƿ����Ŀ��
			ArrowCollide((*itp), scene, npc_set);
			//if (GameState != GAME_RUN) return;
		}

		if ((*itp)->IsVisible() == false ||
			(*itp)->GetY() < 0 || (*itp)->GetX() < 0 ||
			(*itp)->GetY() > win_height || (*itp)->GetX() > win_width)
		{
			//ɾ�������еĶ���	
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
// ����ڵ��Ƿ����Ŀ��
void CrazyMan::ArrowCollide(T_Sprite* arrow, T_Scene* scene, vSpriteSet npc_set) {
	if (arrow->IsActive() == true && arrow->IsVisible() == true)
	{
		T_Map* map;
		bool collideBarrier = arrow->CollideWith(scene->getBarrier());
		if (collideBarrier)
		{
			map = scene->getBarrier();
			//map->setTile(bomb->GetMapBlockPT().x, bomb->GetMapBlockPT().y, 0);	//��ȡ�ϰ�������к�
			arrow->SetVisible(false);			//�ڵ����ɼ�
			arrow->SetActive(false);				//�ڵ�������
			int x = arrow->GetX() - 33;
			int y = arrow->GetY() - 33;
			//LoadExplosion(x, y);

			//explosionSound.Restore();
			//explosionSound.Play(false);
		}

		// ���NPC�ڵ����������
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

		// �������ڵ�������NPC
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
						//NPC��Ŀ����
						//NpcNumber = NpcNumber - 1;
						//LoadExplosion((*sp)->GetX(), (*sp)->GetY());
						//explosionSound.Play(false);
						//��Ϸ������������
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