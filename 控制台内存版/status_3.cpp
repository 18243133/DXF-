#include "pch.h"
#ifndef __ASTAR_H__
#include "../astar/astar.h"
#pragma comment(lib,"../x64/Release/library/astar.lib")
#endif // !__ASTAR_H__
#include "status_3.h"



DWORD status_3::getMapAddress()
{

	return memory::readOffset<int>(__�����ַ, { __��ͼƫ�� });
}

DWORD status_3::getMapStartAddress()
{
	return  memory::read<int>(getMapAddress() + __�׵�ַ);
}

int status_3::getMapObjectCount(DWORD map_start_address)
{
	return  (memory::read<int>(getMapAddress() + __β��ַ) - map_start_address) / 4;
}

// ��ȡ������Ϣ
MAP_OBJECT_STRUCT status_3::get_object_info(DWORD object_pointer)
{
	MAP_OBJECT_STRUCT object;
	DWORD pos_pointer;
	object.address = object_pointer;
	object.type = memory::read<DWORD>(object_pointer + __����ƫ��);
	object.camp = memory::read<DWORD>(object_pointer + __��Ӫƫ��);
	object.health_point = memory::read<DWORD>(object_pointer + __Ѫ��ƫ��);
	object.code = memory::read<DWORD>(object_pointer + __����ƫ��);
	if (object.type == 289 && object.camp == 200)
	{
		object.name = memory::readWString(memory::read<DWORD>(memory::read<DWORD>(object_pointer + __������Ʒ����ƫ��) + 0x24) + 0, 100);
	}
	else {
		object.name = memory::readWString(memory::read<DWORD>(object_pointer + __����ƫ��), 100);
	}
	if (object.type == 273)
	{
		pos_pointer = memory::read<int>(object_pointer + __��������ƫ��);
		object.x = (int)memory::read<float>(__��ɫ���� + 0);
		object.y = (int)memory::read<float>(__��ɫ���� + 4);
		object.z = (int)memory::read<float>(__��ɫ���� + 8);
	}
	else {
		pos_pointer = memory::read<int>(object_pointer + __����ƫ��);
		object.x = (int)memory::read<float>(pos_pointer + 0x10);
		object.y = (int)memory::read<float>(pos_pointer + 0x14);
		object.z = (int)memory::read<float>(pos_pointer + 0x18);
	}
	return object;
}

// ��ӡ����������Ϣ
void status_3::outputMapObjectInfo()
{
	DWORD map_start_address = getMapStartAddress();
	utils::myprintf("map_start_address %x\n",RED, map_start_address);
	DWORD map_object_count = getMapObjectCount(map_start_address);
	utils::myprintf("map_object_count %d\n", RED, map_object_count);
	MAP_OBJECT_STRUCT _ObjectInfo;
	DWORD object_address;
	for (size_t i = 0; i < map_object_count; i++)
	{
		object_address = memory::read<int>(map_start_address + i * 4);
		if (object_address <= 0)continue;
		_ObjectInfo = get_object_info(object_address);
		utils::myprintf("=====================================");
		utils::myprintf("address 0x%x", RED, _ObjectInfo.address);
		utils::myprintf("code %d", RED, _ObjectInfo.code);
		utils::myprintf("type %d", RED, _ObjectInfo.type);
		utils::myprintf("camp %d", RED, _ObjectInfo.camp);
		utils::myprintf("health_point %d", RED, _ObjectInfo.health_point);
		utils::myprintf("pos %d,%d,%d", RED, _ObjectInfo.x, _ObjectInfo.y, _ObjectInfo.z);
		utils::myprintf("name %s", RED, _ObjectInfo.name.c_str());
	}
}