#pragma once
#include "pch.h"
#include "call.h"
#include "function.h"

void call::����Call(int pointer,int code,int damage,int x,int y,int z)
{
	int skill_struct[26] = {};
	skill_struct[0] = pointer;//����ָ��;
	skill_struct[2] = code; //����
	skill_struct[3] = damage; //�˺�
	skill_struct[6] = x;//x
	skill_struct[7] = y;//y
	skill_struct[8] = z;//z
	skill_struct[22] = 0; //��С
	skill_struct[23] = 0;
	skill_struct[24] = 65535;
	skill_struct[25] = 65535;

	byte shell_code[] = {
		0xb9,0x00,0x00,0x00,0x00,
		0xb8,0x00,0x00,0x00,0x00,
		0xff,0xd0,
		0xc3
	};
	*(int*)(shell_code + 1) = __CALL����;
	*(int*)(shell_code + 6) = __����CALL;
	
	function::remoteMainThreadCall(shell_code,sizeof(shell_code),skill_struct,sizeof(skill_struct));

}

void call::�ͷ�Call(int pointer, int code, int damage, int x, int y, int z)
{
	byte shell_code[] = {
		0x68,0x00,0x00,0x00,0x00,
		0x68,0x00,0x00,0x00,0x00,
		0x68,0x00,0x00,0x00,0x00,
		0x68,0x00,0x00,0x00,0x00,
		0x68,0x00,0x00,0x00,0x00,
		0x68,0x00,0x00,0x00,0x00,
		0xBF,0x00,0x00,0x00,0x00,
		0x8B,0xC7,
		0xFF,0xD0,
		0x83,0xC4,0x24,
		0xC3,
	};
	*(int*)(shell_code + 1) = z;
	*(int*)(shell_code + 6) = y;
	*(int*)(shell_code + 11) = x;
	*(int*)(shell_code + 16) = damage;
	*(int*)(shell_code + 21) = code;
	*(int*)(shell_code + 26) = pointer;
	*(int*)(shell_code + 31) = __�ͷ�CALL;
	function::remoteMainThreadCall(shell_code, sizeof(shell_code));
}