#pragma once

static int decrypt(int address)
{
	int eax, esi, edx, i;
	eax = read<int>(address);
	esi = read<int>(__解密基址);
	edx = eax >> 16;
	edx = read<int>(esi + edx * 4 + 36);
	eax = eax & 65535;
	eax = read<int>(edx + eax * 4 + 8468);
	edx = eax & 65535;
	esi = edx << 16;
	esi = esi | edx;
	i = read<int>(address + 4);
	esi = esi ^ i;
	return esi;
}

//加密
static void encrypt(INT32 Address, INT32 Value)
{
	INT32 EncryptId = 0;
	INT32 OffsetParam = 0;
	INT32 OffsetAddress = 0;
	INT32 Data = 0;
	INT32 AddressMask = 0;
	INT16 ax = 0;
	INT16 si = 0;
	EncryptId = read<int>(Address);
	OffsetParam = read<int>(read<int>(__解密基址) + (EncryptId >> 16) * 4 + 36);
	OffsetAddress = OffsetParam + (EncryptId & 0xFFFF) * 4 + 8468;
	OffsetParam = read<int>(OffsetAddress);
	Data = OffsetParam & 0xFFFF;
	Data += Data << 16;
	ax = OffsetParam & 0xFFFF;
	AddressMask = Address & 0xF;
	if (AddressMask == 0x0)
	{
		si = Value >> 16;
		si -= ax;
		si += Value;
	}
	else if (AddressMask == 0x4)
	{
		si = (Value & 0xFFFF) - (Value >> 16);
	}
	else if (AddressMask == 0x8)
	{
		si = Value >> 16;
		si *= Value;
	}
	else if (AddressMask == 0xC)
	{
		si = Value >> 16;
		si += Value;
		si += ax;
	}
	else
	{
		return;
	}
	ax ^= si;
	write<BYTE>(OffsetAddress + 2, (BYTE)ax);
	write<BYTE>(OffsetAddress + 3, (BYTE)(ax >> 8));
	write<int>(Address + 4, Data ^ Value);
}

static int get_game_status()
{
	//output_bebug_wstring(L"__游戏状态 %d", read<int>(__游戏状态));
	return read<int>(__游戏状态);
}

//static void bulletin(LPCWSTR Format, ...)
//{
//	WCHAR Buffer[0x1000];
//
//	va_list args;
//	va_start(args, Format);
//	vswprintf_s(Buffer, Format, args);
//	va_end(args);
//
//	main_thread_exec_call(Call_公告Call, { (int)&Buffer });
//}

static Pos get_current_room_pos()
{
	Pos CurrentRoomPos;
	if (get_game_status() == 1)
	{
		CurrentRoomPos.x = read_offset<int>(__遍历取值, { __大区域偏移 });
		CurrentRoomPos.y = read_offset<int>(__遍历取值, { __小区域偏移 });
	}
	else {
		DWORD OffsetAddress = read_offset<int>(__房间编号, { __时间基址 ,__门型偏移 });
		CurrentRoomPos.x = read<int>(OffsetAddress + __当前房间X);
		CurrentRoomPos.y = read<int>(OffsetAddress + __当前房间Y);
	}

	return CurrentRoomPos;
}

static Pos get_boss_room_pos()
{
	Pos BossRoomPos;
	DWORD OffsetAddress = read_offset<int>(__房间编号, { __时间基址 ,__门型偏移 });
	BossRoomPos.x = decrypt(OffsetAddress + __BOSS房间X);
	BossRoomPos.y = decrypt(OffsetAddress + __BOSS房间Y);
	return BossRoomPos;
}

static bool is_boss_room()
{
	Pos current_room_pos;
	Pos boss_room_pos;

	current_room_pos = get_current_room_pos();
	boss_room_pos = get_boss_room_pos();

	if (current_room_pos.x == boss_room_pos.x && current_room_pos.y == boss_room_pos.y)
	{
		//bulletin(L"在Boss 房间");
		return true;
	}
	//bulletin(L"不Boss 房间");
	return false;
}

static bool is_open_door()
{
	if (decrypt(read_offset<int>(__人物基址, { __地图偏移 }) + __开门偏移) == 0)
	{
		//bulletin(L"已开门");
		return true;
	}
	else {
		//bulletin(L"未开门");
		return false;
	}
}

static ULONG 取技能Atk目录(ULONG Obj路径文本)
{
	/*ULONG  副本信息基址 = __副本信息;
	ULONG  加载_CALL = __加载CALL;
	ULONG  分割_CALL = __分割CALL;
	ULONG  Atk路径_基址 = __路径基址;
	ULONG  Atk路径_Call = __加载CALL;
	ULONG  Atk目录 = 0;
	__asm
	{
		mov ecx, 副本信息基址
		mov ecx, [ecx]
		push 1
		push Obj路径文本
		call 加载_CALL
		test eax, eax
		je Label1
		mov edi, eax
		lea esi, [edi + 0xC8]
		lea eax, [edi + 0x108]
		push esi
		push eax
		lea edx, 副本信息基址
		push edx
		mov[edx], 0
		call 分割_CALL
		add esp, 0xC
		mov eax, [eax]
		mov ecx, Atk路径_基址
		mov ecx, [ecx]
		push 1
		push eax
		call Atk路径_Call
		Label1 :
		mov Atk目录, eax
	}
	return Atk目录;*/
}

static ULONG 取文本路径(ULONG ID)
{
	/*ULONG ebp_4 = __目录基址;
	ULONG ebp_8 = __路径CALL;

	_asm push ID
	_asm mov ecx, ebp_4
	_asm call ebp_8
	_asm mov ebp_4, eax

	return ebp_4;*/
}

static void 置入技能伤害(ULONG 技能代码, ULONG 技能伤害)
{
	/*ULONG Atk;
	ULONG Obj路径;
	if (技能代码 != 11520 && 技能代码 != 39002 && 技能代码 != 54141 && 技能代码 != 70119 && 技能代码 != 11305 && 技能代码 != 14098 && 技能代码 != 11431 && 技能代码 != 11506 && 技能代码 != 70032 && 技能代码 != 54106 && 技能代码 != 61057 && 技能代码 != 56609 && 技能代码 != 100010 && 技能代码 != 0)
	{
		Obj路径 = 取文本路径(技能代码);
		if (Obj路径 > 0)
		{
			Atk = 取技能Atk目录(Obj路径);
			if (Atk > 0)
			{
				encrypt(Atk + 32, 技能伤害);
			}
		}
	}*/
}