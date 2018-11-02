#include "pch.h"

//#define 调式输出(内容){KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, 内容));} 
#define ACTIVE_PROCESS_LINK 0x188

NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(__in PEPROCESS Process);

PLOAD_IMAGE_NOTIFY_ROUTINE NotifyImageLoadCallback(PUNICODE_STRING w_FullImageName, HANDLE w_ProcessId, PIMAGE_INFO w_ImageInfo)
{
	//DbgPrint("RW %ws\n", w_FullImageName->Buffer);
	/*if (wcsstr(w_FullImageName->Buffer, L"cmd.exe") && wcsstr(w_FullImageName->Buffer, L"Device"))
	{
		DbgPrint("RW 111111111111111111111111111111 %ws\n", w_FullImageName->Buffer);
		PUID = w_ProcessId;
		PUBase = GetSectionBaseAddress(w_ProcessId);
		PUBGepGame = GetEpGameIdByProcess(w_ProcessId);
	}*/
	//w_ProcessId = (HANDLE)3776;
	//PUID = w_ProcessId;
	////DbgPrint("RW %d\n", PUID);
	//PUBase = GetSectionBaseAddress(w_ProcessId);
	////DbgPrint("RW %p\n", PUBase);
	//PUBGepGame = GetEpGameIdByProcess(w_ProcessId);
	////DbgPrint("RW %p\n", PUBGepGame);
	return 0;
}

DWORD64 GetSectionBaseAddress(HANDLE w_pid)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PEPROCESS targetProcess;
	PVOID value = 0;

	ntStatus = PsLookupProcessByProcessId((HANDLE)w_pid, &targetProcess);

	if (ntStatus != STATUS_SUCCESS || !targetProcess)
		return 0;

	__try
	{
		KeAttachProcess((PKPROCESS)targetProcess);
		value = PsGetProcessSectionBaseAddress(targetProcess);
		KeDetachProcess();
	}
	__except (GetExceptionCode())
	{
		return 0;
	}

	return (DWORD64)value;
}

PEPROCESS GetEpGameIdByProcess(HANDLE w_pid)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PEPROCESS targetProcess;

	ntStatus = PsLookupProcessByProcessId((HANDLE)w_pid, &targetProcess);

	if (ntStatus != STATUS_SUCCESS || !targetProcess)
		return 0;
	return targetProcess;
}

VOID Unload(PDRIVER_OBJECT  DriverObject)
{
	PsRemoveLoadImageNotifyRoutine((PLOAD_IMAGE_NOTIFY_ROUTINE)NotifyImageLoadCallback);

	UNICODE_STRING usDosDeviceName;

	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\RW");
	IoDeleteSymbolicLink(&usDosDeviceName);

	IoDeleteDevice(DriverObject->DeviceObject);

	//DbgPrint("RW Unload\n");
}

NTSTATUS ReadMemOutputClient(pReadStruct w_poReadStruct)
{
	PEPROCESS hClient, hGame;
	//DbgPrint("RW ReadMemOutputClient\n");
	KAPC_STATE apc_state;
	NTSTATUS NtStatus = STATUS_SUCCESS;

	PUID = (HANDLE)w_poReadStruct->GamePID;
	//DbgPrint("RW %d\n", PUID);
	PUBase = GetSectionBaseAddress(PUID);
	//DbgPrint("RW %p\n", PUBase);
	PUBGepGame = GetEpGameIdByProcess(PUID);
	
	if (PUBase == 0 && w_poReadStruct->ProtocolMsg != PROTO_GET_BASEADDR)
	{
		return STATUS_UNSUCCESSFUL;
	}

	PsLookupProcessByProcessId(PUID, &hGame);
	PsLookupProcessByProcessId((HANDLE)w_poReadStruct->UserPID, &hClient);

	DriverBuffer = ExAllocatePoolWithTag(NonPagedPool, w_poReadStruct->ReadSize, 0);

	

	if (DriverBuffer == NULL)
	{
		return STATUS_MEMORY_NOT_ALLOCATED;
	}

	if (w_poReadStruct->ProtocolMsg == PROTO_GET_BASEADDR)
	{
		__try
		{
			KeStackAttachProcess(hClient, &apc_state);
			ProbeForRead((CONST PVOID)w_poReadStruct->UserBufferAdress, w_poReadStruct->ReadSize, sizeof(CHAR));
			RtlCopyMemory((void *)w_poReadStruct->UserBufferAdress, &PUBase, w_poReadStruct->ReadSize);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}
	}

	else if (w_poReadStruct->ProtocolMsg == PROTO_NORMAL_READ)
	{
		__try
		{
			
			KeStackAttachProcess(hGame, &apc_state);
			ProbeForRead((CONST PVOID)w_poReadStruct->GameAddressOffset, w_poReadStruct->ReadSize, sizeof(CHAR));
			PMDL pMdl;
			PUCHAR SrcAddress;
			//DbgPrint("RW (CONST PVOID)w_poReadStruct->GameAddressOffset) %p\n", (CONST PVOID)w_poReadStruct->GameAddressOffset);
			if (MmIsAddressValid((CONST PVOID)w_poReadStruct->GameAddressOffset))
			{
				
				pMdl = IoAllocateMdl((CONST PVOID)w_poReadStruct->GameAddressOffset, (ULONG)w_poReadStruct->ReadSize, FALSE, FALSE, NULL);

				if (MmIsAddressValid(pMdl))
				{
					MmBuildMdlForNonPagedPool(pMdl);
					SrcAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
					if (MmIsAddressValid(SrcAddress))
						RtlCopyMemory((void *)DriverBuffer, (const void *)SrcAddress, (size_t)w_poReadStruct->ReadSize);
					IoFreeMdl(pMdl);
					
				}
			}

			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}
		__try
		{
			KeStackAttachProcess(hClient, &apc_state);
			ProbeForRead((CONST PVOID)w_poReadStruct->UserBufferAdress, w_poReadStruct->ReadSize, sizeof(CHAR));
			RtlCopyMemory((void *)w_poReadStruct->UserBufferAdress, DriverBuffer, w_poReadStruct->ReadSize);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}
	}
	else if (w_poReadStruct->ProtocolMsg == PROTO_NORMAL_WRITE)
	{
		__try
		{
			KeStackAttachProcess(hClient, &apc_state);
			ProbeForRead((CONST PVOID)w_poReadStruct->UserBufferAdress, w_poReadStruct->ReadSize, sizeof(CHAR));
			RtlCopyMemory((void *)DriverBuffer, (const void *)w_poReadStruct->UserBufferAdress, (size_t)w_poReadStruct->ReadSize);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}
		__try
		{
			KeStackAttachProcess(hGame, &apc_state);
			ProbeForRead((CONST PVOID)w_poReadStruct->GameAddressOffset, w_poReadStruct->ReadSize, sizeof(CHAR));
			RtlCopyMemory((void *)w_poReadStruct->GameAddressOffset, (const void *)DriverBuffer, (size_t)w_poReadStruct->ReadSize);
			KeUnstackDetachProcess(&apc_state);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KeUnstackDetachProcess(&apc_state);
			NtStatus = STATUS_ABANDONED;
		}

	}
	ExFreePool(DriverBuffer);
	return NtStatus;
}

NTSTATUS WriteBufferedIO(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS NtStatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStackIrp = NULL;
	pReadStruct readStruct;

	pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);

	if (pIoStackIrp)
	{
		readStruct = (pReadStruct)Irp->AssociatedIrp.SystemBuffer;
		if (readStruct)
		{
			if (readStruct->ReadSize)
			{
				ReadMemOutputClient(readStruct);
			}
		}
	}

	return NtStatus;
}

NTSTATUS UnSupportedIrpFunction(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS NtStatus = STATUS_NOT_SUPPORTED;
	return NtStatus;
}

NTSTATUS CreateDispatch(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

////通过EPROCESS枚举进程
//NTSTATUS SearchProcessEPROCESS()
//{
//	PEPROCESS process = NULL, firstProcess = NULL;
//	NTSTATUS status = STATUS_SUCCESS;
//	PLIST_ENTRY plist;
//	process = firstProcess = PsGetCurrentProcess();
//	do
//	{
//		PUCHAR ProcessNmae = NULL;
//		ProcessNmae = PsGetProcessImageFileName(process);
//		DbgPrint("PID:%d,ProcessName:%s\n", (HANDLE)PsGetProcessId(process), ProcessNmae);
//		plist = (PLIST_ENTRY)((ULONG)process + ACTIVE_PROCESS_LINK);
//		process = (PEPROCESS)((ULONG)plist->Flink - ACTIVE_PROCESS_LINK);
//		if (process == firstProcess)
//		{
//			break;
//		}
//	} while (process != NULL);
//
//	return status;
//}



NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  pDriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	//DbgPrint("RW Start\n");
	PUBase = 0;
	NTSTATUS status = STATUS_SUCCESS;

	RtlInitUnicodeString(&dev, L"\\Device\\RW");//链接名
	RtlInitUnicodeString(&dos, L"\\DosDevices\\RW");//驱动对象名

	status = IoCreateDevice(pDriverObject, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	if (status == STATUS_SUCCESS)
	{
		//DbgPrint("RW 设备创建成功\n");
		for (UINT32 uiIndex = 0; uiIndex < IRP_MJ_MAXIMUM_FUNCTION; uiIndex++)
		{
			pDriverObject->MajorFunction[uiIndex] = UnSupportedIrpFunction;
		}

		pDriverObject->MajorFunction[IRP_MJ_WRITE] = WriteBufferedIO;
		//pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatch;
		pDriverObject->DriverUnload = Unload;

		pDeviceObject->Flags |= DO_BUFFERED_IO;
		pDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);

		IoCreateSymbolicLink(&dos, &dev);
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
	//PsSetLoadImageNotifyRoutine((PLOAD_IMAGE_NOTIFY_ROUTINE)NotifyImageLoadCallback);
	return STATUS_SUCCESS;
}