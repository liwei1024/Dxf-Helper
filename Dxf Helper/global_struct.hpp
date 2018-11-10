#pragma once

struct Pos
{
	int x, y, z = 0;
};

struct COORDINATE
{
	int x, y, z = 0;
};

struct CITY_INFO
{
	int max_city_id, min_city_id, x, y = 0;
};

struct MAP_OBJECT_STRUCT
{
	DWORD address;
	std::wstring name;
	DWORD type;
	DWORD camp;
	DWORD health_point;
	DWORD code;
	int x;
	int y;
	int z;
};