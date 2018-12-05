#include "rw4.h"

RW4::RW4() {

}
RW4::~RW4() {
	closeHandle();
}

void RW4::Init()
{
	hDriver = CreateFile(
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
		MessageBox(NULL, L"#��������ʧ��,����취����\n\n#��ͥ�û���رհ�ȫ���\n#������ɵ����ɷ���ǽ�ص�\n\n#������Ϸ���û�н�������������������Թ���Ա�������", NULL, NULL);
		exit(0);
	}
	dwProcessId = getProcessId();
	dwProcessBaseAddress = getProcessBaseAddress();
}

void RW4::closeHandle()
{
	if (hDriver != NULL)
	{
		CloseHandle(hDriver);
		hDriver = NULL;
		dwProcessId = 0;
		dwProcessBaseAddress = 0;
	}

}

DWORD RW4::getProcessId()
{
	ULONG pid;
	DeviceIoControl(hDriver, GET_PROCESS_ID, &pid, sizeof(pid), &pid, sizeof(pid), 0, 0);
	return pid;
}

DWORD RW4::getProcessBaseAddress()
{
	ULONG baseAddress;
	DeviceIoControl(hDriver, GET_PROCESS_BASE_ADDRESS, &baseAddress, sizeof(baseAddress), &baseAddress, sizeof(baseAddress), 0, 0);
	return baseAddress;
}

BOOL RW4::readVirtualMemory(ULONG Address, PVOID Response, SIZE_T Size)
{
	READ_VIRTUAL_MEMORY_STRUCT rvms;
	rvms.Response = Response;
	rvms.Address = Address;
	rvms.Size = Size;
	return DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);
}

BOOL RW4::writeVirtualMemory(ULONG Address, PVOID Value, SIZE_T Size)
{
	WRITE_VIRTUAL_MEMORY_STRUCT wvms;
	wvms.Address = Address;
	wvms.Value = Value;
	wvms.Size = Size;
	return DeviceIoControl(hDriver, WRITE_VIRTUAL_MEMORY, &wvms, sizeof(wvms), &wvms, sizeof(wvms), 0, 0);
}

