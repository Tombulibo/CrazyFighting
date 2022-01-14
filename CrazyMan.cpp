#include "CrazyMan.h"

CrazyMan::CrazyMan(LPCTSTR imgPath, int frameWidth, int frameHeight, int winWidth, int winHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	win_width = winWidth;
	win_height = winHeight;

	jumpSpeed = -16;
	gravity = 1;
	jumping = false;
	falling = false;
}

CrazyMan::~CrazyMan()
{
}

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
			jumpSpeed = 0; //���������ٶ�
		}
		if (yDir == DIR_DOWN)
		{
			jumpSpeed = 0; //���������ٶ�
			jumping = false; //��������״̬
		}
	}
}

void CrazyMan::fallingDown()
{
	if (jumping == false)
	{
		jumpSpeed = 0;
		jumping = true;
		falling = true;
	}
}

void CrazyMan::jumpUpDown(T_Map* pBarrier)
{
	// Ϊ�ٶȼ�������Ӱ��
	jumpSpeed = jumpSpeed + gravity;
	if (jumpSpeed < 0)   // ������
	{
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