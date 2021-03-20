#pragma once

typedef enum _KTHREAD_STATE
{
	Initialized,
	Ready,
	Running,
	Standby,
	Terminated,
	Waiting,
	Transition,
	DeferredReady,
	GateWaitObsolete,
	WaitingForProcessInSwap,
	MaximumThreadState
} KTHREAD_STATE, * PKTHREAD_STATE;

typedef struct _LIST_DRIVER
{
	char ImageName[MAX_PATH];
	char Path[MAX_PATH];
	uintptr_t ImageBase;
	ULONG ImageSize;

	_LIST_DRIVER* Next;
	_LIST_DRIVER* Last;
} LIST_DRIVER, * PLIST_DRIVER;

typedef struct _LIST_PROCESS
{
	wchar_t ImageName[MAX_PATH];
	wchar_t ImagePath[MAX_PATH];
	ULONG ThreadsCount;
	HANDLE PID;
	bool IsSuspended;
	bool IsWow64;

	_LIST_PROCESS* Next;
	_LIST_PROCESS* Last;
} LIST_PROCESS, * PLIST_PROCESS;

typedef struct _LIST_THREAD
{
	wchar_t ModuleName[MAX_PATH];
	char szModuleName[MAX_PATH];
	uintptr_t StartAddress;
	HANDLE TID;
	ULONG Status;
	bool IsSuspended;

	_LIST_THREAD* Next;
	_LIST_THREAD* Last;
} LIST_THREAD, * PLIST_THREAD;

typedef struct _LIST_MODULE //32 & 64-Bit
{
	uintptr_t DllBase;
	uintptr_t EntryPoint;
	ULONG ImageSize;
	wchar_t FullDllName[MAX_PATH];
	wchar_t BaseDllName[MAX_PATH];

	_LIST_MODULE* Next;
	_LIST_MODULE* Last;

}LIST_MODULE, * PLIST_MODULE;

#define IO_REQUEST_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x500, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x501, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_KILL_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x502, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_SUSPEND_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x503, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_RESUME_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x504, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_UNLOAD CTL_CODE(FILE_DEVICE_UNKNOWN, 0x505, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_THREAD CTL_CODE(FILE_DEVICE_UNKNOWN, 0x506, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_BLACKLIST_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x507, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_THREAD_HANDLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x508, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_REQUEST_MODULE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x509, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _LIST_DATA
{
	PULONG NodeCount; //Amount of nodes in the linked list
	void* buffer; //Buffer to PDRIVER_LIST array/vector
	HANDLE PID; //PID for GetThreadList
}LIST_DATA, * PLIST_DATA;

typedef struct _DATA_BLACKLIST
{
	bool bRemove;
	wchar_t wName[MAX_PATH];
}DATA_BLACKLIST, * PDATA_BLACKLIST;

typedef struct _THREAD_HANDLE
{
	HANDLE TID;
	PHANDLE hThread;
}THREAD_HANDLE, *PTHREAD_HANDLE;

typedef NTSTATUS(NTAPI* tNtSuspendThread)(HANDLE, PULONG);
typedef NTSTATUS(NTAPI* tNtResumeThread)(HANDLE, PULONG);
typedef NTSTATUS(NTAPI* tNtTerminateThread)(HANDLE, PULONG);
#define ID_KILL_THREAD 0
#define ID_SUSPEND_THREAD 1
#define ID_RESUME_THREAD 2
