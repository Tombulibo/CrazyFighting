#include "Horizontal_Scene.h"

void Horizontal_Scene::ScrollScene(CrazyMan* player)
{
	bool scn_scrolling = false;
	int speedX = 0, speedY = 0;

		if (player->GetX() <= (WinWidth / 5))// 在离窗口左边缘五分之一以内区域，场景需要滚动
		{
//			speedX = movingSpeed;
			speedX = (WinWidth / 5) - player->GetX();
			//speedY = 0;
			scn_scrolling = true;
		}
		else if (player->GetX() >= (WinWidth * 4 / 5) - player->GetRatioSize().cx)// 在离窗口右边缘五分之一以内区域，场景需要滚动
		{
//			speedX = -movingSpeed;
			speedX = (WinWidth * 4 / 5) - player->GetRatioSize().cx - player->GetX();
			//speedY = 0;
			scn_scrolling = true;
		}
		//else
		//{
		//	scn_scrolling = false;
		//}
		if (player->GetY() <= (WinHeight / 5))// 在离窗口上边缘五分之一以内区域，场景需要滚动
		{
			//speedX = 0;
			speedY = (WinHeight / 5) - player->GetY();
//			speedY = movingSpeed;
			scn_scrolling = true;
		}
		else if (player->GetY() >= (WinHeight * 4 / 5) - player->GetRatioSize().cy)// 在离窗口下边缘五分之一以内区域，场景需要滚动
		{
			//speedX = 0;
//			speedY = -movingSpeed;
			speedY = (WinHeight * 4 / 5) - player->GetRatioSize().cy - player->GetY();
			scn_scrolling = true;
		}

	//如果地图可以滚动，滚动地图
	if (scn_scrolling == true)
	{
		MoveScene((int)speedX, (int)speedY);
	}
}
