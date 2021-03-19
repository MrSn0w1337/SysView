#include "Includes.hpp"


NTSTATUS Util::OpenProcessByPointer(PEPROCESS Process, PHANDLE hProcess, ACCESS_MASK AccessMask, ULONG Attributes, KPROCESSOR_MODE ProcessorMode)
{
	return ObOpenObjectByPointer(Process, Attributes, 0, AccessMask, *PsProcessType, ProcessorMode, hProcess);
}

PEPROCESS Util::GetEPROCESS(HANDLE ProcessId)
{
	PEPROCESS tmpProcess = nullptr;	
	PsLookupProcessByProcessId(ProcessId, &tmpProcess);
	return tmpProcess;
}

uintptr_t Util::GetThreadAddress(HANDLE ThreadId)
{
	uintptr_t addr = 0;
	HANDLE hThread = 0;
	PETHREAD Thread = GetETHREAD(ThreadId);
	if (!Thread)
		return addr;
	
	if (!OpenThreadByPointer(Thread, &hThread, THREAD_ALL_ACCESS, 0, KernelMode))
	{
		ObDereferenceObject(Thread);
		return addr;
	}

	ULONG bytes = 0;
	ZwQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &addr, sizeof(uintptr_t), &bytes);
	ZwClose(hThread);
	ObDereferenceObject(Thread);
	return addr;
}

PETHREAD Util::GetETHREAD(HANDLE ThreadId)
{
	PETHREAD pThread = nullptr;
	PsLookupThreadByThreadId(ThreadId, &pThread);
	return pThread;
}

bool Util::OpenThreadByPointer(PETHREAD Thread, PHANDLE hThread, ACCESS_MASK DesiredAccess, ULONG Attributes, KPROCESSOR_MODE ProcessorMode)
{
	if (ObOpenObjectByPointer(Thread, Attributes, nullptr, DesiredAccess, *PsThreadType, ProcessorMode, hThread)
		!= STATUS_SUCCESS)
		return false;
	else
		return true;
}

void Util::GetProcPath(HANDLE ProcessId, void* pList)
{
	//Get path
	UNICODE_STRING procPath = { 0 };
	HANDLE hProc = 0;
	auto proc = GetEPROCESS(ProcessId);
	if (!proc)
		return;

	if (OpenProcessByPointer(proc, &hProc, PROCESS_ALL_ACCESS, 0, KernelMode) != STATUS_SUCCESS)
	{
		ObDereferenceObject(proc);
		return;
	}

	//Get size of path
	ObDereferenceObject(proc);
	ULONG Bytes = 0;
	if (ZwQueryInformationProcess(hProc, ProcessImageFileName, nullptr, 0, &Bytes) != STATUS_INFO_LENGTH_MISMATCH)
	{
		ZwClose(hProc);
		return;
	}

	auto buffer = ExAllocatePoolWithTag(NonPagedPool, Bytes, 0xC);
	if (!buffer)
	{
		ZwClose(hProc);
		return;
	}

	if (ZwQueryInformationProcess(hProc, ProcessImageFileName, buffer, Bytes, nullptr) != STATUS_SUCCESS)
	{
		ZwClose(hProc);
		ExFreePoolWithTag(buffer, 0xC);
		return;
	}

	RtlCopyUnicodeString(&procPath, (PUNICODE_STRING)buffer);
	auto pPath = (PUNICODE_STRING)buffer;
	auto pListCopy = (PLIST_PROCESS)pList;
	wcscpy_s(pListCopy->ImagePath, pPath->Buffer);
	ExFreePoolWithTag(buffer, 0xC);
	ZwClose(hProc);
}

bool Util::IsWow64Process(HANDLE ProcessId)
{
	auto Process = GetEPROCESS(ProcessId);
	HANDLE hProc = 0;
	ULONG bytes = 0;
	ULONG_PTR wow64Info = 0;
	OpenProcessByPointer(Process, &hProc, PROCESS_ALL_ACCESS, 0, KernelMode);
	ZwQueryInformationProcess(hProc, ProcessWow64Information, &wow64Info, sizeof(ULONG_PTR), &bytes);
	ObDereferenceObject(Process);
	return wow64Info;
}

void Util::KillProcess(HANDLE ProcessId)
{
	HANDLE hProc = 0;
	PEPROCESS pProcess = nullptr;
	pProcess = GetEPROCESS(ProcessId);
	if (!pProcess)
		return;

	if (OpenProcessByPointer(pProcess, &hProc, PROCESS_ALL_ACCESS, 0, KernelMode) != STATUS_SUCCESS)
	{
		ObDereferenceObject(pProcess);
		return;
	}

	ZwTerminateProcess(hProc, STATUS_SUCCESS);
	ObDereferenceObject(pProcess);
	ZwClose(hProc);
}

void Util::SuspendProcess(HANDLE ProcessId)
{
	auto pProcess = GetEPROCESS(ProcessId);
	if (!pProcess)
		return;

	PsSuspendProcess(pProcess);
	ObDereferenceObject(pProcess);
}

void Util::ResumeProcess(HANDLE ProcessId)
{
	auto pProcess = GetEPROCESS(ProcessId);
	if (!pProcess)
		return;

	PsResumeProcess(pProcess);
	ObDereferenceObject(pProcess);
}

bool Util::IsSuspended(PSYSTEM_PROCESS_INFORMATION& procInfo)
{
	ULONG suspendedCount = 0;
	for (ULONG i = 0; i <= procInfo->NumberOfThreads; i++)
	{
		if (procInfo->Threads[i].ThreadState == KTHREAD_STATE::Waiting && procInfo->Threads[i].WaitReason == KWAIT_REASON::Suspended)
			suspendedCount++;
	}

	if (suspendedCount == procInfo->NumberOfThreads)
		return true;
	else
		return false;
}

void Util::ChangePID(HANDLE currProcessId, HANDLE newProcessId)
{
	auto Process = reinterpret_cast<PMYEPROCESS>(GetEPROCESS(currProcessId));
	if (!Process)
		return;

	Process->UniqueProcessId = newProcessId;
}