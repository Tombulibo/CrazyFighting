#pragma once
#include "CrazyMan.h"
class Shooter :
    public CrazyMan
{
protected:
	
public:
	Shooter(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0, int winWidth = WIN_WIDTH, int winHeight = WIN_HEIGHT);
	virtual ~Shooter();
	
	static int FRAME_WALK[20];
	static int FRAME_FALL[20];
	static int FRAME_JUMP[20];
	static int FRAME_SLIDE[20];
	static int FRAME_DIE[20];
	static int FRAME_SHOOT[20];
	static int FRAME_KICK[20];
	static int FRAME_FLY[20];


};

