#pragma comment(lib,"KeyCall.lib") 
extern "C" _declspec(dllimport) int _stdcall KeySendChar(const char * AData);
extern "C" _declspec(dllimport) int _stdcall MouseDown(BYTE AKey);
extern "C" _declspec(dllimport) int _stdcall MouseMove(BYTE AKey, int AX, int AY);
extern "C" _declspec(dllimport) int _stdcall MouseMoveTo(BYTE AKey, int AX, int AY);
extern "C" _declspec(dllimport) int _stdcall MouseMoveToEx(BYTE AKey, int AX, int AY);
extern "C" _declspec(dllimport) int _stdcall MouseClick(BYTE AKey);
extern "C" _declspec(dllimport) int _stdcall MouseDbClick(BYTE AKey);
extern "C" _declspec(dllimport) int _stdcall KeyDown(BYTE KeyCtrl, const char * AData);
extern "C" _declspec(dllimport) int _stdcall KeyDownEx(const char * AData);
extern "C" _declspec(dllimport) int _stdcall KeyUp(void);
extern "C" _declspec(dllimport) int _stdcall KeyDownUp(BYTE KeyCtrl, const char * AData);
extern "C" _declspec(dllimport) int _stdcall KeyDownUpEx(const char * AData);
extern "C" _declspec(dllimport) int _stdcall MouseKeyDownEx(const char * AData);
extern "C" _declspec(dllimport) int _stdcall MouseKeyDownUpEx(const char * AData);
extern "C" _declspec(dllimport) int _stdcall GetKeyDev(void);
extern "C" _declspec(dllimport) int _stdcall KeybdEvent(BYTE bVk, BYTE bScan, int dwFlags, int dwExtraInfo);
extern "C" _declspec(dllimport) int _stdcall MouseEvent(int dwFlags, int dx, int dy, int dwData, int dwExtraInfo);
extern "C" _declspec(dllimport) int _stdcall MouseRoll(BYTE AKey, BYTE ARoll);
