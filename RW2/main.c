#include "rw2.h"

UNICODE_STRING SymbolicLinkName;
HANDLE TargetProcessId = 0;
PVOID TargetProcessBaseAddress = NULL;

//#define PROCESS_TERMINATE 0x0001
//#define PROCESS_VM_OPERATION 0x0008
//#define PROCESS_VM_READ 0x0010
//#define PROCESS_VM_WRITE 0x0020
//
//PVOID obHandle;//����һ��void*���͵ı�������������ΪObRegisterCallbacks�����ĵ�2�������� 
//
//OB_PREOP_CALLBACK_STATUS
//preCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
//{
//	HANDLE pid = PsGetProcessId((PEPROCESS)pOperationInformation->Object);
//	char szProcName[16] = { 0 };
//	UNREFERENCED_PARAMETER(RegistrationContext);
//	PEPROCESS Process;
//	PsLookupProcessByProcessId(pid, &Process);
//	dprintf("%s", PsGetProcessImageFileName(Process));
//	strcpy(szProcName, PsGetProcessImageFileName(Process));
//	if (!_stricmp(szProcName, "calc.exe"))
//	{
//		if (pOperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
//		{
//			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE)
//			{
//				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
//			}
//			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_OPERATION) == PROCESS_VM_OPERATION)
//			{
//				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_OPERATION;
//			}
//			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_READ) == PROCESS_VM_READ)
//			{
//				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_READ;
//			}
//			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_WRITE) == PROCESS_VM_WRITE)
//			{
//				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_WRITE;
//			}
//		}
//	}
//	return OB_PREOP_SUCCESS;
//}
//NTSTATUS ProtectProcess(BOOLEAN Enable)
//{
//
//	OB_CALLBACK_REGISTRATION obReg;
//	OB_OPERATION_REGISTRATION opReg;
//
//	memset(&obReg, 0, sizeof(obReg));
//	obReg.Version = ObGetFilterVersion();
//	obReg.OperationRegistrationCount = 1;
//	obReg.RegistrationContext = NULL;
//	RtlInitUnicodeString(&obReg.Altitude, L"321000");
//
//	memset(&opReg, 0, sizeof(opReg)); //��ʼ���ṹ�����
//
//  //���� ��ע������ṹ��ĳ�Ա�ֶε�����
//	opReg.ObjectType = PsProcessType;
//	opReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
//
//	opReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall; //������ע��һ���ص�����ָ��
//
//	obReg.OperationRegistration = &opReg; //ע����һ�����
//
//	return ObRegisterCallbacks(&obReg, &obHandle); //������ע��ص�����
//}

NTSTATUS DefaultDispatchFunction(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

VOID NotifyImageLoadCallback(
	_In_opt_ PUNICODE_STRING FullImageName,
	_In_ HANDLE ProcessId,                // pid into which image is being mapped
	_In_ PIMAGE_INFO ImageInfo
)
{
	if (wcsstr(FullImageName->Buffer, TARGET_PROCESS_NAME) && wcsstr(FullImageName->Buffer, L"Device"))
	{
		dprintf("TargetName %ls\n", FullImageName->Buffer);
		dprintf("TargetProcessId %d\n", ProcessId);
		dprintf("TargetImageBase %x\n", ImageInfo->ImageBase);
		TargetProcessId = ProcessId;
		TargetProcessBaseAddress = ImageInfo->ImageBase;
	}
}



VOID DriverUnload(
	_In_ PDRIVER_OBJECT DriverObject
)
{
	if (!NT_SUCCESS(PsRemoveLoadImageNotifyRoutine(NotifyImageLoadCallback)))
	{
		dprintf("Delete NotifyImageLoadCallback Failure");
	}
	if (!NT_SUCCESS(IoDeleteSymbolicLink(&SymbolicLinkName)))
	{
		dprintf("Delete SymbolicLinkName Failure");
	}
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING DeviceName;
	// ���������豸
	RtlInitUnicodeString(&DeviceName, DEVICE_NAME);
	Status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
	if (!NT_SUCCESS(Status))
	{
		dprintf("Create DeviceObject Failure");
		return Status;
	}
	// �����豸�ķ�������
	RtlInitUnicodeString(&SymbolicLinkName, SYMBOLIC_LINK_NAME);
	Status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);
	if (!NT_SUCCESS(Status))
	{
		dprintf("Create SymbolicLink Failure");
		IoDeleteDevice(DeviceObject);
		return Status;
	}

	// ����Irp����
	for (size_t i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DefaultDispatchFunction;
	}
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
	DriverObject->DriverUnload = DriverUnload;

	DriverObject->Flags |= DO_BUFFERED_IO;
	DeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);
	// �������֪ͨ�ص�����
	Status = PsSetLoadImageNotifyRoutine(NotifyImageLoadCallback);
	if (!NT_SUCCESS(Status))
	{
		dprintf("Create LoadImageNotifyRoutine Failure");
		IoDeleteSymbolicLink(&SymbolicLinkName);
		IoDeleteDevice(DeviceObject);
	}
	return Status;
}