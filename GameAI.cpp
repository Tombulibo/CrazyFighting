#include "GameAI.h"


//���ά��GAME_DIR��Ԫ�ر��һ��
//�ڶ�ά��ʾ��Ӧ����Ķ�ܷ���
int GameAI::EvadeDir[8][5] = {
	//DIR_LEFT��ܷ���
	{DIR_RIGHT, DIR_DOWN, DIR_UP, DIR_RIGHT_UP, DIR_RIGHT_DOWN},
	//DIR_RIGHT��ܷ���
	{DIR_LEFT, DIR_DOWN, DIR_UP, DIR_LEFT_UP, DIR_LEFT_DOWN},
	//DIR_UP��ܷ���
	{DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_LEFT_DOWN, DIR_RIGHT_DOWN},
	//DIR_DOWN��ܷ���
	{DIR_UP, DIR_LEFT, DIR_RIGHT, DIR_LEFT_UP, DIR_RIGHT_UP},
	//DIR_LEFT_UP��ܷ���
	{DIR_RIGHT_DOWN, DIR_RIGHT_UP, DIR_RIGHT, DIR_UP, DIR_DOWN},
	//DIR_LEFT_DOWN��ܷ���
	{DIR_RIGHT_UP, DIR_RIGHT_DOWN, DIR_RIGHT, DIR_UP, DIR_DOWN},
	//DIR_RIGHT_UP��ܷ���
	{DIR_LEFT_DOWN, DIR_LEFT_UP, DIR_LEFT, DIR_UP, DIR_DOWN},
	//DIR_RIGHT_DOWN��ܷ���
	{DIR_LEFT_UP, DIR_LEFT_DOWN, DIR_LEFT, DIR_UP, DIR_DOWN}
};

GameAI::GameAI(int d_style ):T_AI(d_style)
{
}

GameAI::~GameAI(void)
{
}

// �������spָ���Ľ�ɫǰ�������Ӧ����ײ�������
// ����sp�������Ľ�ɫ����
// ����spSizeTimes����ɫ��ߵı����������������ķ�Χ
// ����testRec������Ͳ������õ����յļ������
void GameAI::GetCHitRect(IN CrazyMan* sp, IN int spSizeTimes, OUT RECT& testRec)
{
	int sp_width = sp->GetRatioSize().cx;
	int sp_height = sp->GetRatioSize().cy;

	switch (sp->GetDir())
	{
	case DIR_LEFT:
		testRec.left = sp->GetX() - spSizeTimes * sp_width;
		testRec.top = sp->GetY();
		testRec.right = sp->GetX();
		testRec.bottom = sp->GetY() + sp_height;;
		break;

	case DIR_RIGHT:
		testRec.left = sp->GetX() + sp_width;
		testRec.top = sp->GetY();
		testRec.right = sp->GetX() + (spSizeTimes + 1) * sp_width;
		testRec.bottom = sp->GetY() + sp_height;;
		break;

	case DIR_UP:
		testRec.left = sp->GetX();
		testRec.top = sp->GetY() - spSizeTimes * sp_height;
		testRec.right = sp->GetX() + sp_width;
		testRec.bottom = sp->GetY();
		break;

	case DIR_DOWN:
		testRec.left = sp->GetX();
		testRec.top = sp->GetY() + sp_height;;
		testRec.right = sp->GetX() + sp_width;
		testRec.bottom = sp->GetY() + (spSizeTimes + 1) * sp_height;
		break;

	case DIR_LEFT_UP:
		testRec.left = sp->GetX() - spSizeTimes * sp_width;
		testRec.top = sp->GetY() - spSizeTimes * sp_height;
		testRec.right = sp->GetX();
		testRec.bottom = sp->GetY();
		break;

	case DIR_LEFT_DOWN:
		testRec.left = sp->GetX() - spSizeTimes * sp_width;
		testRec.top = sp->GetY() + sp_height;;
		testRec.right = sp->GetX();
		testRec.bottom = sp->GetY() + (spSizeTimes + 1) * sp_height;
		break;

	case DIR_RIGHT_UP:
		testRec.left = sp->GetX() + sp_width;
		testRec.top = sp->GetY() - spSizeTimes * sp_height;
		testRec.right = sp->GetX() + (spSizeTimes + 1) * sp_width;
		testRec.bottom = sp->GetY();
		break;

	case DIR_RIGHT_DOWN:
		testRec.left = sp->GetX() + sp_width;
		testRec.top = sp->GetY() + sp_height;;
		testRec.right = sp->GetX() + (spSizeTimes + 1) * sp_width;
		testRec.bottom = sp->GetY() + (spSizeTimes + 1) * sp_height;
		break;
	}
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �õ�һ��T_Sprite������Χ8������ľ�������
// ����sp�������Ľ�ɫ����
// ����spSizeTimes����ɫ��ߵı����������������ķ�Χ
// ����testRec������Ͳ�����ָ��һ��8��Ԫ�ص�RECT���飬�õ����ս�ɫ������Χ8������ľ��μ������
void GameAI::GetCHitRects(IN CrazyMan* sp, IN float spSizeTimes, OUT RECT*& testRec)
{
	if (testRec == NULL)
	{
		testRec = new RECT[8];
	}
	int sp_width = sp->GetRatioSize().cx;
	int sp_height = sp->GetRatioSize().cy;

	for (int i = 0; i < 8; i++)
	{
		if (i == DIR_LEFT)
		{
			testRec[i].left = (LONG)(sp->GetX() - spSizeTimes * sp_width);
			testRec[i].top = (LONG)sp->GetY();
			testRec[i].right = (LONG)sp->GetX();
			testRec[i].bottom = (LONG)(sp->GetY() + sp_height);
		}
		if (i == DIR_RIGHT)
		{
			testRec[i].left = (LONG)(sp->GetX() + sp_width);
			testRec[i].top = (LONG)sp->GetY();
			testRec[i].right = (LONG)(sp->GetX() + (spSizeTimes + 1) * sp_width);
			testRec[i].bottom = (LONG)(sp->GetY() + sp_height);
		}
		if (i == DIR_UP)
		{
			testRec[i].left = (LONG)sp->GetX();
			testRec[i].top = (LONG)(sp->GetY() - spSizeTimes * sp_height);
			testRec[i].right = (LONG)(sp->GetX() + sp_width);
			testRec[i].bottom = (LONG)sp->GetY();
		}

		if (i == DIR_DOWN)
		{
			testRec[i].left = (LONG)sp->GetX();
			testRec[i].top = (LONG)(sp->GetY() + sp_height);
			testRec[i].right = (LONG)(sp->GetX() + sp_width);
			testRec[i].bottom = (LONG)(sp->GetY() + (spSizeTimes + 1) * sp_height);
		}
		if (i == DIR_LEFT_UP)
		{
			testRec[i].left = (LONG)(sp->GetX() - spSizeTimes * sp_width);
			testRec[i].top = (LONG)(sp->GetY() - spSizeTimes * sp_height);
			testRec[i].right = (LONG)sp->GetX();
			testRec[i].bottom = (LONG)sp->GetY();
		}
		if (i == DIR_LEFT_DOWN)
		{
			testRec[i].left = (LONG)(sp->GetX() - spSizeTimes * sp_width);
			testRec[i].top = (LONG)(sp->GetY() + sp_height);
			testRec[i].right = (LONG)sp->GetX();
			testRec[i].bottom = (LONG)(sp->GetY() + (spSizeTimes + 1) * sp_height);
		}
		if (i == DIR_RIGHT_UP)
		{
			testRec[i].left = (LONG)(sp->GetX() + sp_width);
			testRec[i].top = (LONG)(sp->GetY() - spSizeTimes * sp_height);
			testRec[i].right = (LONG)(sp->GetX() + (spSizeTimes + 1) * sp_width);
			testRec[i].bottom = (LONG)sp->GetY();
		}
		if (i == DIR_RIGHT_DOWN)
		{
			testRec[i].left = (LONG)(sp->GetX() + sp_width);
			testRec[i].top = (LONG)(sp->GetY() + sp_height);
			testRec[i].right = (LONG)(sp->GetX() + (spSizeTimes + 1) * sp_width);
			testRec[i].bottom = (LONG)(sp->GetY() + (spSizeTimes + 1) * sp_height);
		}
	}
}

// �����ƶ������ٶȺ�ָ���Ļ��Χ���㵱ǰ��ɫ���ƶ�λ��
MoveCoord GameAI::GetCMoveCoord(CrazyMan* npc_sp, RECT boundary)
{
	MoveCoord mRate;
	int nextStepX, nextStepY;
	int SpeedX = 0, SpeedY = 0;
	mRate.BoundDir = -1;	//û���ڱ߽���

	int sp_width = npc_sp->GetRatioSize().cx;
	int sp_height = npc_sp->GetRatioSize().cy;

	int speed = npc_sp->GetSpeed();
	int dir = npc_sp->GetDir();

	switch (dir)
	{
	//case DIR_LEFT:
	//	SpeedX = -npc_sp->GetSpeed();
	//	SpeedY = 0;
	//	//������һ���ƶ��Ƿ񳬹��߽�
	//	nextStepX = npc_sp->GetX() - speed;
	//	if (nextStepX <= boundary.left)
	//	{
	//		SpeedX = -(npc_sp->GetX() - boundary.left);
	//		mRate.BoundDir = DIR_LEFT;
	//	}
	//	break;

	//case DIR_RIGHT:
	//	SpeedX = speed;
	//	SpeedY = 0;
	//	//������һ���ƶ��Ƿ񳬹��߽�
	//	nextStepX = npc_sp->GetX() + sp_width + speed;
	//	if (nextStepX >= boundary.right)
	//	{
	//		SpeedX = boundary.right - (npc_sp->GetX() + sp_width);
	//		mRate.BoundDir = DIR_RIGHT;
	//	}
	//	break;

	case DIR_UP:
		SpeedX = 0;
		SpeedY = -speed;
		//������һ���ƶ��Ƿ񳬹��߽�
		nextStepY = npc_sp->GetY() - speed;
		if (nextStepY <= boundary.top)
		{
			SpeedY = -(npc_sp->GetY() - boundary.top);
			mRate.BoundDir = DIR_UP;
		}
		break;

	case DIR_DOWN:
		SpeedX = 0;
		SpeedY = speed;
		//������һ���ƶ��Ƿ񳬹��߽�
		nextStepY = npc_sp->GetY() + sp_height + speed;
		if (nextStepY >= boundary.bottom)
		{
			SpeedY = boundary.bottom - (npc_sp->GetY() + sp_height);
			mRate.BoundDir = DIR_DOWN;
		}
		break;

	case DIR_LEFT_UP:
		SpeedX = -speed;
		SpeedY = -speed;

		//������һ���ƶ��Ƿ񳬹��߽�
		nextStepX = npc_sp->GetX() - speed;
		if (nextStepX <= boundary.left)
		{
			SpeedX = -(npc_sp->GetX() - boundary.left);
			SpeedY = -(npc_sp->GetX() - boundary.left);
			mRate.BoundDir = DIR_LEFT;
			break;
		}
		nextStepY = npc_sp->GetY() - speed;
		if (nextStepY <= boundary.top)
		{
			SpeedY = -(npc_sp->GetY() - boundary.top);
			SpeedX = -(npc_sp->GetY() - boundary.top);
			mRate.BoundDir = DIR_UP;
		}
		break;

	case DIR_LEFT:
		SpeedX = -speed;
		SpeedY = speed;

		//������һ���ƶ��Ƿ񳬹��߽�
		nextStepX = npc_sp->GetX() - speed;
		if (nextStepX <= boundary.left)
		{
			SpeedX = -(npc_sp->GetX() - boundary.left);
			SpeedY = (npc_sp->GetX() - boundary.left);
			mRate.BoundDir = DIR_LEFT;
			break;
		}
		nextStepY = npc_sp->GetY() + sp_height + speed;
		if (nextStepY >= boundary.bottom)
		{
			SpeedY = boundary.bottom - (npc_sp->GetY() + sp_height);
			SpeedX = -(boundary.bottom - (npc_sp->GetY() + sp_height));
			mRate.BoundDir = DIR_DOWN;
		}
		break;

	case DIR_RIGHT_UP:
		SpeedX = speed;
		SpeedY = -speed;

		//������һ���ƶ��Ƿ񳬹��߽�
		nextStepX = npc_sp->GetX() + sp_width + speed;
		if (nextStepX >= boundary.right)
		{
			SpeedX = boundary.right - (npc_sp->GetX() + sp_width);
			SpeedY = -(boundary.right - (npc_sp->GetX() + sp_width));
			mRate.BoundDir = DIR_RIGHT;
			break;
		}
		nextStepY = npc_sp->GetY() - speed;
		if (nextStepY <= boundary.top)
		{
			SpeedY = -(npc_sp->GetY() - boundary.top);
			SpeedX = npc_sp->GetY() - boundary.top;
			mRate.BoundDir = DIR_UP;
		}
		break;

	case DIR_RIGHT:
		SpeedX = speed;
		SpeedY = speed;

		//������һ���ƶ��Ƿ񳬹��߽�
		nextStepX = npc_sp->GetX() + sp_width + speed;
		if (nextStepX >= boundary.right)
		{
			SpeedX = boundary.right - (npc_sp->GetX() + sp_width);
			SpeedY = boundary.right - (npc_sp->GetX() + sp_width);
			mRate.BoundDir = DIR_RIGHT;
			break;
		}
		nextStepY = npc_sp->GetY() + sp_height + speed;
		if (nextStepY >= boundary.bottom)
		{
			SpeedY = boundary.bottom - (npc_sp->GetY() + sp_height);
			SpeedX = boundary.bottom - (npc_sp->GetY() + sp_height);
			mRate.BoundDir = DIR_DOWN;
		}
	}

	mRate.moveX = SpeedX;
	mRate.moveY = SpeedY;
	mRate.oldX = npc_sp->GetX();
	mRate.oldY = npc_sp->GetY();
	return mRate;
}
void GameAI::Escape(CrazyMan* npc_sp, CrazyMan* player, RECT boundary)
{
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �����ƶ������ٶȺ�ָ���Ļ��Χ�Խ�ɫ�����δ���
void GameAI::Wander(CrazyMan* npc_sp, RECT boundary)
{
	// ��ȡ��ǰ��Ϸ��ɫ���ƶ�������ٶȼ�������һ�����ƶ���Ϣ
	MoveCoord mRate = GetCMoveCoord(npc_sp, boundary);
	// �����ɫû�е�����Χ�ı߽�
	if (mRate.BoundDir == -1)
	{
		// ��ɫ�����ƶ�
		npc_sp->Move(mRate.moveX, mRate.moveY);
	}
	// �����ɫ������Χ�ı߽�
	if (mRate.BoundDir != -1)
	{
		int r_dir = 0;
		// ���ݽ�ɫ���ƶ�����ģʽ��ȡһ�������ܷ���
		if (dir_style == 8)
		{
			r_dir = rand() % 5;
		}
		else
		{
			r_dir = rand() % 3;
		}
		// ����ɫ��λ�ڱ߽�
		npc_sp->SetPosition(mRate.oldX, mRate.oldY);
		// Ϊ��ɫ�����µ��ƶ�����
		npc_sp->SetDir(EvadeDir[mRate.BoundDir][r_dir]);
	}
}

// �����ƶ������ٶȺ�ָ���ĵ�ͼ����Խ�ɫ�����δ���
void GameAI::Wander(CrazyMan* npc_sp, T_Map* map,int type)
{
	// ������Ϸ��ͼ�ľ��η�Χ
	RECT mapBound;
	mapBound.left = map->GetX();
	mapBound.top = map->GetY();
	mapBound.right = map->GetX() + map->GetWidth();
	mapBound.bottom = map->GetY() + map->GetHeight();

	// ��ȡ��ǰ��Ϸ��ɫ���ƶ�������ٶȼ�������һ�����ƶ���Ϣ
	MoveCoord mRate = GetCMoveCoord(npc_sp, mapBound);

	// ���ݽ�ɫ���ƶ��ķ���ģʽ��ȡһ�������ܷ�
	if (type == 0) 
	{
		// �����ɫû�е�����Ϸ��ͼ�ı߽�
		if (mRate.BoundDir == -1)
		{
			npc_sp->Move(mRate.moveX, mRate.moveY);
		}

		// �����ɫ�Ѿ�������Ϸ��ͼ�ı߽�
		if (mRate.BoundDir != -1)
		{
			npc_sp->SetPosition(mRate.oldX, mRate.oldY);
		}
		int t = npc_sp->getWanderTime();
		if (t > 15)
		{
			npc_sp->setFaceTo(DIR_RIGHT);
			npc_sp->SetDir(DIR_RIGHT);
		}
		else if (t > 0) 
		{
			npc_sp->setFaceTo(DIR_LEFT);
			npc_sp->SetDir(DIR_LEFT);
		}
		t--;
		npc_sp->setWanderTime(t);
		if(t<0)
		{	
			npc_sp->setWanderTime(30);
		}
	  
			if ((npc_sp->FrontMissFoot(map) 
				||
				npc_sp->CollideWith(map)
				)&& npc_sp ->getFallen()
				)
			{
				npc_sp->SetPosition(mRate.oldX, mRate.oldY);

			}
	}
	else if (type == 1) 
	{
		// ���ݽ�ɫ���ƶ��ķ���ģʽ��ȡһ�������ܷ���
		int r_dir = 0;
		if (dir_style == 8)
		{
			r_dir = rand() % 5;
		}
		else
		{
			r_dir = rand() % 3;
		}

		// �����ɫû�е�����Ϸ��ͼ�ı߽�
		if (mRate.BoundDir == -1)
		{
			npc_sp->Move(mRate.moveX, mRate.moveY);
		}
		// �����ɫ�Ѿ�������Ϸ��ͼ�ı߽�
		if (mRate.BoundDir != -1)
		{
			npc_sp->SetPosition(mRate.oldX, mRate.oldY);
			npc_sp->SetDir(EvadeDir[mRate.BoundDir][r_dir]);
		}
		if (npc_sp->CollideWith(map))
		{
			// ����ɫ��λ�ڱ߽�
			npc_sp->SetPosition(mRate.oldX, mRate.oldY);
			// Ϊ��ɫ�����µ��ƶ����򣨳�����3�������ܣ�
			npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][rand() % 3]);
		}
	}
}

void GameAI::Evade(CrazyMan* npc_sp, CrazyMan* player)
{
	RECT lprcDst;
	RECT playerRect = { player->GetX(), player->GetY(),
					 player->GetX() + player->GetRatioSize().cx,
					 player->GetY() + player->GetRatioSize().cy };

	RECT testRect;
	GetCHitRect(npc_sp, 1, testRect);

	if ((IntersectRect(&lprcDst, &playerRect, &testRect)) == TRUE)
	{

		int r_dir = 0;
		if (dir_style == 8)
		{
			r_dir = rand() % 5;
		}
		else
		{
			r_dir = rand() % 1;
		}
		npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][r_dir]);
	}
}



void GameAI::CheckOverlay(CrazyMan* npc_sp, vector<CrazyMan*> vCrazyManSet)
{
	vector<CrazyMan*>::iterator p;

	RECT testRec;
	GetCHitRect(npc_sp, 1, testRec);// ��ȡ��ǰSprite��Χ�ľ���

	int r_dir = 0;
	if (dir_style == 8)
	{
		r_dir = rand() % 5;
	}
	else
	{
		r_dir = rand() % 1;
	}

	RECT lprcDst;
	for (p = vCrazyManSet.begin(); p != vCrazyManSet.end(); p++)
	{
		if (*p == npc_sp) continue;

		RECT pRect = { (*p)->GetX(), (*p)->GetY(),
					(*p)->GetX() + (*p)->GetRatioSize().cx,
					(*p)->GetY() + (*p)->GetRatioSize().cy };

		if ((IntersectRect(&lprcDst, &pRect, &testRec)) == TRUE)
		{
			npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][r_dir]);
		}
	}
}

void GameAI::Chase(CrazyMan* npc_sp, CrazyMan* player, T_Map* map)
{
	RECT lprcDst;
	RECT playerRect = { player->GetX(), player->GetY(),
					 player->GetX() + player->GetRatioSize().cx,
					 player->GetY() + player->GetRatioSize().cy };

	RECT testRect;
	GetCHitRect(npc_sp, 1, testRect);

	if ((IntersectRect(&lprcDst, &playerRect, &testRect)) == FALSE)
	{
		if (npc_sp->GetX() > player->GetX()) 
		{
			npc_sp->SetDir(DIR_LEFT);
		}
		else 
		{
			npc_sp->SetDir(DIR_RIGHT);
		}
		//if (!npc_sp->CollideWith(map)) 
		//{
		//	if (npc_sp->getJumping() == false)
		//	{
		//		npc_sp->SetDir(DIR_UP);
		//		npc_sp->SetActive(true);
		//		npc_sp->setJumping(true);
		//		npc_sp->setJumpSpeed(-20);
		//		npc_sp->setFallen(false);
		//	}
		//}
	}

}
