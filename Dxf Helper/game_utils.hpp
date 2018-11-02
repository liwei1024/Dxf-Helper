#pragma once

static inline std::vector<byte> get_call_bytes(LPVOID call_address)
{
	size_t call_size = 0;
	std::vector<byte> call_bytes;
	while (true)
	{
		if (
			*(byte*)((DWORD)call_address + (call_size + 0)) == 0xcc &&
			*(byte*)((DWORD)call_address + (call_size + 1)) == 0xcc &&
			*(byte*)((DWORD)call_address + (call_size + 2)) == 0xcc &&
			*(byte*)((DWORD)call_address + (call_size + 3)) == 0xcc &&
			*(byte*)((DWORD)call_address + (call_size + 4)) == 0xcc)
		{
			break;
		}
		else {
			call_bytes[call_size] = *(byte*)((DWORD)call_address + call_size);
		}
		call_size++;
	}
}