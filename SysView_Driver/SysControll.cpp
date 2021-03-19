#include "Includes.hpp"

extern PSYSVIEW SysView;

extern "C" NTSTATUS SysControll::IoControll(PDEVICE_OBJECT pDeviceObject, PIRP pIRP)
{
	ULONG ByteIO = 0;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	auto pStack = IoGetCurrentIrpStackLocation(pIRP);
	auto CodeIO = pStack->Parameters.DeviceIoControl.IoControlCode;

	switch (CodeIO)
	{
		case IO_REQUEST_DRIVER:
		{
			auto pDriverData = (PLIST_DATA)pIRP->AssociatedIrp.SystemBuffer;
			if (!pDriverData)
				break;

			if (!pDriverData->buffer)
			{
				*pDriverData->NodeCount = SysView->GetDriverList(0, nullptr);
				if (*pDriverData->NodeCount)
					status = STATUS_SUCCESS;
			}
			else
			{
				auto controllArray = (PLIST_DRIVER)pDriverData->buffer;
				for (ULONG i = 0; i < *pDriverData->NodeCount; i++)
				{
					auto currNode = (PLIST_DRIVER)SysView->Get(i, ID::DRIVER);
					if (!currNode)
						continue;
					
					controllArray[i].ImageBase = currNode->ImageBase;
					controllArray[i].ImageSize = currNode->ImageSize;
					strcpy_s(controllArray[i].Path, currNode->Path);
					strcpy_s(controllArray[i].ImageName, currNode->ImageName);
				}
				//Clean linked list
				SysView->Erease(ID::DRIVER);
				status = STATUS_SUCCESS;
			}
			break;
		}

		case IO_REQUEST_PROCESS:
		{
			auto pProcData = (PLIST_DATA)pIRP->AssociatedIrp.SystemBuffer;
			if (!pProcData)
				break;

			if (!pProcData->buffer)
			{
				*pProcData->NodeCount = SysView->GetProcessList();
				if (*pProcData->NodeCount)
					status = STATUS_SUCCESS;
			}
			else
			{
				auto controllArray = (PLIST_PROCESS)pProcData->buffer;
				for (ULONG i = 0; i < *pProcData->NodeCount; i++)
				{
					auto currNode = (PLIST_PROCESS)SysView->Get(i, ID::PROCESS);
					if (!currNode)
						continue;

			
					controllArray[i].ThreadsCount = currNode->ThreadsCount;
					controllArray[i].PID = currNode->PID;
					controllArray[i].IsSuspended = currNode->IsSuspended;
					wcscpy_s(controllArray[i].ImageName, currNode->ImageName);
					wcscpy_s(controllArray[i].ImagePath, currNode->ImagePath);
				}
	
				SysView->Erease(ID::PROCESS);
				status = STATUS_SUCCESS;
			}
			break;
		}

		case IO_REQUEST_THREAD:
		{
			auto pThreadData = (PLIST_DATA)pIRP->AssociatedIrp.SystemBuffer;
			if (!pThreadData)
				break;

			if (!pThreadData->PID)
				break;

			if (!pThreadData->buffer)
			{
				*pThreadData->NodeCount = SysView->GetThreadList((HANDLE)pThreadData->PID);
				if (*pThreadData->NodeCount)
					status = STATUS_SUCCESS;
			}
			else
			{
				auto controllArray = (PLIST_THREAD)pThreadData->buffer;
				for (ULONG i = 0; i < *pThreadData->NodeCount; i++)
				{
					auto currNode = (PLIST_THREAD)SysView->Get(i, ID::THREAD);
					if (!currNode)
						continue;

					controllArray[i].IsSuspended = currNode->IsSuspended;
					controllArray[i].StartAddress = currNode->StartAddress;
					controllArray[i].TID = currNode->TID;
					controllArray[i].Status = currNode->Status;

					if (pThreadData->PID != (HANDLE)4)
						wcscpy_s(controllArray[i].ModuleName, currNode->ModuleName);
					else
						strcpy_s(controllArray[i].szModuleName, currNode->szModuleName);
				}

				SysView->Erease(ID::THREAD);
				status = STATUS_SUCCESS;
			}
			break;
		}

		case IO_REQUEST_MODULE:
		{
			auto pModData = (PLIST_DATA)pIRP->AssociatedIrp.SystemBuffer;
			if (!pModData || pModData->PID == (HANDLE)4) //4 == ntoskrnl.exe
				break;

			if (!pModData->buffer)
			{
				*pModData->NodeCount = SysView->GetModuleList(pModData->PID, 0, nullptr);
				if (*pModData->NodeCount)
					status = STATUS_SUCCESS;
			}
			else
			{
				auto controllArray = (PLIST_MODULE)pModData->buffer;
				for (ULONG i = 0; i < *pModData->NodeCount; i++)
				{
					auto currNode = (PLIST_MODULE)SysView->Get(i, ID::MODULE);
					if (!currNode)
						continue;

					controllArray[i].DllBase = currNode->DllBase;
					controllArray[i].EntryPoint = currNode->EntryPoint;
					controllArray[i].ImageSize = currNode->ImageSize;
					wcscpy_s(controllArray[i].BaseDllName, currNode->BaseDllName);
					wcscpy_s(controllArray[i].FullDllName, currNode->FullDllName);
				}
				
				SysView->Erease(ID::MODULE);
				status = STATUS_SUCCESS;
			}
			break;
		}

		case IO_REQUEST_KILL_PROCESS:
		{
			auto PID = *(PHANDLE)pIRP->AssociatedIrp.SystemBuffer;
			if (PID)
				Util::KillProcess(PID);

			status = STATUS_SUCCESS;
			break;
		}

		case IO_REQUEST_SUSPEND_PROCESS:
		{
			auto PID = *(PHANDLE)pIRP->AssociatedIrp.SystemBuffer;
			if (PID)
				Util::SuspendProcess(PID);

			status = STATUS_SUCCESS;
			break;
		}

		case IO_REQUEST_THREAD_HANDLE:
		{
			auto ThreadHandle = (PTHREAD_HANDLE)pIRP->AssociatedIrp.SystemBuffer;
			if (!ThreadHandle || !ThreadHandle->TID)
				break;

			auto Thread = Util::GetETHREAD(ThreadHandle->TID);
			if (!Thread)
				break;

			HANDLE hThread = 0;
			Util::OpenThreadByPointer(Thread, &hThread, THREAD_ALL_ACCESS, 0, KernelMode);
			*ThreadHandle->hThread = hThread;
			ObDereferenceObject(Thread);
		}

		case IO_REQUEST_RESUME_PROCESS:
		{
			auto PID = *(PHANDLE)pIRP->AssociatedIrp.SystemBuffer;
			if (PID)
				Util::ResumeProcess(PID);

			status = STATUS_SUCCESS;
			break;
		}

		case IO_REQUEST_BLACKLIST_PROCESS:
		{
			auto DataBlacklist = (PDATA_BLACKLIST)pIRP->AssociatedIrp.SystemBuffer;
			if(!wcslen(DataBlacklist->wName))
				break;

			if (!DataBlacklist->bRemove)
			{
				LIST_BLACKLIST Entry = { 0 };
				wcscpy_s(Entry.wName, DataBlacklist->wName);
				SysView->Insert(&Entry, ID::BLACKLIST);
			}
			else
			{
				SysView->Remove(&DataBlacklist->wName, ID::BLACKLIST);
			}
			status = STATUS_SUCCESS;
		}
	}

	//Exit
	pIRP->IoStatus.Status = status;
	pIRP->IoStatus.Information = ByteIO;
	IoCompleteRequest(pIRP, IO_NO_INCREMENT);
	return status;
}

extern "C" NTSTATUS SysControll::IoCloseCall(PDEVICE_OBJECT pDeviceObject, PIRP pIRP)
{
	UNREFERENCED_PARAMETER(pDeviceObject);
	pIRP->IoStatus.Status = STATUS_SUCCESS;
	pIRP->IoStatus.Information = 0;
	IoCompleteRequest(pIRP, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

extern "C" NTSTATUS SysControll::IoCreateCall(PDEVICE_OBJECT pDeviceObject, PIRP pIRP)
{
	UNREFERENCED_PARAMETER(pDeviceObject);
	pIRP->IoStatus.Status = STATUS_SUCCESS;
	pIRP->IoStatus.Information = 0;
	IoCompleteRequest(pIRP, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}