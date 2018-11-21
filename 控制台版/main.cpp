// 控制台版.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

int main()
{
	HANDLE hDriver; // Handle to driver
	hDriver = CreateFileW(L"\\\\.\\" SYMBOLIC_LINK_SHORT_NAME, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hDriver == INVALID_HANDLE_VALUE)
	{
		printf("驱动句柄创建失败\n");
	}
	else {
		KERNEL_READ_REQUEST ReadRequest;

		ReadRequest.ProcessId = 5684;
		ReadRequest.Address = 0x01506290;
		ReadRequest.Size = sizeof(int);

		// send code to our driver with the arguments
		if (DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
			printf("value %d\n", ReadRequest.Response);
		else
			printf("读取失败\n");

	}
	system("pause");
}


