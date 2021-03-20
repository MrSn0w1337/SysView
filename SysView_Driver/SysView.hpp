#pragma once

typedef class CSysView : public CSysList
{
	static PCREATE_PROCESS_NOTIFY_ROUTINE_EX ProcessNotifyRoutineEx
	(HANDLE parentID, HANDLE processID, PPS_CREATE_NOTIFY_INFO notifyInfo);

	static PVOID pObProcHandle;
	static bool IsBlacklisted(wchar_t* wProc);

public:
	CSysView(PBOOL IsInit);
	~CSysView();

	static ULONG GetDriverList(uintptr_t ThreadEntry, char* szModule);
	static ULONG GetProcessList();
	static ULONG GetThreadList(HANDLE PID);
	static ULONG GetModuleList(HANDLE PID, uintptr_t ThreadEntry, wchar_t* wModule);

}SYSVIEW, *PSYSVIEW;
