#pragma once
#include ".\\TinyEngine\\T_AI.h"
#include "CrazyMan.h"


class GameAI:
	public T_AI
{
private:
	int dir_style;		// 方向模式，其值必须是4或8

	// 躲避方向
	// 最高维与GAME_DIR的元素编号一致
	// 第二维表示对应的躲避方向
	static int EvadeDir[8][5];

	// 计算参数sp指定的角色前进方向对应的碰撞检测区域
	// 参数sp：待检测的角色对象
	// 参数spSizeTimes：角色宽高的倍数，决定检测区域的范围
	// 参数testRec：输出型参数，得到最终的检测区域
	void GetCHitRect(IN CrazyMan* sp, IN int spSizeTimes, OUT RECT& testRec);

	// 根据移动方向、速度和指定的活动范围计算当前角色的移动位置
	MoveCoord GetCMoveCoord(CrazyMan* npc_sp, RECT boundary);

public:
	GameAI(int d_style);	// 
	virtual ~GameAI(void);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 得到一个T_Sprite对象周围8个延伸的矩形区域
	// 参数sp：待检测的角色对象
	// 参数spSizeTimes：角色宽高的倍数，决定检测区域的范围
	// 参数testRec：输出型参数，指向一个8个元素的RECT数组，得到最终角色对象周围8个延伸的矩形检测区域
	void GetCHitRects(IN CrazyMan* sp, IN float spSizeTimes, OUT RECT*& testRec);
	// 角色躲避处理，第一个参数为躲避的对象，第二个参数为要回避的检测对象，第三个参数为边界范围
	void Escape(CrazyMan* npc_sp, CrazyMan* player, RECT boundary);
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// 根据移动方向、速度和指定的活动范围对角色做漫游处理
	void Wander(CrazyMan* npc_sp, RECT boundary);
	// 根据移动方向、速度和指定的地图对象对角色做漫游处理
	void Wander(CrazyMan* npc_sp, T_Map* map,int type);
	// 角色躲避处理，第一个参数为躲避的对象，第二个参数为要回避的检测对象
	void Evade(CrazyMan* npc_sp, CrazyMan* player);
	// 角色放重叠处理，第一个参数为检测对象，第二个参数为检测对象所在的角色集合
	void CheckOverlay(CrazyMan* npc_sp, vector<CrazyMan*> vSpriteSet);

	void Chase(CrazyMan* npc_sp, CrazyMan* player, T_Map* map);
};

