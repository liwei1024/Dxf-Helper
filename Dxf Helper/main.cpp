// Dxf Helper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "game_status_0.h"
#include "game_status_1.h"
#include "game_status_2.h"
#include "game_status_3.h"

// 变量
HWND g_game_window_handle = NULL;
DWORD g_game_process_id = NULL;
DWORD g_game_main_thread_id = NULL;
HANDLE g_driver_handle = NULL;
std::string g_self_file_path;
HHOOK g_hHook;
DWORD g_self_main_thread_id;

game_status_0 _game_status_0;
game_status_1 _game_status_1;
game_status_2 _game_status_2;
game_status_3 _game_status_3;

//函数
void game_initialize();
void self_initialize();
LRESULT CALLBACK Keypress(int nCode, WPARAM wParam, LPARAM lParam);
VOID ThreadMessage(UINT message, WPARAM wParam, LPARAM lParam);
void ProcessExit();







void test()
{
	/*Map _Map;
	_Map.OutputMapObjectsInfo();*/

	//ExecCALL(Asm_选择角色,1,1);



	//_Game.OutputTaskInfo(0);

	//Knapsac _Knapsac;
	//_Knapsac.OutputGoodsInfo();
	//_Knapsac.SaleGoods();

}










int main()
{
	delete_self_file();

	self_initialize();

	game_initialize();


	char self_file_path[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, self_file_path);
	g_self_file_path = self_file_path;

	/*file_protect((g_self_file_path + "\\Dxf Helper.exe").c_str(), true);
	file_protect((g_self_file_path + "\\KeyCall.dll").c_str(), true);
	file_protect((g_self_file_path + "\\RW.sys").c_str(), true);*/

	write<int>(0x400500, 1000);
	printf("%d\n", read<int>(0x400500));
	
	
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
	g_driver_handle = load_driver(驱动路径, 驱动名称);
	if (!g_driver_handle)
	{
		red_print("驱动加载失败");
		system("pause");
		exit(0);
	}
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
}

void self_initialize() 
{
	unload_driver(驱动名称);
	window_initialize();

	g_self_main_thread_id = GetCurrentThreadId();

	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, Keypress, GetModuleHandle(NULL), NULL);

	if (!g_hHook)
	{
		red_print("键盘钩子安装失败");
		system("pause");
		exit(0);
	}
}

VOID ThreadMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
	cyan_print("message %d", message);
	cyan_print("wParam %d", wParam);
	cyan_print("lParam %d", lParam);
}

LRESULT CALLBACK Keypress(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* p = (CWPSTRUCT*)lParam;
	if (nCode == HC_ACTION)
	{
		switch (wParam) //wParam中是消息ID
		{
			// 键盘消息
		case WM_KEYDOWN:
		{
			switch (p->lParam)
			{
			case VK_HOME:
				PostThreadMessage(g_self_main_thread_id, 10024, 0, 0);
				//_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)test, NULL, NULL, NULL);
				//_game_status_3.output_map_objects_info();
				hook_window_message();
				printf("%d\n", read<int>(0x400500));
				write<int>(0x400500, 100);
				break;
			case VK_END:
				ProcessExit();//进程退出
				break;
			default:
				break;
			}
		}
		}
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

void ProcessExit()
{
	UnhookWindowsHookEx(g_hHook);
	/*file_protect((g_self_file_path + "\\Dxf Helper.exe").c_str(), false);
	file_protect((g_self_file_path + "\\KeyCall.dll").c_str(), false);
	file_protect((g_self_file_path + "\\RW.sys").c_str(), false);*/
	unload_driver(驱动名称);
	exit(0);
}