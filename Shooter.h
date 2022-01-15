#pragma once
#include "CrazyMan.h"
#include "Horizontal_Scene.h"


class Shooter :
    public CrazyMan
{
protected:
	
	bool shooting;			//是否射箭
	int shootTime;		//射箭间隔时间

	static int EXPLOSION_FRAME[8];

	//vSpriteSet explosion_set;
	vSpriteSet arrow_set;

public:
	Shooter(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0, int winWidth = WIN_WIDTH, int winHeight = WIN_HEIGHT);
	virtual ~Shooter();
	
	static int FRAME_WALK[20];
	static int FRAME_FALL[20];
	static int FRAME_JUMP[20];
	static int FRAME_SLIDE[20];
	static int FRAME_DIE[20];
	static int FRAME_SHOOT[40];
	static int FRAME_KICK[20];
	static int FRAME_FLY[20];

	void setShooting(bool shoot) { shooting = shoot; }
	bool getShooting() { return shooting; }

	void setShootTime(int stime) { shootTime = stime; }
	int getShootTime() { return shootTime; }


	void LoadArrow(int iTime, Horizontal_Scene* scene);
	void UpdateArrowPos(Horizontal_Scene* scene, vSpriteSet npc_set);
	void ArrowCollide(T_Sprite* arrow, Horizontal_Scene* scene, vSpriteSet npc_set);
};

