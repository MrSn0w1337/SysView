#pragma once

namespace Util
{
	bool IsWow64Process(HANDLE ProcessId);
	void KillProcess(HANDLE ProcessId);
	void SuspendProcess(HANDLE ProcessId);
	void ResumeProcess(HANDLE ProcessId);
	void GetProcPath(HANDLE ProcessId, void* pList);
	bool IsSuspended(PSYSTEM_PROCESS_INFORMATION& procInfo);
	NTSTATUS OpenProcessByPointer(PEPROCESS Process, PHANDLE hProcess, ACCESS_MASK AccessMask, ULONG Attributes, KPROCESSOR_MODE ProcessorMode);
	PEPROCESS GetEPROCESS(HANDLE ProcessId);
	uintptr_t GetThreadAddress(HANDLE ThreadId);
	PETHREAD GetETHREAD(HANDLE ThreadId);
	bool OpenThreadByPointer(PETHREAD Thread, PHANDLE hThread, ACCESS_MASK DesiredAccess, ULONG Attributes, KPROCESSOR_MODE ProcessorMode);

	//Patchguard features
	void ChangePID(HANDLE currProcessId, HANDLE newProcessId);
}