#pragma once
#include "TinyEngine\\T_Config.h"
#include "TinyEngine\\T_Sprite.h"
#include "TinyEngine\\T_Map.h"
#include "TinyEngine\\T_Scene.h"

//����vector�������͵���������vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class CrazyMan : public T_Sprite
{
protected:
	// ����Ծ��ر���
	int jumpSpeed;
	int gravity;
	bool jumping;
	bool falling;
	bool fallen;
	int faceTo;
	bool shooting;			//�Ƿ����
	int shootTime;			//������ʱ��
	vSpriteSet arrow_set;

	int win_width, win_height;

public:
	CrazyMan(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0, int winWidth = WIN_WIDTH, int winHeight = WIN_HEIGHT);
	virtual ~CrazyMan();

	static int SHOOTER_WALK[20];
	static int SHOOTER_FALL[20];
	static int SHOOTER_JUMP[20];
	static int SHOOTER_SLIDE[20];
	static int SHOOTER_DIE[20];
	static int SHOOTER_SHOOT[40];
	static int SHOOTER_KICK[20];
	static int SHOOTER_FLY[20];

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

	void updatePostion(int xDir, int yDir, int jumpDir, T_Map* pBarrier);
	void jumpUpDown(T_Map* pBarrier);
	void fallingDown();


	bool CollideWith(IN T_Map* map);			// ��д��ײ��⣬�����ɫ�ز�

	void setShooting(bool shoot) { shooting = shoot; }
	bool getShooting() { return shooting; }

	void setShootTime(int stime) { shootTime = stime; }
	int getShootTime() { return shootTime; }


	void LoadArrow(int iTime, T_Scene* scene);									// �����ڵ�
	void UpdateArrowPos(T_Scene* scene, vSpriteSet npc_set);					// �����ڵ�λ��
	void ArrowCollide(T_Sprite* arrow, T_Scene* scene, vSpriteSet npc_set);		// ����ڵ��Ƿ����Ŀ��
};
