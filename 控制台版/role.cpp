#include "pch.h"
#include "role.h"
#include "function.h"

int role::getRoleLevel()
{
	return read<int>(__��ɫ�ȼ�);
}
std::wstring role::getRoleJobName()
{
	return readWString(read<DWORD>(__ְҵ����) + 0x0, 100);
}
int role::getCurrentRoleFatigueValue()
{
	return function::decrypt(__���ƣ��) - function::decrypt(__��ǰƣ��);
}
ROLE_POS role::getRolePos()
{
	ROLE_POS RolePos;
	RolePos.room = function::getCurrentRoomPos();
	RolePos.x = (int)read<float>(__��ɫ����);
	RolePos.y = (int)read<float>(__��ɫ���� + 4);
	RolePos.z = (int)read<float>(__��ɫ���� + 8);
	return RolePos;
}
int role::getRoleStatus()
{
	return read<int>(read<int>(__��ɫ״̬) + __��ɫ״̬ƫ��);
}
void role::releaseSkillByKey(int keyCode, int s)
{
	if (function::isOpenDoor() == false)
	{
		utils::doKeyPress(keyCode,s);
		// �ͷż��ܵ���ʱ
		while (getRoleStatus() != 0)
		{
			Sleep(100);
		}
	}
}