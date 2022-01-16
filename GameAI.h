#pragma once
#include ".\\TinyEngine\\T_AI.h"
#include "CrazyMan.h"


class GameAI:
	public T_AI
{
private:
	int dir_style;		// ����ģʽ����ֵ������4��8

	// ��ܷ���
	// ���ά��GAME_DIR��Ԫ�ر��һ��
	// �ڶ�ά��ʾ��Ӧ�Ķ�ܷ���
	static int EvadeDir[8][5];

	// �������spָ���Ľ�ɫǰ�������Ӧ����ײ�������
	// ����sp�������Ľ�ɫ����
	// ����spSizeTimes����ɫ��ߵı����������������ķ�Χ
	// ����testRec������Ͳ������õ����յļ������
	void GetCHitRect(IN CrazyMan* sp, IN int spSizeTimes, OUT RECT& testRec);

	// �����ƶ������ٶȺ�ָ���Ļ��Χ���㵱ǰ��ɫ���ƶ�λ��
	MoveCoord GetCMoveCoord(CrazyMan* npc_sp, RECT boundary);

public:
	GameAI(int d_style);	// 
	virtual ~GameAI(void);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �õ�һ��T_Sprite������Χ8������ľ�������
	// ����sp�������Ľ�ɫ����
	// ����spSizeTimes����ɫ��ߵı����������������ķ�Χ
	// ����testRec������Ͳ�����ָ��һ��8��Ԫ�ص�RECT���飬�õ����ս�ɫ������Χ8������ľ��μ������
	void GetCHitRects(IN CrazyMan* sp, IN float spSizeTimes, OUT RECT*& testRec);
	// ��ɫ��ܴ�����һ������Ϊ��ܵĶ��󣬵ڶ�������ΪҪ�رܵļ����󣬵���������Ϊ�߽緶Χ
	void Escape(CrazyMan* npc_sp, CrazyMan* player, RECT boundary);
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// �����ƶ������ٶȺ�ָ���Ļ��Χ�Խ�ɫ�����δ���
	void Wander(CrazyMan* npc_sp, RECT boundary);
	// �����ƶ������ٶȺ�ָ���ĵ�ͼ����Խ�ɫ�����δ���
	void Wander(CrazyMan* npc_sp, T_Map* map,int type);
	// ��ɫ��ܴ�����һ������Ϊ��ܵĶ��󣬵ڶ�������ΪҪ�رܵļ�����
	void Evade(CrazyMan* npc_sp, CrazyMan* player);
	// ��ɫ���ص�������һ������Ϊ�����󣬵ڶ�������Ϊ���������ڵĽ�ɫ����
	void CheckOverlay(CrazyMan* npc_sp, vector<CrazyMan*> vSpriteSet);

	void Chase(CrazyMan* npc_sp, CrazyMan* player, T_Map* map);
};

