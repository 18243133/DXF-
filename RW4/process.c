#include "rw4.h"
NTSTATUS ReadVirtualMemory(
	PREAD_VIRTUAL_MEMORY_STRUCT rvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	SIZE_T Bytes;
	PEPROCESS TargetProcess;
	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	if (NT_SUCCESS(Status))
	{
		Status = MmCopyVirtualMemory(TargetProcess, (CONST PVOID)rvms->Address, PsGetCurrentProcess(), rvms->Response, rvms->Size, KernelMode, &Bytes);
		ObDereferenceObject(TargetProcess);
	}
	return Status;
	
}

NTSTATUS WriteVirtualMemory(
	PWRITE_VIRTUAL_MEMORY_STRUCT wvms
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	SIZE_T Bytes;
	PEPROCESS TargetProcess;
	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);
	if (NT_SUCCESS(Status))
	{
		Status = MmCopyVirtualMemory(PsGetCurrentProcess(), wvms->Value, TargetProcess, (PVOID)wvms->Address, wvms->Size, KernelMode, &Bytes);
		ObDereferenceObject(TargetProcess);
	}
	return Status;
}

HANDLE DriverAllocMemory(ULONG RegionSize)
{

	NTSTATUS Status;
	PEPROCESS TargetProcess;
	HANDLE BaseAddr = NULL;
	KAPC_STATE ApcState;

	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);

	if (NT_SUCCESS(Status))
	{

		KeStackAttachProcess((PKPROCESS)TargetProcess, &ApcState);
		__try
		{
			ZwAllocateVirtualMemory(NtCurrentProcess(),(PVOID*)&BaseAddr,0,(PSIZE_T)&RegionSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
			MmProtectMdlSystemAddress(BaseAddr, PAGE_EXECUTE_READWRITE);
			RtlZeroMemory(BaseAddr, RegionSize); //�ؼ���
			//�����ǹؼ���,�ڴ������,Ȼ��ring0���ٵ��� VirtualProtectEx() �޸��ڴ�����,�������뵽���ڴ�,��֮ǰ�Ĵ���,��Ī������ľͿ��Է�����
			//ԭ���Ҳ���,֮ǰ��Ϊ���뵽���ڴ�,���ܷ���,�Ҿ���CE�鿴�ڴ�,Ȼ������CE���ʹ�����ڴ�Ϳ���������д��
			//�����Ҿͺ��Ҳ���,���������������,ring0�����ڴ�,Ȼ������,Ȼ��ring3�޸��ڴ�����,�Ϳ��Է���
			KeUnstackDetachProcess(&ApcState);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&ApcState);
		}
		ObDereferenceObject(TargetProcess);
	}

	return BaseAddr;
}
HANDLE GetProcessHandle()
{
	NTSTATUS Status;
	PEPROCESS TargetProcess;
	HANDLE hProcess = NULL;
	UNICODE_STRING Unicode;
	PVOID PsProcessType;

	Status = PsLookupProcessByProcessId(TargetProcessId, &TargetProcess);

	if (NT_SUCCESS(Status)) {
		RtlInitUnicodeString(&Unicode,L"PsProcessType");
		PsProcessType = MmGetSystemRoutineAddress(&Unicode);
		if (PsProcessType)
		{
			Status = ObOpenObjectByPointer(
				TargetProcess,
				0,
				NULL,
				PROCESS_ALL_ACCESS,
				*(POBJECT_TYPE*)PsProcessType,
				KernelMode,
				&hProcess
			);
		}
		ObDereferenceObject(TargetProcess);
	}
	return hProcess;
}