// Dxf Helper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

HWND g_hWnd = NULL;
DWORD g_game_process_id = NULL;
DWORD g_game_main_thread_id = NULL;
HANDLE g_driver_handle = NULL;


int main()
{
	g_hWnd = FindWindow(L"地下城与勇士", L"地下城与勇士");

	g_game_main_thread_id = GetWindowThreadProcessId(g_hWnd, &g_game_process_id);

	g_driver_handle = load_driver(驱动路径,驱动名称);

	window_initialize();

	red_print("123132");

	system("pause");
	unload_driver(驱动名称);
}


