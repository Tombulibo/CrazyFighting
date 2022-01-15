#include "Horizontal_Scene.h"

void Horizontal_Scene::ScrollScene(CrazyMan* player)
{
	bool scn_scrolling = false;
	int speedX = 0, speedY = 0;

		if (player->GetX() <= (WinWidth / 5))// ���봰�����Ե���֮һ�������򣬳�����Ҫ����
		{
//			speedX = movingSpeed;
			speedX = (WinWidth / 5) - player->GetX();
			//speedY = 0;
			scn_scrolling = true;
		}
		else if (player->GetX() >= (WinWidth * 4 / 5) - player->GetRatioSize().cx)// ���봰���ұ�Ե���֮һ�������򣬳�����Ҫ����
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
		if (player->GetY() <= (WinHeight / 5))// ���봰���ϱ�Ե���֮һ�������򣬳�����Ҫ����
		{
			//speedX = 0;
			speedY = (WinHeight / 5) - player->GetY();
//			speedY = movingSpeed;
			scn_scrolling = true;
		}
		else if (player->GetY() >= (WinHeight * 4 / 5) - player->GetRatioSize().cy)// ���봰���±�Ե���֮һ�������򣬳�����Ҫ����
		{
			//speedX = 0;
//			speedY = -movingSpeed;
			speedY = (WinHeight * 4 / 5) - player->GetRatioSize().cy - player->GetY();
			scn_scrolling = true;
		}

	//�����ͼ���Թ�����������ͼ
	if (scn_scrolling == true)
	{
		MoveScene((int)speedX, (int)speedY);
	}
}
