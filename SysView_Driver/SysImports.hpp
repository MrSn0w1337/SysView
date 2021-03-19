#pragma once

extern "C"
NTSTATUS NTAPI ZwQuerySystemInformation(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

extern "C" NTSTATUS NTAPI PsSuspendProcess(IN PEPROCESS Process);
extern "C" NTSTATUS NTAPI PsResumeProcess(IN PEPROCESS Process);

extern "C"
NTSTATUS NTAPI ZwQueryInformationProcess(
    _In_      HANDLE           ProcessHandle,
    _In_      PROCESSINFOCLASS ProcessInformationClass,
    _Out_     PVOID            ProcessInformation,
    _In_      ULONG            ProcessInformationLength,
    _Out_opt_ PULONG           ReturnLength
);

extern "C"
NTSTATUS NTAPI ZwQueryInformationThread(
    _In_      HANDLE          ThreadHandle,
    _In_      THREADINFOCLASS ThreadInformationClass,
    _In_      PVOID           ThreadInformation,
    _In_      ULONG           ThreadInformationLength,
    _Out_opt_ PULONG          ReturnLength
);

extern "C" NTSTATUS NTAPI MmCopyVirtualMemory
(
    PEPROCESS SourceProcess,
    PVOID SourceAddress,
    PEPROCESS TargetProcess,
    PVOID TargetAddress,
    SIZE_T BufferSize,
    KPROCESSOR_MODE PreviousMode,
    PSIZE_T ReturnSize
);

extern "C" PPEB NTAPI PsGetProcessPeb(PEPROCESS Process);

extern "C" PVOID NTAPI PsGetProcessWow64Process(IN PEPROCESS Process);

extern "C" PVOID NTAPI PsGetProcessSectionBaseAddress(PEPROCESS Process);