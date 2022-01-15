#include "CrazyMan.h"

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
}

CrazyMan::~CrazyMan()
{
}

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