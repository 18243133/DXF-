#include "pch.h"
#include "status_0.h"
#include "function.h"

void status_0::manage()
{
	if (getRoleCount() == getRoleIndox()) {
		utils::printString("���н�ɫˢͼ��ϣ��Զ��ر�");
		autoSwitch = false;
	}
	else {
		selectRole(getRoleIndox() + 1);
	}
}

int status_0::getRoleIndox()
{
	return memory::readOffset<int>(__����ȡֵ, { __��ɫ�±�ƫ�� });
}
int status_0::getRoleCount()
{
	return memory::readOffset<int>(__����ȡֵ, { __��ǰ��ɫ����,__��ɫ����ƫ��2 });
}

int status_0::getRoleMaxCount()
{
	return memory::readOffset<int>(__����ȡֵ, { __����ɫ����,__��ɫ����ƫ��2 });
}

void status_0::selectRole(int select_role_index)
{
	while (function::getGameStatus() == 0)
	{
		if (select_role_index != getRoleIndox())
		{
			msdk.doKeyPress(VK_RIGHT);
		}
		msdk.setMousePos(game_window_info.left + 534, game_window_info.top + 550);
		msdk.mouseClick();
		Sleep(3000);
	}
}