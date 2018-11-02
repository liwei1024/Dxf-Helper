#pragma once

static int createRandom(int min, int max)
{
	int 随机数 = 0;
	if (max > min) {
		srand((unsigned)time(NULL));
		随机数 = (rand() % (min - max + 1) + max);
	}
	else {
		srand((unsigned)time(NULL));
		随机数 = (rand() % (max - min + 1) + min);
	}
	return 随机数;
}

// 获取时间戳
static DWORD getTime()
{
	DWORD t_start;
	t_start = GetTickCount();//从操作系统启动所经过（elapsed）的毫秒数，它的返回值是DWORD。
	return  t_start;
}

static POINT getMouseCoord()
{
	POINT Coord;
	if (GetCursorPos(&Coord)) {
		//printf("getMouseCoord Erro!\n");
	}
	return Coord;
}

static BOOL setMouseCoord(INT x, INT y)
{
	BOOL result;
	result = SetCursorPos(x, y);
	if (result == FALSE) {
		//printf("setMouseCoord Erro!\n");
	}
	return result;
}

static VOID mouseClick()
{
	MouseEvent(MOUSEEVENTF_LEFTDOWN or MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

static VOID mouseDoubleClick()
{
	MouseEvent(MOUSEEVENTF_LEFTDOWN or MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	MouseEvent(MOUSEEVENTF_LEFTDOWN or MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

static bool getKeyStatus(INT keyCode)
{
	if (GetKeyState(keyCode) < 0) {
		return TRUE;
	}
	return FALSE;
}

static INT getSCan(INT keyCode)
{
	INT sCan = MapVirtualKey(keyCode, 0);
	if (
		keyCode == VK_LEFT ||
		keyCode == VK_RIGHT ||
		keyCode == VK_DOWN ||
		keyCode == VK_UP
		)
	{
		sCan += 0x80;
	}
	return sCan;
}

static VOID keyDown(INT keyCode)
{
	KeybdEvent(keyCode, 0, 0, 0);
}

static VOID keyUp(INT keyCode)
{
	KeybdEvent(keyCode, 0, KEYEVENTF_KEYUP, 0);
}

static VOID doKeyPress(INT keyCode, INT s = 0)
{
	keyDown(keyCode);
	Sleep(s + createRandom(1, 10));
	keyUp(keyCode);
}

static void main_thread_exec_call(LPVOID pfun, std::vector<int> params = { 0 })
{
	DWORD_PTR *call_params = new DWORD_PTR[params.size()];
	for (size_t i = 0; i < params.size(); i++)
	{
		call_params[i] = params[i];
	}
	//SendMessage(g_hWnd, __MY_MESSAGE_ID, (WPARAM)pfun, (LPARAM)call_params);
	delete[]call_params;
}

// 青色
static inline void cyan_print(const char *内容, ...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);//设置绿色和蓝色相加
	va_list argList;
	char buffer[0x1024];
	va_start(argList, 内容);
	vsprintf_s(buffer, 内容, argList);
	printf("%s\n", buffer);
	va_end(argList);
}
//黄色
static inline void yellow_print(const char *内容, ...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);//设置红色和绿色相加

	va_list argList;
	char buffer[0x1024];
	va_start(argList, 内容);
	vsprintf_s(buffer, 内容, argList);
	printf("%s\n", buffer);
	va_end(argList);
}
//粉色
static inline void pink_print(const char *内容, ...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);//设置红色和蓝色相加
	va_list argList;
	char buffer[0x1024];
	va_start(argList, 内容);
	vsprintf_s(buffer, 内容, argList);
	printf("%s\n", buffer);
	va_end(argList);
}
//红色
static inline void red_print(const char *内容, ...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);//设置红色
	va_list argList;
	char buffer[0x1024];
	va_start(argList, 内容);
	vsprintf_s(buffer, 内容, argList);
	printf("%s\n", buffer);
	va_end(argList);
}
// 绿色
static inline void green_print(const char *内容, ...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);//设置绿色
	va_list argList;
	char buffer[0x1024];
	va_start(argList, 内容);
	vsprintf_s(buffer, 内容, argList);
	printf("%s\n", buffer);
	va_end(argList);
}

static inline void window_initialize()
{
	RECT rect;
	int cx, cy;
	int width, height;
	cx = GetSystemMetrics(SM_CXFULLSCREEN);
	cy = GetSystemMetrics(SM_CYFULLSCREEN);
	HWND g_self_window_handle = GetConsoleWindow();
	GetWindowRect(g_self_window_handle, &rect);
	MoveWindow(g_self_window_handle, 0, 0, 800, 600, TRUE);
	GetWindowRect(g_self_window_handle, &rect);
	width = (rect.right - rect.left);
	height = (rect.bottom - rect.top);
	MoveWindow(g_self_window_handle, cx - width, cy - height, width, height, TRUE);

	EnableMenuItem(GetSystemMenu(g_self_window_handle, FALSE), SC_CLOSE, MF_GRAYED);

	SetConsoleTitle(_T("丶"));
	//SetLayeredWindowAttributes(g_self_window_handle, 0, 200, 3);//透明度设置
}

static inline void unload_driver(const char *driver_name)
{
	std::string 停止命令(driver_name);
	std::string 卸载命令(driver_name);
	停止命令 = "sc.exe stop " + 停止命令;
	卸载命令 = "sc.exe delete " + 卸载命令;

	WinExec(停止命令.c_str(), SW_HIDE);
	WinExec(卸载命令.c_str(), SW_HIDE);
}

static inline HANDLE load_driver(const char *path, const char *derver_name)
{
	DWORD dwRtn;
	BOOL bRet = FALSE;
	SC_HANDLE schSCManager = NULL;//SCM管理器的句柄
	SC_HANDLE schService = NULL;//NT驱动程序的服务句柄
	HANDLE driver_handle = NULL;

	std::string 驱动服务名(derver_name);
	驱动服务名 = "\\\\.\\" + 驱动服务名;

	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (schSCManager == NULL)
	{
		red_print("OpenSCManager() Faild %d ! \n", GetLastError());
		system("pause");
		exit(0);
	}
	schService = CreateServiceA(
		schSCManager,
		derver_name,
		derver_name,
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		path,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);
	if (schService == NULL) {
		dwRtn = GetLastError();
		if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)
		{
			//由于其他原因创建服务失败
			red_print("由于其他原因创建服务失败 %d ! \n", dwRtn);
			unload_driver(derver_name);
			system("pause");
			exit(0);
		}

		// 驱动程序已经加载，只需要打开 
		schService = OpenServiceA(schSCManager, derver_name, SERVICE_ALL_ACCESS);
		if (schService == NULL)
		{
			//如果打开服务也失败，则意味错误
			red_print("如果打开服务也失败，则意味错误 ! \n");
			system("pause");
			exit(0);
		}
	}
	bRet = StartService(schService, NULL, NULL);
	if (!bRet)
	{
		DWORD dwRtn = GetLastError();
		if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING)
		{
			red_print("StartService() Faild %d ! \n", dwRtn);
			system("pause");
			exit(0);
		}
		else
		{
			if (dwRtn == ERROR_IO_PENDING)
			{
				//设备被挂住
				red_print("StartService() Faild ERROR_IO_PENDING ! \n");
				system("pause");
				exit(0);
			}
			
		}
	}
	CloseServiceHandle(schService);

	//red_print("%ws", 驱动服务名.c_str());

	driver_handle = CreateFileA(
		驱动服务名.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		0,
		0
	);

	if (driver_handle == INVALID_HANDLE_VALUE)
	{
		red_print("驱动句柄创建失败 hDriver %p Error Code <%d>\n ", driver_handle, GetLastError());
	}

	return driver_handle;
}

static inline void file_protect(const char * file_path,bool toggle)
{
	std::string str(file_path);
	if (toggle = true)
	{
		str = "cacls \"" + str + "\" /e /p everyone:n ";
	}
	else {
		str = "cacls \"" + str + "\" /e /p everyone:f ";
	}
	WinExec(str.c_str(), SW_HIDE);
}

static inline bool EnableDebugPrivilege(bool bEnable)
{
	//VMProtectBeginUltra("EnableDebugPrivilege");
	//Enabling the debug privilege allows the application to see 
	//information about service application
	BOOL fOK = false;     //Assume function fails
	HANDLE hToken;
	//Try to open this process's acess token
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		//Attempt to modify the "Debug" privilege
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		fOK = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	//VMProtectEnd();
	return fOK;
}

static inline bool delete_self_file()
{
	//VMProtectBeginUltra("删除自身");
	bool result = true;
	wchar_t FileName[MAX_PATH];
	memset(FileName, 0, MAX_PATH);
	//获取文件路径
	GetModuleFileName(NULL, FileName, MAX_PATH);
	wchar_t *NewFileName;
	NewFileName = new wchar_t[MAX_PATH];
	memset(NewFileName, 0, MAX_PATH);
	//设置文件属性
	SetFileAttributes(NewFileName, FILE_ATTRIBUTE_NORMAL);
	//尝试直接删除
	if (DeleteFile(FileName))//能直接删掉最好
	{
		delete[] NewFileName;
		return false;
	}
	//以下API一样，不再啰嗦
	wsprintf(NewFileName, _T("C:\\Windows\\%C\0"), FileName[0]);
	CreateDirectory(NewFileName, NULL);
	SetFileAttributes(NewFileName, FILE_ATTRIBUTE_HIDDEN);
	wsprintf(NewFileName, _T("C:\\Windows 服务主进程\0"), FileName[0], GetTickCount());
	SetFileAttributes(NewFileName, FILE_ATTRIBUTE_NORMAL);
	DeleteFile(NewFileName);
	if (!MoveFileEx(FileName, NewFileName, MOVEFILE_REPLACE_EXISTING))
	{
		delete[] NewFileName;
		//printf("删除自身失败 Error Code\n");
		return false;//动不了就放弃
	}
	MoveFileEx(NewFileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	SetFileAttributes(NewFileName, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
	delete[] NewFileName;
	if (result == false)
	{
		printf("删除自身失败 Error Code\n");
		system("pause");
		exit(0);
	}
	//VMProtectEnd();
	return result;
}