#pragma once
//static inline __declspec(naked) void window_memssage_proc()
//{
//	//__asm {
//	//	mov eax, [ebp + 0xc]
//	//	cmp eax,__MY_MESSAGE_ID
//	//	jne originalcode
//	//	pushad
//	//	pushfd
//	//	mov eax, [esp + 34]
//	//	call eax
//	//	popfd
//	//	popad
//	//	mov esp, ebp
//	//	pop ebp
//	//	ret
//	//	originalcode:
//	//		sub esp,0x4c
//	//		mov eax, [0x400500]
//	//		jmp returnhere
//	//	returnhere:
//	//		mov eax, __HOOK消息还原
//	//		jmp eax
//	//}
//	//// 标记用 
//	//__asm int 3
//	//__asm int 3
//	//__asm int 3
//	//__asm int 3
//	//__asm int 3
//}

static inline void hook_window_message()
{

	write_bytes(0x224D1036, { 0xE9, 0xC5, 0xEF, 0xBF, 0xE4, 0x90, 0x90, 0x90 });
}