#pragma once
#include "pch.h"
#include "call.h"
#include "function.h"
/*
void Send_����Call(DWORD cdov, DWORD len)
{
//DWORD parameter[] = { 100, �����ַ, cdov, len, ����CALL };
//���Զ��ִ��(Send_����, parameter, sizeof(parameter));
char Code[] = { 0x51,                       //push ecx
0x68, 0xFF, 0x00, 0x00, 0x00,      //push len
0x68, 0xFF, 0x00, 0x00, 0x00,      //push cdov
0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00,//mov ecx, dword ptr  [������ַ]
0xE8, 0x5F, 0x1C, 0x06, 0x00,      //call ����CALL
0x59,  0xC3 };                        //  pop ecx ret 
LPVOID CodeAddr = VirtualAllocEx(hProcess, 0, sizeof(Code), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

*(int *)(Code + 2) = len;
*(int *)(Code + 7) = cdov;
*(int *)(Code + 13) = ������ַ;
*(int *)(Code + 18) = ����CALL - ((int)CodeAddr + 17) - 5;
WriteProcessMemory(hProcess, CodeAddr, &Code, sizeof(Code), NULL);
//����log(L"Send_����Call%X", CodeAddr);
FARPROC  CallWindowProcA_addr = ::GetProcAddress(GetModuleHandleA("user32.dll"), "CallWindowProcW");
HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)CallWindowProcA_addr, CodeAddr, 0, 0);
//�ȴ��߳̽���
WaitForSingleObject(hRemoteThread, 0xFF);
// ������
VirtualFreeEx(hProcess, CodeAddr, sizeof(Code), MEM_DECOMMIT);
CloseHandle(hRemoteThread);

}


*/



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

/*
lw_test - 68 00010000           - push 00000100 { 256 }
01830005- 68 00010000           - push 00000100 { 256 }
0183000A- 68 00010000           - push 00000100 { 256 }
0183000F- 68 00010000           - push 00000100 { 256 }
01830014- 68 00020000           - push 00000200 { 512 }
01830019- 68 00020000           - push 00000200 { 512 }
0183001E- BF 00084000           - mov edi,00400800 { 4196352 }
01830023- 8B C7                 - mov eax,edi
01830025- FF D0                 - call eax
01830027- 83 C4 24              - add esp,24 { 36 }
0183002A- C3                    - ret

*/
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