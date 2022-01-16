#include "CrazyMan.h"

int CrazyMan::FRAME_WALK[20] = { 0, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 15, 16, 17 };
int CrazyMan::FRAME_FALL[20] = { 18, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 23 };
int CrazyMan::FRAME_JUMP[20] = { 36, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 41 };
int CrazyMan::FRAME_ATTACK[20] = { 54, 54, 54, 54, 55, 55, 55, 56, 56, 56, 57, 57, 57, 58, 58, 58, 59, 59, 59, 59 };
int CrazyMan::FRAME_DIE[70] = { 72,73,74,75,76,77,78,79,80,80,81,81,82,82,83,83,84,84,85,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86 };
int CrazyMan::FRAME_SHOOT[40] = { 90, 90, 90, 90, 91, 91, 92, 92, 92, 92, 93, 93, 94, 94, 94, 94, 95, 95, 96, 96, 96, 96, 97, 97, 98, 98, 98, 98, 99, 99, 99, 99, 100, 100, 100, 100, 101, 101, 101, 101 };
int CrazyMan::FRAME_KICK[20];
int CrazyMan::FRAME_FLY[20];

CrazyMan::CrazyMan(LPCTSTR imgPath, int frameWidth, int frameHeight, int winWidth, int winHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	win_width = winWidth;
	win_height = winHeight;

	type = 0;

	jumpSpeed = -20;
	gravity = 1;
	jumping = false;
	falling = false;
	fallen = true;
	faceTo = DIR_RIGHT;
	shooting = false;
	shootTime = 600;
	attacking = false;

	wanderTime = 30;
}

CrazyMan::~CrazyMan()
{
}
// �������λ��
void CrazyMan::updatePostion(int xDir, int yDir, int jumpDir, T_Map* pBarrier, int isNPC)
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
		if (nextStepY <= 0 && isNPC == 0)
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
		if (nextStepY >= win_height && isNPC == 0)
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
		if (nextStepX <= 0 && isNPC == 0)
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
		if (nextStepX >= win_width && isNPC == 0)
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
void CrazyMan::jumpUpDown(T_Map* pBarrier, int isNPC)
{
	// Ϊ�ٶȼ�������Ӱ��
	jumpSpeed = jumpSpeed + gravity;
	if (jumpSpeed < 0)   // ������
	{
		//SetDir(DIR_UP);
		// ����ֻ�������·�������xDir��Ϊһ�������ڵķ�����-1
		updatePostion(-1, DIR_UP, DIR_UP, pBarrier, isNPC);
	}
	else if (jumpSpeed > 0)  // ������������
	{
		SetDir(DIR_DOWN);
		// ����ֻ�������·�������xDir��Ϊһ�������ڵķ�����-1
		updatePostion(-1, DIR_DOWN, DIR_DOWN, pBarrier, isNPC);
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


// ��Ȿ��ɫ�����Ƿ���Ŀ���ɫ��ײ(distance����Ϊ������)
bool CrazyMan::CollideWith(T_Sprite* target, int distance)
{
	//���������ײ���Ľ�ɫ��������
	RECT targetRect = *(target->GetCollideRect());
	RECT hitRec;
	hitRec.left = targetRect.left - distance;
	hitRec.top = targetRect.top - distance;
	hitRec.right = targetRect.right + distance;
	hitRec.bottom = targetRect.bottom + distance;
	RECT thisRect = *(this->GetCollideRect());
	int cw = abs(thisRect.right - thisRect.left);
	int tw = abs(hitRec.right - hitRec.left);
	int ch = abs(thisRect.bottom - thisRect.top);
	int th = abs(hitRec.bottom - hitRec.top);

	//��ʽһ��ͬʱ����4������
	return thisRect.left <= hitRec.right &&
		hitRec.left <= thisRect.right &&
		thisRect.top <= hitRec.bottom &&
		hitRec.top <= thisRect.bottom;
	//��ʽ����ͬʱ����2������
		//return (abs((thisRect.left + cw/2)-(hitRec.left + tw/2))<(cw+tw)/2) &&
		//       (abs((thisRect.top + ch/2)-(hitRec.top + th/2))<(ch+th)/2);

}


bool CrazyMan::FrontMissFoot(IN T_Map* map)
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

	int spLeft, spTop, spRight, spBottom;
	// ���㵱ǰ��ɫǰ������ľ��η�Χ

	if (this->getFaceTo() == DIR_LEFT)
	{
		spLeft = (this->GetCollideRect()->left + this->GetCollideRect()->right) / 2 - 16;
		spTop = (this->GetCollideRect()->top + this->GetCollideRect()->bottom) / 2 - 16;
		spRight = (this->GetCollideRect()->left + this->GetCollideRect()->right) / 2 + 16;
		spBottom = (this->GetCollideRect()->top + this->GetCollideRect()->bottom) / 2 - 16;
	}
	else if (this->getFaceTo() == DIR_RIGHT)
	{
		spLeft = this->GetCollideRect()->left + 64;
		spTop = this->GetCollideRect()->top + 64;
		spRight = this->GetCollideRect()->right + 64;
		spBottom = this->GetCollideRect()->bottom + 64;
	}


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
	// ����������ζԣ����˳�����
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
			if (map->getTile(col, row) == 0)
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

