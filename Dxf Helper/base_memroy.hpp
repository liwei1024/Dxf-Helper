#pragma once

enum PROTO_MESSAGE
{
	PROTO_GET_Base = 0,
	PROTO_NORMAL_READ = 1,
	PROTO_NORMAL_WRITE = 2,
	PROTO_Protect_Process = 3,
	PROTO_UnProtect_Process = 4
};

typedef struct ReadStruct
{
	DWORD_PTR UserBufferAdress;
	DWORD_PTR GameAddressOffset;
	ULONGLONG ReadSize;
	ULONG UserPID;
	ULONG GamePID;
	BOOLEAN WriteOrRead;
	UINT32 ProtocolMsg;
} ReadStruct, *pReadStruct;

typedef struct WriteStruct
{
	DWORD_PTR UserBufferAdress;
	DWORD_PTR GameAddressOffset;
	ULONGLONG ReadSize;
	ULONG UserPID;
	ULONG GamePID;
	BOOLEAN WriteOrRead;
	UINT32 ProtocolMsg;
} WriteStruct, *pWriteStruct;


template<typename T> static inline T read(DWORD_PTR base_address)
{
	T buffer = T();
	ReadStruct rStruct{ (DWORD_PTR)&buffer, (DWORD_PTR)base_address, sizeof(buffer), (uint32_t)GetCurrentProcessId(), g_game_process_id, NULL, PROTO_NORMAL_READ };
	WriteFile(g_driver_handle, (LPCVOID)&rStruct, sizeof(ReadStruct), NULL, NULL);
	return buffer;
}

template<typename T> static inline bool write(DWORD_PTR base_address,T value)
{
	WriteStruct rStruct{ (DWORD_PTR)&value, (DWORD_PTR)base_address, sizeof(int), (uint32_t)GetCurrentProcessId(), g_game_process_id, NULL, PROTO_NORMAL_WRITE };
	return WriteFile(g_driver_handle, (LPCVOID)&rStruct, sizeof(ReadStruct), NULL, NULL);
}

template <typename T> static inline T read_offset(DWORD_PTR base_address, std::vector<int> offset)
{
	DWORD ofset_address = read<DWORD>(base_address);
	T value;
	for (size_t i = 0; i < offset.size(); i++)
	{
		if (ofset_address > 0)
		{
			if ((i + 1) < offset.size())
			{
				ofset_address = read<DWORD>(ofset_address + offset[i]);
			}
			else {
				value = read<T>(ofset_address + offset[i]);
			}
		}
	}
	return value;
}

template <typename T> static inline bool write_offset(DWORD_PTR base_address, std::vector<int> offset, T value)
{
	DWORD ofset_address = read<DWORD>(base_address);
	bool result = false;

	for (size_t i = 0; i < offset.size(); i++)
	{
		if (ofset_address)
		{
			if ((i + 1) < offset.size())
			{
				ofset_address = read<DWORD>(ofset_address + offset[i]);
			}
			else {
				result = write<T>(ofset_address + offset[i], value);
			}
		}
	}

	return result;
}

static inline std::vector<BYTE> read_bytes(DWORD_PTR base_address, size_t length)
{
	std::vector<BYTE> bytes;
	for (size_t i = 0; i < bytes.size(); i++)
	{
		bytes[i] = read<BYTE>(base_address + i);
	}
	return bytes;
}

static inline void write_bytes(DWORD_PTR base_address, std::vector<BYTE> bytes)
{
	for (size_t i = 0; i < bytes.size(); i++)
	{
		write<BYTE>(base_address + i, bytes[i]);
	}
}


static inline std::wstring  read_wstring(DWORD_PTR base_address, size_t length)
{
	wchar_t *buffer = new wchar_t[length];
	for (size_t i = 0; i < length; i++)
	{
		buffer[i] = read<wchar_t>(base_address + 1);
	}
	std::wstring str(buffer, length);
	delete[] buffer;
	return str;
}

static inline std::string read_string(DWORD_PTR base_address, size_t length)
{
	char *buffer = new char[length];
	for (size_t i = 0; i < length; i++)
	{
		buffer[i] = read<char>(base_address + 1);
	}
	std::string str(buffer, length);
	delete[] buffer;
	return str;
}

