#include "Includes.hpp"

PVOID CSysView::pObProcHandle;

CSysView::CSysView(PBOOL IsInit)
{
	CSysView::pObProcHandle = nullptr;
	*IsInit = TRUE;

	if (!NT_SUCCESS(PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessNotifyRoutineEx, FALSE)))
		*IsInit = FALSE;
}

CSysView::~CSysView()
{
	CSysList::~CSysList();
	PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessNotifyRoutineEx, TRUE);
}

ULONG CSysView::GetDriverList(uintptr_t ThreadEntry, char* szModule)
{
	ULONG Bytes = 0;
	ULONG NodeCount = 0;

	//Get size of module list
	auto status = ZwQuerySystemInformation(SystemModuleInformation, 0, Bytes, &Bytes);
	if (status != STATUS_INFO_LENGTH_MISMATCH)
		return NodeCount;

	//Allocate memory for modules
	auto pModuleList = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, Bytes, 0xA);
	if (!pModuleList)
		return NodeCount;

	status = ZwQuerySystemInformation(SystemModuleInformation, pModuleList, Bytes, &Bytes);
	if (status != STATUS_SUCCESS)
	{
		ExFreePoolWithTag(pModuleList, 0xA);
		return NodeCount;
	}

	auto pModInfo = pModuleList->Modules;
	if(!ThreadEntry && !szModule)
		NodeCount = pModuleList->NumberOfModules;

	for (ULONG i = 0; i < pModuleList->NumberOfModules; i++)
	{
		if (ThreadEntry && szModule)
		{
			auto modStart = (uintptr_t)pModInfo[i].ImageBase;
			auto modEnd = (uintptr_t)((uintptr_t)pModInfo[i].ImageBase + pModInfo[i].ImageSize);
			if (ThreadEntry < modStart || ThreadEntry > modEnd)
				continue;

			strcpy(szModule, (char*)(pModInfo[i].FullPathName + pModInfo[i].OffsetToFileName));
			NodeCount = 1;
			break;
		}
		else
		{
			if (pModInfo[i].ImageBase)
			{
				LIST_DRIVER DriverEntry = { 0 };
				DriverEntry.ImageBase = (uintptr_t)pModInfo[i].ImageBase;
				DriverEntry.ImageSize = pModInfo[i].ImageSize;
				strcpy_s(DriverEntry.Path, (char*)pModInfo[i].FullPathName);
				strcpy_s(DriverEntry.ImageName, (char*)(pModInfo[i].FullPathName + pModInfo[i].OffsetToFileName));
				CSysList::Insert(&DriverEntry, ID::DRIVER);
			}
		}
	}

	//Cleanup and exit
	ExFreePoolWithTag(pModuleList, 0xA);
	if (!NodeCount && szModule && ThreadEntry) strcpy(szModule, "Unknown");
	return NodeCount;
}

ULONG CSysView::GetProcessList()
{
	ULONG Bytes = 0;
	ULONG NodeCount = 0;

	//Get SystemProcessInformation size
	auto status = ZwQuerySystemInformation(SystemProcessInformation, 0, 0, &Bytes);
	if (status != STATUS_INFO_LENGTH_MISMATCH)
		return NodeCount;

	if (!Bytes)
		return NodeCount;

	//Allocate memory
	auto pProc = (PSYSTEM_PROCESS_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, Bytes, 0xB);
	if (!pProc)
		return NodeCount;

	//Fill struct
	status = ZwQuerySystemInformation(SystemProcessInformation, pProc, Bytes, &Bytes);
	if (status != STATUS_SUCCESS)
	{
		ExFreePoolWithTag(pProc, 0xB);
		return NodeCount;
	}

	//Create linked list
	auto currProc = (PSYSTEM_PROCESS_INFORMATION)pProc;
	for (;;)
	{
		LIST_PROCESS procEntry = { 0 };
		if (currProc->UniqueProcessId)
			Util::GetProcPath(currProc->UniqueProcessId, &procEntry);

		procEntry.ThreadsCount = currProc->NumberOfThreads;
		procEntry.PID = currProc->UniqueProcessId;
		procEntry.IsSuspended = Util::IsSuspended(currProc);
		wcscpy_s(procEntry.ImageName, currProc->ImageName.Buffer);
		Insert(&procEntry, ID::PROCESS);

		if (currProc->NextEntryOffset)
			currProc = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)currProc + currProc->NextEntryOffset);
		else
			break;
		NodeCount++;
	}

	ExFreePoolWithTag(pProc, 0xB);
	return NodeCount;
}

ULONG CSysView::GetModuleList(HANDLE PID, uintptr_t ThreadEntry, wchar_t* wModule)
{
	ULONG NodeCount = 0;
	auto Process = Util::GetEPROCESS(PID);
	if (!Process)
		return NodeCount;

	KAPC_STATE apc;
	KeStackAttachProcess(Process, &apc);

	if (Util::IsWow64Process(PID))
	{
		auto Peb32 = reinterpret_cast<PPEB32>(PsGetProcessWow64Process(Process));
		if (!Peb32)
			return NodeCount;

		for (auto pListEntry32 = (PLIST_ENTRY32)((PPEB_LDR_DATA32)Peb32->Ldr)->InLoadOrderModuleList.Flink;
			pListEntry32 != &((PPEB_LDR_DATA32)Peb32->Ldr)->InLoadOrderModuleList; pListEntry32 = (PLIST_ENTRY32)pListEntry32->Flink)
		{
			auto modEntry32 = (PLDR_DATA_TABLE_ENTRY32)
				CONTAINING_RECORD(pListEntry32, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

			if (ThreadEntry && wModule)
			{
				auto modStart = (uintptr_t)modEntry32->DllBase;
				auto modEnd = (uintptr_t)modEntry32->DllBase + modEntry32->SizeOfImage;
				if (ThreadEntry < modStart || ThreadEntry > modEnd)
					continue;
				
				wcscpy(wModule, (PWCH)modEntry32->BaseDllName.Buffer);
				NodeCount = 1;
				break;
			}
			else
			{
				LIST_MODULE modInfo = { 0 };
				modInfo.DllBase = (uintptr_t)modEntry32->DllBase;
				modInfo.ImageSize = modEntry32->SizeOfImage;
				modInfo.EntryPoint = (uintptr_t)modEntry32->EntryPoint;
				wcscpy_s(modInfo.BaseDllName, (PWCH)modEntry32->BaseDllName.Buffer);
				wcscpy_s(modInfo.FullDllName, (PWCH)modEntry32->FullDllName.Buffer);
				CSysList::Insert(&modInfo, ID::MODULE);
				NodeCount++;
			}
		}
	}
	else
	{
		auto Peb = reinterpret_cast<PPEB64>(PsGetProcessPeb(Process));
		if (!Peb)
			return NodeCount;

		for (auto pListEntry = Peb->Ldr->InLoadOrderModuleList.Flink;
			pListEntry != &Peb->Ldr->InLoadOrderModuleList; pListEntry = pListEntry->Flink)
		{
			auto modEntry = (PLDR_DATA_TABLE_ENTRY)CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

			if (ThreadEntry && wModule)
			{
				auto modStart = (uintptr_t)modEntry->DllBase;
				auto modEnd = (uintptr_t)modEntry->DllBase + modEntry->SizeOfImage;
				if (ThreadEntry < modStart || ThreadEntry > modEnd)
					continue;

				wcscpy(wModule, modEntry->BaseDllName.Buffer);
				NodeCount = 1;
				break;
			}
			else
			{
				LIST_MODULE modInfo = { 0 };
				modInfo.DllBase = (uintptr_t)modEntry->DllBase;
				modInfo.ImageSize = modEntry->SizeOfImage;
				modInfo.EntryPoint = (uintptr_t)modEntry->EntryPoint;
				wcscpy_s(modInfo.BaseDllName, modEntry->BaseDllName.Buffer);
				wcscpy_s(modInfo.FullDllName, modEntry->FullDllName.Buffer);
				CSysList::Insert(&modInfo, ID::MODULE);
				NodeCount++;
			}
		}
	}

	KeUnstackDetachProcess(&apc);
	ObDereferenceObject(Process);
	if (!NodeCount && wModule && ThreadEntry) wcscpy(wModule, L"Unknown");
	return NodeCount;
}

ULONG CSysView::GetThreadList(HANDLE PID)
{
	ULONG Bytes = 0;
	ULONG NodeCount = 0;

	//Get SystemProcessInformation size
	auto status = ZwQuerySystemInformation(SystemProcessInformation, 0, 0, &Bytes);
	if (status != STATUS_INFO_LENGTH_MISMATCH)
		return NodeCount;

	if (!Bytes)
		return NodeCount;

	//Allocate memory
	auto pProc = (PSYSTEM_PROCESS_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, Bytes, 0xC);
	if (!pProc)
		return NodeCount;

	//Fill struct
	status = ZwQuerySystemInformation(SystemProcessInformation, pProc, Bytes, &Bytes);
	if (status != STATUS_SUCCESS)
	{
		ExFreePoolWithTag(pProc, 0xC);
		return NodeCount;
	}

	//Create linked list
	auto currProc = pProc;
	for (;;) //Process loop
	{
		if (currProc->UniqueProcessId == PID)
		{
			NodeCount = currProc->NumberOfThreads;
			for (ULONG i = 0; i < currProc->NumberOfThreads; i++) //Thread loop
			{
				LIST_THREAD thEntry = { 0 };
				thEntry.TID = currProc->Threads[i].ClientId.UniqueThread;
				thEntry.StartAddress = Util::GetThreadAddress(thEntry.TID);
				thEntry.Status = currProc->Threads[i].ThreadState;
				if (currProc->Threads[i].ThreadState == KTHREAD_STATE::Waiting
					&& currProc->Threads[i].WaitReason == KWAIT_REASON::Suspended)
					thEntry.IsSuspended = true;
				else
					thEntry.IsSuspended = false;

				if (PID != (HANDLE)4)
					CSysView::GetModuleList(PID, thEntry.StartAddress, thEntry.ModuleName);
				else
					CSysView::GetDriverList(thEntry.StartAddress, thEntry.szModuleName);

				CSysList::Insert(&thEntry, ID::THREAD);
			}
			break;
		}

		if (currProc->NextEntryOffset)
			currProc = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)currProc + currProc->NextEntryOffset);
		else
			break;
	}

	ExFreePoolWithTag(pProc, 0xC);
	return NodeCount;
}

bool CSysView::IsBlacklisted(wchar_t* wProc)
{
	auto currBlacklistEntry = BlacklistHead;
	while (currBlacklistEntry)
	{
		if (wcsstr(wProc, currBlacklistEntry->wName))
			return true;

		currBlacklistEntry = currBlacklistEntry->Next;
	}

	return false;
}

PCREATE_PROCESS_NOTIFY_ROUTINE_EX CSysView::ProcessNotifyRoutineEx(HANDLE parentID, HANDLE processID, PPS_CREATE_NOTIFY_INFO notifyInfo)
{
	if (notifyInfo && BlacklistHead)
	{
		if (CSysView::IsBlacklisted(notifyInfo->ImageFileName->Buffer))
			notifyInfo->CreationStatus = STATUS_ACCESS_DENIED;
	}

	return STATUS_SUCCESS;
}
