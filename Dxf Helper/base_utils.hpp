#pragma once


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

	SetConsoleTitle(_T(""));
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

