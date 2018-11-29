#include "rw2.h"

RW2::RW2() {
	Init();
}
RW2::~RW2() {

}

void RW2::Init()
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
		MessageBox(NULL, L"#��������ʧ��,����취����\n\n#��ͥ�û���رհ�ȫ���\n#������ɵ����ɷ���ǽ�ص�\n#���»�ȡ���������󶨶���������\n#�Ѹ������÷�ϵͳ��Ŀ¼�¹���Ա�������\n#������Ϸ���û�н�������������������Թ���Ա������и���", NULL, NULL);
		exit(0);
	}
	dwProcessId = getProcessId();
	dwProcessBaseAddress = getProcessBaseAddress();
}

DWORD RW2::getProcessId()
{
	ULONG pid;
	DeviceIoControl(hDriver, GET_PROCESS_ID, &pid, sizeof(pid), &pid, sizeof(pid), 0, 0);
	return pid;
}

DWORD RW2::getProcessBaseAddress()
{
	ULONG baseAddress;
	DeviceIoControl(hDriver, GET_PROCESS_BASE_ADDRESS, &baseAddress, sizeof(baseAddress), &baseAddress, sizeof(baseAddress), 0, 0);
	return baseAddress;
}

BOOL RW2::readVirtualMemory(ULONG Address, PVOID Response, SIZE_T Size)
{
	READ_VIRTUAL_MEMORY_STRUCT rvms;
	rvms.Response = Response;
	rvms.Address = Address;
	rvms.Size = Size;
	return DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);
}

BOOL RW2::writeVirtualMemory(ULONG Address, PVOID Value, SIZE_T Size)
{
	WRITE_VIRTUAL_MEMORY_STRUCT wvms;
	wvms.Address = Address;
	wvms.Value = Value;
	wvms.Size = Size;
	return DeviceIoControl(hDriver, WRITE_VIRTUAL_MEMORY, &wvms, sizeof(wvms), &wvms, sizeof(wvms), 0, 0);
}

