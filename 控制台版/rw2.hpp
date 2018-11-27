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
		T Response;
		rvms.Response = &Response;
		rvms.Address = (ULONG)dwBaseAddress;
		rvms.Size = sizeof(T);
		DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);
		return Response;
	}

	template<typename T>
	BOOL wvm(DWORD_PTR dwBaseAddress, T Value)
	{
		WRITE_VIRTUAL_MEMORY_STRUCT wvms;
		wvms.Address = (ULONG)dwBaseAddress;
		wvms.Value = &Value;
		wvms.Size = sizeof(T);
		return DeviceIoControl(hDriver, WRITE_VIRTUAL_MEMORY, &wvms, sizeof(wvms), &wvms, sizeof(wvms), 0, 0);
	}

	std::wstring read_wstring(DWORD_PTR dwBaseAddress,SIZE_T size)
	{
		wchar_t *buffer = new wchar_t[size];
		READ_VIRTUAL_MEMORY_STRUCT rvms;
		rvms.Response = buffer;
		rvms.Address = (ULONG)dwBaseAddress;
		rvms.Size = size;
		DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);
		std::wstring wstr(buffer, rvms.Size);
		delete[]buffer;
		return wstr;
	}

	std::string read_string(DWORD_PTR dwBaseAddress, SIZE_T size)
	{
		char *buffer = new char[size];
		READ_VIRTUAL_MEMORY_STRUCT rvms;
		rvms.Response = buffer;
		rvms.Address = (ULONG)dwBaseAddress;
		rvms.Size = size;
		DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);
		std::string str(buffer, rvms.Size);
		delete[]buffer;
		return str;
	}

	std::vector<byte> read_bytes(DWORD_PTR dwBaseAddress, SIZE_T size)
	{
		std::vector<byte> bytes;
		byte * buffer = new byte[size];
		READ_VIRTUAL_MEMORY_STRUCT rvms;
		rvms.Response = buffer;
		rvms.Address = (ULONG)dwBaseAddress;
		rvms.Size = size;
		DeviceIoControl(hDriver, READ_VIRTUAL_MEMORY, &rvms, sizeof(rvms), &rvms, sizeof(rvms), 0, 0);

		for (size_t i = 0; i < size; i++)
		{
			bytes[i] = buffer[i];
		}
		return bytes;
	}

	BOOL write_bytes(DWORD_PTR dwBaseAddress, std::vector<byte> bytes)
	{
		WRITE_VIRTUAL_MEMORY_STRUCT wvms;
		byte * buffer = new byte[bytes.size()];

		for (size_t i = 0; i < bytes.size(); i++)
		{
			buffer[i] = bytes[i];
		}
		
		wvms.Address = (ULONG)dwBaseAddress;
		wvms.Value = buffer;
		wvms.Size = bytes.size();
		return DeviceIoControl(hDriver, WRITE_VIRTUAL_MEMORY, &wvms, sizeof(wvms), &wvms, sizeof(wvms), 0, 0);
	}
};