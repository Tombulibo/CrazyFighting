#pragma once
#include "TinyEngine\\T_Config.h"
#include "TinyEngine\\T_Sprite.h"
#include "TinyEngine\\T_Map.h"

class CrazyMan : public T_Sprite
{
protected:
	// ÓëÌøÔ¾Ïà¹Ø±äÁ¿
	int jumpSpeed;
	int gravity;
	bool jumping;
	bool falling;
	bool fallen;
	int win_width, win_height;

public:
	CrazyMan(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0, int winWidth = WIN_WIDTH, int winHeight = WIN_HEIGHT);
	virtual ~CrazyMan();

	void setJumpSpeed(int jspeed) { jumpSpeed = jspeed; }
	int getJumpSpeed() { return jumpSpeed; }

	void setJumping(bool jump) { jumping = jump; }
	bool getJumping() { return jumping; }

	void setFalling(bool fall) { falling = fall; }
	bool getFalling() { return falling; }

	void setFallen(bool fall) { fallen = fall; }
	bool getFallen() { return fallen; }


	// ¼ì²â½ÇÉ«Åö×², distance¼ì²âÅö×²µÄ¾àÀë
	bool CollideWith(T_Sprite* target, int distance = 0);
	// ¼ì²âµØÍ¼Åö×²
	bool CollideWith(IN T_Map* map);

	void updatePostion(int xDir, int yDir, int jumpDir, T_Map* pBarrier);
	void jumpUpDown(T_Map* pBarrier);
	void fallingDown();

	
};
