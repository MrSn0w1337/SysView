#include "Includes.h"

std::vector<LIST_MODULE>CDriver::vModules;
std::vector<LIST_DRIVER> CDriver::vDriver;
std::vector<LIST_PROCESS> CDriver::vProcess;
std::vector<LIST_THREAD> CDriver::vThreads;
std::vector<std::wstring>CDriver::vBlacklistProc; //Blacklist for processes
bool CDriver::bIsInit;
HANDLE CDriver::hDriver;
SC_HANDLE CDriver::schService;
tNtSuspendThread CDriver::NtSuspendThread;
tNtResumeThread CDriver::NtResumeThread;
tNtTerminateThread CDriver::NtTerminateThread;

CDriver::CDriver()
{
	CDriver::bIsInit = true;
	if (Init())
		CDriver::hDriver = CreateFileA("\\\\.\\SysView", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

	CDriver::NtSuspendThread = reinterpret_cast<tNtSuspendThread>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtSuspendThread"));
	CDriver::NtResumeThread = reinterpret_cast<tNtResumeThread>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtResumeThread"));
	CDriver::NtTerminateThread = reinterpret_cast<tNtTerminateThread>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtTerminateThread"));

	if (!CDriver::NtSuspendThread || !CDriver::NtResumeThread || !CDriver::NtTerminateThread || !CDriver::hDriver)
		bIsInit = false;
}

CDriver::~CDriver()
{
	//Stop service
	Exit();
}

void CDriver::Exit()
{
	SERVICE_STATUS drvStatus = { 0 };
	drvStatus.dwServiceType = SERVICE_KERNEL_DRIVER;
	drvStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	drvStatus.dwCurrentState = SERVICE_STOPPED;
	ControlService(CDriver::schService, SERVICE_CONTROL_STOP, &drvStatus);
	CloseServiceHandle(CDriver::schService);

	STARTUPINFOA startInfo;
	memset(&startInfo, 0, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	PROCESS_INFORMATION procInfo;
	memset(&procInfo, 0, sizeof(procInfo));

	//Avoid problems after the driver is unloaded and service is deleted
	CreateProcessA(0, "sc query SysView", 0, 0, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
		0, 0, &startInfo, &procInfo);

	CloseHandle(procInfo.hProcess);
}

bool CDriver::Init()
{
	char szCurrDirectory[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, szCurrDirectory);
	char szBuffer[MAX_PATH + 50];
	sprintf_s(szBuffer, "%s\\SysView_Driver.sys", szCurrDirectory);

	if (!GetFileAttributesA(szBuffer))
		return false;

	SC_HANDLE schSCManager = 0;
	schSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if (schSCManager == INVALID_HANDLE_VALUE)
		return false;

	//Check if Service Exist
	CDriver::schService = OpenServiceA(schSCManager, "SysView", SC_MANAGER_ALL_ACCESS);
	if (!CDriver::schService)
	{
		CDriver::schService = CreateServiceA(schSCManager, "SysView", "SysView Kernel Mode Driver", SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, szBuffer, 0, 0, 0, 0, 0);

		if (!CDriver::schService)
			return false;

		if (!StartServiceA(CDriver::schService, 0, nullptr))
		{
			DeleteService(CDriver::schService);
			return false;
		}
		DeleteService(CDriver::schService);
	}

	CloseServiceHandle(schSCManager);
	return true;
}

void CDriver::KillProcess(HANDLE PID)
{
	DWORD Bytes = 0;
	DeviceIoControl(CDriver::hDriver, IO_REQUEST_KILL_PROCESS, &PID, sizeof(HANDLE), nullptr, 0, &Bytes, nullptr);
}

void CDriver::SuspendProcess(HANDLE PID)
{
	DWORD Bytes = 0;
	DeviceIoControl(CDriver::hDriver, IO_REQUEST_SUSPEND_PROCESS, &PID, sizeof(HANDLE), nullptr, 0, &Bytes, nullptr);
}

void CDriver::ResumeProcess(HANDLE PID)
{
	DWORD Bytes = 0;
	DeviceIoControl(CDriver::hDriver, IO_REQUEST_RESUME_PROCESS, &PID, sizeof(HANDLE), nullptr, 0, &Bytes, nullptr);
}

void CDriver::BlockProcess(std::wstring& sProcess, bool Remove)
{
	DATA_BLACKLIST blacklist = { 0 };
	blacklist.bRemove = Remove;
	wcscpy_s(blacklist.wName, sProcess.c_str());
	DeviceIoControl(CDriver::hDriver, IO_REQUEST_BLACKLIST_PROCESS, &blacklist, sizeof(DATA_BLACKLIST),
		&blacklist, sizeof(DATA_BLACKLIST), nullptr, nullptr);

	if (Remove)
	{
		int index = 0;
		for (auto x : CDriver::vBlacklistProc)
		{
			if (x.compare(sProcess.c_str()) == 0)
				break;
			index++;
		}
		CDriver::vBlacklistProc.erase(CDriver::vBlacklistProc.begin() + index);
	}
	else
	{
		CDriver::vBlacklistProc.push_back(sProcess);
	}
}

bool CDriver::IsProcessAlreadyBlacklisted(std::wstring& sProcess)
{
	for (auto x : CDriver::vBlacklistProc)
	{
		if (x.compare(sProcess.c_str()) == 0)
			return true;
	}
	return false;
}

void CDriver::ThreadOperation(HANDLE TID, DWORD OperationID)
{
	DWORD Bytes = 0;
	THREAD_HANDLE handleData = { 0 };
	HANDLE hThread = 0;
	handleData.hThread = &hThread;
	handleData.TID = TID;

	if (DeviceIoControl(CDriver::hDriver, IO_REQUEST_THREAD_HANDLE, &handleData, sizeof(THREAD_HANDLE), &handleData, sizeof(THREAD_HANDLE), &Bytes, nullptr)
		&& hThread != INVALID_HANDLE_VALUE)
	{
		switch (OperationID)
		{
		case ID_KILL_THREAD:
			CDriver::NtTerminateThread(hThread, nullptr);
			break;
		case ID_SUSPEND_THREAD:
			CDriver::NtSuspendThread(hThread, nullptr);
			break;
		case ID_RESUME_THREAD:
			CDriver::NtResumeThread(hThread, nullptr);
			break;
		}
		CloseHandle(hThread);
	}
}

bool CDriver::GetModuleList(HANDLE PID)
{
	if (!CDriver::bIsInit || !PID)
		return false;
	
	if (CDriver::vModules.size())
		CDriver::vModules.clear();

	//Init driver request struct
	ULONG ArrayCount = 0;
	LIST_DATA mod = { 0 };
	mod.NodeCount = &ArrayCount;
	mod.PID = PID;
	DWORD Bytes = 0;
	if (!DeviceIoControl(CDriver::hDriver, IO_REQUEST_MODULE, &mod, sizeof(LIST_DATA), &mod, sizeof(LIST_DATA), &Bytes, nullptr))
		return false;

	//Init vector
	std::vector<LIST_MODULE>tmpVector;
	LIST_MODULE tmp = { 0 };
	for (ULONG i = 0; i < ArrayCount; i++)
		tmpVector.push_back(tmp);

	mod.buffer = &tmpVector[0];
	if (!DeviceIoControl(CDriver::hDriver, IO_REQUEST_MODULE, &mod, sizeof(LIST_DATA), &mod, sizeof(LIST_DATA), &Bytes, nullptr))
		return false;

	//Fill vector
	for (auto x : tmpVector)
		CDriver::vModules.push_back(x);

	return true;
}

bool CDriver::GetDriverList()
{
	if (!CDriver::bIsInit)
		return false;

	if (CDriver::vDriver.size())
		CDriver::vDriver.clear();

	//Init driver request struct
	ULONG ArrayCount = 0;
	LIST_DATA drv = { 0 };
	drv.NodeCount = &ArrayCount;
	DWORD Bytes = 0;
	if (!DeviceIoControl(CDriver::hDriver, IO_REQUEST_DRIVER, &drv, sizeof(LIST_DATA), &drv, sizeof(LIST_DATA), &Bytes, nullptr))
		return false;

	//Init vector
	std::vector<LIST_DRIVER>tmpVector;
	LIST_DRIVER tmp = { 0 };
	for (ULONG i = 0; i < ArrayCount; i++)
		tmpVector.push_back(tmp);

	//Fill array with all loaded drivers and their data
	drv.buffer = &tmpVector[0];
	if (!DeviceIoControl(CDriver::hDriver, IO_REQUEST_DRIVER, &drv, sizeof(LIST_DATA), &drv, sizeof(LIST_DATA), &Bytes, nullptr))
		return false;

	//Fill vector
	for (auto x : tmpVector)
		CDriver::vDriver.push_back(x);

	return true;
}

bool CDriver::GetProcList()
{
	if (!CDriver::bIsInit)
		return false;

	if (CDriver::vProcess.size())
		CDriver::vProcess.clear();

	//Init driver request struct
	ULONG ArrayCount = 0;
	LIST_DATA proc = { 0 };
	proc.NodeCount = &ArrayCount;
	DWORD Bytes = 0;
	if (!DeviceIoControl(CDriver::hDriver, IO_REQUEST_PROCESS, &proc, sizeof(LIST_DATA), &proc, sizeof(LIST_DATA), &Bytes, nullptr))
		return false;

	//Init vector
	std::vector<LIST_PROCESS>tmpVector;
	LIST_PROCESS tmp = { 0 };
	for (ULONG i = 0; i < ArrayCount; i++)
		tmpVector.push_back(tmp);

	//Fill array with all loaded processes and their data
	proc.buffer = &tmpVector[0];
	if (!DeviceIoControl(CDriver::hDriver, IO_REQUEST_PROCESS, &proc, sizeof(LIST_DATA), &proc, sizeof(LIST_DATA), &Bytes, nullptr))
		return false;

	//Fill vector
	for (auto x : tmpVector)
		CDriver::vProcess.push_back(x);

	return true;
}

bool CDriver::GetThreadList(HANDLE PID)
{
	if (!CDriver::bIsInit)
		return false;
		
	if (CDriver::vThreads.size())
		CDriver::vThreads.clear();

	//Init driver request struct
	ULONG ArrayCount = 0;
	LIST_DATA proc = { 0 };
	proc.NodeCount = &ArrayCount;
	proc.PID = PID;
	DWORD Bytes = 0;
	if (!DeviceIoControl(CDriver::hDriver, IO_REQUEST_THREAD, &proc, sizeof(LIST_DATA), &proc, sizeof(LIST_DATA), &Bytes, nullptr))
		return false;

	//Init vector
	std::vector<LIST_THREAD>tmpVector;
	tmpVector.clear();
	LIST_THREAD tmp = { 0 };
	for (ULONG i = 0; i < ArrayCount; i++)
		tmpVector.push_back(tmp);

	//Fill array with all loaded drivers and their data
	proc.buffer = &tmpVector[0];
	proc.PID = PID;
	if (!DeviceIoControl(CDriver::hDriver, IO_REQUEST_THREAD, &proc, sizeof(LIST_DATA), &proc, sizeof(LIST_DATA), &Bytes, nullptr))
		return false;

	//Fill vector
	for (auto y : tmpVector)
		CDriver::vThreads.push_back(y);

	return true;
}
