#pragma once
#include "TinyEngine\\T_Sprite.h"
#include "TinyEngine\\T_Map.h"

class CrazyMan : public T_Sprite
{
protected:
	int type;				// 角色类型，0为女射手，1为狂战士，2为元素使，3为NPC1，4为NPC2，5为NPC3

	// 与跳跃相关变量
	int jumpSpeed;
	int gravity;
	bool jumping;
	bool falling;
	bool fallen;

	int faceTo;
	bool shooting;			// 是否射箭
	int shootTime;			// 射箭间隔时间
	bool attacking;			// 是否攻击
	int wanderTime;			// 徘徊时间


	int win_width, win_height;

public:
	CrazyMan(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0, int winWidth = WIN_WIDTH, int winHeight = WIN_HEIGHT);
	virtual ~CrazyMan();

	static int FRAME_WALK[20];
	static int FRAME_FALL[20];
	static int FRAME_JUMP[20];
	static int FRAME_ATTACK[20];
	static int FRAME_DIE[70];
	static int FRAME_SHOOT[40];
	static int FRAME_KICK[20];
	static int FRAME_FLY[20];

	void setType(int t) { type = t; }
	int getType() { return type; }

	void setJumpSpeed(int jspeed) { jumpSpeed = jspeed; }
	int getJumpSpeed() { return jumpSpeed; }

	void setJumping(bool jump) { jumping = jump; }
	bool getJumping() { return jumping; }

	void setFalling(bool fall) { falling = fall; }
	bool getFalling() { return falling; }

	void setFallen(bool fall) { fallen = fall; }
	bool getFallen() { return fallen; }

	void setFaceTo(int fto) { faceTo = fto; }
	int getFaceTo() { return faceTo; }

	void updatePostion(int xDir, int yDir, int jumpDir, T_Map* pBarrier, int isNPC = 0);
	void jumpUpDown(T_Map* pBarrier, int isNPC = 0);
	void fallingDown();

	void setShooting(bool shoot) { shooting = shoot; }
	bool getShooting() { return shooting; }

	void setShootTime(int stime) { shootTime = stime; }
	int getShootTime() { return shootTime; }

	void setAttacking(bool attack) { attacking = attack; }
	bool getAttacking() { return attacking; }

	void setWanderTime(int t) { wanderTime = t; }
	int getWanderTime() { return wanderTime; }

	bool FrontMissFoot(IN T_Map* map);			//

	bool CollideWith(IN T_Map* map);			// 重写碰撞检测，适配角色素材
	bool CollideWith(T_Sprite* target, int distance = 0);
};
