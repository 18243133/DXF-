#include "pch.h"
#include "function.h"



void function::remoteMainThreadCall(byte * shell_code,size_t shell_code_size,PVOID param,size_t paramSize)
{
	if (param > 0 && paramSize>0)
	{
		rw2.writeVirtualMemory(__CALL����, param, paramSize);
	}
	rw2.writeVirtualMemory(__CALL��ַ, shell_code, shell_code_size);
	SendMessage(HWND_BROADCAST, MY_MESSAGE_ID, __CALL��ַ, 0);
}