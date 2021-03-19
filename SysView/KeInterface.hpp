#pragma once

class CDriver
{
private:
	static HANDLE hDriver;
	static SC_HANDLE schService;
	static bool bIsInit;
	static std::vector<LIST_MODULE>vModules;
	static std::vector<LIST_DRIVER>vDriver;
	static std::vector<LIST_PROCESS>vProcess;
	static std::vector<LIST_THREAD>vThreads; //Thread list for all processes
	static std::vector<std::wstring>vBlacklistProc; //Blacklist for processes
	static tNtSuspendThread NtSuspendThread;
	static tNtResumeThread NtResumeThread;
	static tNtTerminateThread NtTerminateThread;

	static bool Init();
	static void Exit();

public:

	CDriver();
	~CDriver();
	static bool IsInit() { return bIsInit; }
	static std::vector<LIST_MODULE>& GetModuleVector() { return vModules; }
	static std::vector<LIST_DRIVER>& GetDriverVector() { return vDriver; }
	static std::vector<LIST_PROCESS>& GetProcVector() { return vProcess; }
	static std::vector<LIST_THREAD>& GetThreadVector() { return vThreads; }
	static std::vector<std::wstring>& GetBlacklistVector() { return vBlacklistProc; };
	static bool GetProcList();
	static bool GetDriverList();
	static bool GetModuleList(HANDLE PID);
	static bool GetThreadList(HANDLE PID);
	static void KillProcess(HANDLE PID);
	static void SuspendProcess(HANDLE PID);
	static void ResumeProcess(HANDLE PID);
	static void BlockProcess(std::wstring& sProcess, bool Remove);
	static bool IsProcessAlreadyBlacklisted(std::wstring& sProcess);
	static void ThreadOperation(HANDLE TID, DWORD OperationID);
};