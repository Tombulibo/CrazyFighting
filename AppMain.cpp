#include "AudioTest.h"

// WinMain函数
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"游戏声音模块应用";		//游戏窗口标题栏文字内容
	// 如果要设定窗口和exe文件的图标, 请将图标资源引入到工程中
	// 然后将图标的标识替换构造函数中的两个NULL值
	// 例如:如果图标标识为IDI_TANKNEW, 则两个NULL值都设为IDI_TANKNEW
	AudioTest* test = new AudioTest(h_instance, WIN_CLASS, WinTitle,
		NULL, NULL, WIN_WIDTH, WIN_HEIGHT);
	T_Engine::pEngine = test;	// 必须将父类的静态指针指向构造好的子类
	test->SetFrame(10);			// 设置游戏画面的刷新频率(可以调用FRAME_SPEED常量)
	// 如果程序要以全屏幕方式启动, 请添加下面这行代码
	// 如果以正常窗口启动, 请删除或注释掉下面这行代码
	// myApp->SetFullScreen(TRUE);
	test->StartEngine();		// 启动游戏引擎(显示游戏窗口并进入游戏主循环)
	return TRUE;
}