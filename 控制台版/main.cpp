// 控制台版.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

HANDLE hDriver; // Handle to driver

DWORD GetTargetPid()
{
	if (hDriver == INVALID_HANDLE_VALUE)
		return false;

	ULONG Id;
	DWORD Bytes;

	if (DeviceIoControl(hDriver, IO_GET_PROCESS_ID, &Id, sizeof(Id),
		&Id, sizeof(Id), &Bytes, NULL))
		return Id;
	else
		return false;
}

template <typename type>
type ReadVirtualMemory(ULONG ProcessId, ULONG ReadAddress,
	SIZE_T Size)
{
	if (hDriver == INVALID_HANDLE_VALUE)
		return (type)false;

	DWORD Return, Bytes;
	READ_VIRTUAL_MEMORY ReadRequest;

	ReadRequest.ProcessId = ProcessId;
	ReadRequest.Address = ReadAddress;
	ReadRequest.Size = Size;

	// send code to our driver with the arguments
	if (DeviceIoControl(hDriver, IO_READ_VIRTUAL_MEMORY, &ReadRequest,sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
		return (type)ReadRequest.Response;
	else
		return (type)false;
}

bool WriteVirtualMemory(ULONG ProcessId, ULONG WriteAddress,
	ULONG WriteValue, SIZE_T WriteSize)
{
	if (hDriver == INVALID_HANDLE_VALUE)
		return false;
	DWORD Bytes;

	WRITE_VIRTUAL_MEMORY  WriteRequest;
	WriteRequest.ProcessId = ProcessId;
	WriteRequest.Address = WriteAddress;
	WriteRequest.Value = WriteValue;
	WriteRequest.Size = WriteSize;

	if (DeviceIoControl(hDriver, IO_WRITE_VIRTUAL_MEMORY, &WriteRequest, sizeof(WriteRequest),0, 0, &Bytes, NULL))
		return true;
	else
		return false;
}

int main()
{
	
	hDriver = CreateFileW(
		L"\\\\.\\" SYMBOLIC_LINK_SHORT_NAME,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		0,
		0
	);
	if (hDriver == INVALID_HANDLE_VALUE)
	{
		printf("驱动句柄创建失败\n");
	}
	else {
		DWORD ProcessId = GetTargetPid();
		printf("ProcessId %d\n", ProcessId);
		DWORD value = ReadVirtualMemory<DWORD>(ProcessId, 0x00AEB570, sizeof(ULONG));
		printf("value %d\n", value);
		WriteVirtualMemory(ProcessId, 0x00AEB570, 1000, 4);
		int i = 0;
		while (true)
		{
			value = ReadVirtualMemory<DWORD>(ProcessId, 0x00AEB570, sizeof(ULONG));
			printf("第%d次读写 value %d\n",i, value);
			i++;
		}
	}
	system("pause");
}


