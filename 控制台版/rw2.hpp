#pragma once
#include "../RW2/rw2api.h"

class RW2
{
private:
	HANDLE hDriver;
public:
	DWORD dwProcessId;
	DWORD_PTR dwProcessBaseAddress;
	void Init()
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
		printf("dwProcessId %d\n", dwProcessId);
		dwProcessBaseAddress = getProcessBaseAddress();
		printf("dwProcessBaseAddress %llx\n", dwProcessBaseAddress);
	}
	
	DWORD getProcessId()
	{
		ULONG pid;
		DeviceIoControl(hDriver, GET_PROCESS_ID, &pid, sizeof(pid), &pid, sizeof(pid), 0, 0);
		return pid;
	}

	DWORD_PTR getProcessBaseAddress()
	{
		ULONG baseAddress;
		DeviceIoControl(hDriver, GET_PROCESS_BASE_ADDRESS, &baseAddress, sizeof(baseAddress), &baseAddress, sizeof(baseAddress), 0, 0);
		return baseAddress;
	}

	template<typename T>
	T rvm(DWORD_PTR dwBaseAddress)
	{
		READ_VIRTUAL_MEMORY_STRUCT rvms;
		rvms.Address = (ULONG)dwBaseAddress;
		rvms.Size = sizeof(T);
		DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);
		return T(rvms.Response);
	}


};