#pragma once


HHOOK g_hHook;
DWORD g_self_main_thread_id;

VOID ThreadMessage(
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	cyan_print("message %d", message);
	cyan_print("wParam %d", wParam);
	cyan_print("lParam %d", lParam);
}

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
				_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)test, NULL, NULL, NULL);
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


HHOOK SetGlobalKeyboardHook()
{
	g_self_main_thread_id = GetCurrentThreadId();
	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, Keypress, GetModuleHandle(NULL), NULL);
	return g_hHook;
}


void ProcessExit()
{
	//OutputDebugString("111111111111233333333333333");
	//protect_file();
	UnhookWindowsHookEx(g_hHook);
	/*file_protect((g_self_file_path + "\\Dxf Helper.exe").c_str(), false);
	file_protect((g_self_file_path + "\\KeyCall.dll").c_str(), false);
	file_protect((g_self_file_path + "\\RW.sys").c_str(), false);*/
	//unload_driver(驱动名称);
	exit(0);
}