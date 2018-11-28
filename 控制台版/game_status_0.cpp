#include "pch.h"
#include "game_status_0.h"
#include "memory.hpp"
#include "utils.h"
#include "function.h"


/*
1.�жϽ�ɫ�Ƿ�ˢ��
2.ѡ���ɫ
*/
void game_status_0::manage()
{
	if (get_role_count() == get_role_index()) {
		utils::myprintf("���н�ɫˢͼ��ϣ��Զ��ر�");
		g_auto_switch = false;
	}
	else {
		select_role();
	}

}

int game_status_0::get_role_index()
{
	int role_index = readOffset<int>(__����ȡֵ, { __��ɫ�±�ƫ�� });
	return role_index;

}

int game_status_0::get_role_count()
{
	int role_count = readOffset<int>(__����ȡֵ, { __��ǰ��ɫ����,__��ɫ����ƫ��2 });
	return role_count;
}

int game_status_0::get_role_max_count()
{
	int role_max_count = readOffset<int>(__����ȡֵ, { __����ɫ����,__��ɫ����ƫ��2 });
	return role_max_count;
}



void game_status_0::select_role()
{
	int select_role_index = g_role_index + 1;
	while (function::getGameStatus() == 0)
	{
		if (select_role_index != get_role_index())
		{
			utils::doKeyPress(VK_RIGHT);
		}
		utils::setMousePos(game_window_info.left + 534, game_window_info.top + 550);
		utils::mouseClick();
		Sleep(3000);
	}
	g_role_index = select_role_index;
}



void game_status_0::create_role()
{

}