#include "CrazyFighting.h"

// WinMain����
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"CrazyFighting";		//��Ϸ���ڱ�������������
	// ���Ҫ�趨���ں�exe�ļ���ͼ��, �뽫ͼ����Դ���뵽������
	// Ȼ��ͼ��ı�ʶ�滻���캯���е�����NULLֵ
	// ����:���ͼ���ʶΪIDI_TANKNEW, ������NULLֵ����ΪIDI_TANKNEW
	CrazyFighting* game = new CrazyFighting(h_instance, WIN_CLASS, WinTitle,
		NULL, NULL, WIN_WIDTH, WIN_HEIGHT);
	T_Engine::pEngine = game;	// ���뽫����ľ�ָ̬��ָ����õ�����
	game->SetFrame(70);			// ������Ϸ�����ˢ��Ƶ��(���Ե���FRAME_SPEED����)
	// �������Ҫ��ȫ��Ļ��ʽ����, ������������д���
	// �����������������, ��ɾ����ע�͵��������д���
	// myApp->SetFullScreen(TRUE);
	game->StartEngine();		// ������Ϸ����(��ʾ��Ϸ���ڲ�������Ϸ��ѭ��)
	return TRUE;
}