#pragma once
class game_status_3
{
public:
	game_status_3();
	~game_status_3();
	bool door_whether_open();
	bool room_whether_boss();
	bool room_whether_abyss();
	bool campfire_whether_exist();
	bool monster_whether_exist();
	MAP_OBJECT_STRUCT get_object_info(DWORD object_pointer);
	void output_map_objects_info();
	DWORD get_map_address();
	DWORD get_map_start_address();
	int get_map_object_count(DWORD map_start_address);
	void move_to_next_room(int direction);
	void sort_by_distance(std::vector<MAP_OBJECT_STRUCT>& Objects);
	void attack_monster();
	void follow();
	bool get_the_custom_shop();
};

