#include "Shooter.h"

int Shooter::FRAME_WALK[20] = { 0,1,2,3,3,4,5,6,7,8,9,10,11,12,13,14,14,15,16,17 };
int Shooter::FRAME_FALL[20] = { 18, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 23 };
int Shooter::FRAME_JUMP[20] = { 36, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 41 };
int Shooter::FRAME_SLIDE[20] = { 54, 54, 54, 54, 55, 55, 55, 56, 56, 56, 57, 57, 57, 58, 58, 58, 59, 59, 59, 59 };
int Shooter::FRAME_DIE[20];
int Shooter::FRAME_SHOOT[20];
int Shooter::FRAME_KICK[20];
int Shooter::FRAME_FLY[20];

Shooter::Shooter(LPCTSTR imgPath, int frameWidth, int frameHeight, int winWidth, int winHeight)
	:CrazyMan(imgPath, frameWidth, frameHeight, winWidth, winHeight)
{
}

Shooter::~Shooter()
{
}
