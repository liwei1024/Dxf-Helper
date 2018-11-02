// Dxf Helper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "base_message.hpp"
HWND g_game_window_handle = NULL;
DWORD g_game_process_id = NULL;
DWORD g_game_main_thread_id = NULL;
HANDLE g_driver_handle = NULL;
std::string g_self_file_path;

void game_initialize();
void self_initialize();



int main()
{
	game_initialize();

	self_initialize();

	/*char self_file_path[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, self_file_path);
	g_self_file_path = self_file_path;

	file_protect((g_self_file_path + "\\Dxf Helper.exe").c_str(), true);
	file_protect((g_self_file_path + "\\KeyCall.dll").c_str(), true);
	file_protect((g_self_file_path + "\\RW.sys").c_str(), true);*/
	
	MSG msg;//消息指针
	while (GetMessage(&msg, NULL, 0, 0))
	{
		ThreadMessage(msg.message, msg.wParam, msg.lParam);//线程消息处理
		TranslateMessage(&msg);//等待信号
		DispatchMessage(&msg);//处理信号
	}
}





void game_initialize()
{
	
	while (!g_game_window_handle)
	{
		g_game_window_handle = FindWindow(L"地下城与勇士", L"地下城与勇士");
		red_print("等待窗口...");
		Sleep(1000);
	}

	g_game_main_thread_id = GetWindowThreadProcessId(g_game_window_handle, &g_game_process_id);
	if (!g_game_main_thread_id)
	{
		red_print("游戏ID获取失败");
		system("pause");
		exit(0);
	}

	g_driver_handle = load_driver(驱动路径, 驱动名称);
	if (!g_driver_handle)
	{
		red_print("驱动加载失败");
		system("pause");
		exit(0);
	}
}

void self_initialize() 
{
	window_initialize();

	if (!SetGlobalKeyboardHook()) 
	{
		red_print("键盘钩子安装失败");
		system("pause");
		exit(0);
	}
}