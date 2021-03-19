#include "Includes.hpp"

void* __cdecl operator new(size_t size, POOL_TYPE pool, ULONG tag) 
{
	PVOID newAddress;

	newAddress = ExAllocatePoolWithTag(pool, size, tag);
	//
	// Remove remenants from previous use.
	//
	if (newAddress)
	{
		memset(newAddress, 0, size);
	}
	return newAddress;
}

void __cdecl operator delete(void* p, unsigned __int64) 
{
	ExFreePool(p);
}

PLIST_PROCESS CSysList::ProcessHead;
PLIST_PROCESS CSysList::ProcessLast;
PLIST_THREAD CSysList::ThreadHead;
PLIST_THREAD CSysList::ThreadLast;
PLIST_MODULE CSysList::ModuleHead;
PLIST_MODULE CSysList::ModuleLast;
PLIST_BLACKLIST CSysList::BlacklistHead;
PLIST_BLACKLIST CSysList::BlacklistLast;
PLIST_PROTECT CSysList::ProtectHead;
PLIST_PROTECT CSysList::ProtectLast;
PLIST_DRIVER CSysList::DriverHead;
PLIST_DRIVER CSysList::DriverLast;

CSysList::CSysList()
{
	CSysList::ProcessHead = nullptr;
	CSysList::ProcessLast = nullptr;
	CSysList::ThreadHead = nullptr;
	CSysList::ThreadLast = nullptr;
	CSysList::ModuleHead = nullptr;
	CSysList::ModuleLast = nullptr;
	CSysList::BlacklistHead = nullptr;
	CSysList::BlacklistLast = nullptr;
	CSysList::ProtectHead = nullptr;
	CSysList::ProtectLast = nullptr;
	CSysList::DriverHead = nullptr;
	CSysList::DriverLast = nullptr;
}

CSysList::~CSysList()
{
	CSysList::Erease(ID::PROTECT);
	CSysList::Erease(ID::BLACKLIST);
}

void* CSysList::Alloc(size_t size, POOL_TYPE pool, ULONG Tag)
{
	return ExAllocatePoolWithTag(pool, size, Tag);
}

void CSysList::Free(void* p, ULONG Tag)
{
	ExFreePoolWithTag(p, Tag);
}

void CSysList::Insert(void* pEntry, USHORT ID)
{
	if (ID == ID::PROCESS)
	{
		auto tmpProc = (PLIST_PROCESS)CSysList::Alloc(sizeof(LIST_PROCESS), NonPagedPool, TAG::PROCESS);
		if (!tmpProc)
			return;

		RtlCopyMemory(tmpProc, pEntry, sizeof(LIST_PROCESS));

		if (!CSysList::ProcessHead)
		{
			tmpProc->Next = nullptr;
			CSysList::ProcessHead = tmpProc;
			CSysList::ProcessLast = tmpProc;
			return;
		}

		tmpProc->Next = nullptr;
		tmpProc->Last = CSysList::ProcessLast;
		CSysList::ProcessLast->Next = tmpProc;
		CSysList::ProcessLast = tmpProc;
	}
	else if (ID == ID::DRIVER)
	{
		auto tmpDriver = (PLIST_DRIVER)CSysList::Alloc(sizeof(LIST_DRIVER), NonPagedPool, TAG::DRIVER);
		if (!tmpDriver)
			return;

		RtlCopyMemory(tmpDriver, pEntry, sizeof(LIST_DRIVER));

		if (!CSysList::DriverHead)
		{
			tmpDriver->Next = nullptr;
			CSysList::DriverHead = tmpDriver;
			CSysList::DriverLast = tmpDriver;
			return;
		}

		tmpDriver->Next = nullptr;
		tmpDriver->Last = CSysList::DriverLast;
		CSysList::DriverLast->Next = tmpDriver;
		CSysList::DriverLast = tmpDriver;
	}
	else if (ID == ID::PROTECT)
	{
		auto tmpProtect = (PLIST_PROTECT)CSysList::Alloc(sizeof(LIST_PROTECT), NonPagedPool, TAG::PROTECT);
		if (!tmpProtect)
			return;

		RtlCopyMemory(tmpProtect, pEntry, sizeof(LIST_PROTECT));

		if (!CSysList::ProtectLast)
		{
			tmpProtect->Next = nullptr;
			CSysList::ProtectHead = tmpProtect;
			CSysList::ProtectLast = tmpProtect;
			return;
		}

		tmpProtect->Next = nullptr;
		tmpProtect->Last = CSysList::ProtectLast;
		CSysList::ProtectLast->Next = tmpProtect;
		CSysList::ProtectLast = tmpProtect;
	}
	else if (ID == ID::BLACKLIST)
	{
		auto tmpBlacklist = (PLIST_BLACKLIST)CSysList::Alloc(sizeof(LIST_BLACKLIST), NonPagedPool, TAG::BLACKLIST);
		if (!tmpBlacklist)
			return;

		RtlCopyMemory(tmpBlacklist, pEntry, sizeof(LIST_BLACKLIST));

		if (!CSysList::BlacklistHead)
		{
			tmpBlacklist->Next = nullptr;
			CSysList::BlacklistHead = tmpBlacklist;
			CSysList::BlacklistLast = tmpBlacklist;
			return;
		}

		tmpBlacklist->Next = nullptr;
		tmpBlacklist->Last = BlacklistLast;
		CSysList::BlacklistLast->Next = tmpBlacklist;
		CSysList::BlacklistLast = tmpBlacklist;
	}
	else if(ID == ID::THREAD)
	{
		auto tmpTh = (PLIST_THREAD)CSysList::Alloc(sizeof(LIST_THREAD), NonPagedPool, TAG::THREAD);
		if (!tmpTh)
			return;

		RtlCopyMemory(tmpTh, pEntry, sizeof(LIST_THREAD));

		if (!CSysList::ThreadHead)
		{
			tmpTh->Next = nullptr;
			CSysList::ThreadHead = tmpTh;
			CSysList::ThreadLast = tmpTh;
			return;
		}

		tmpTh->Next = nullptr;
		tmpTh->Last = ThreadLast;
		CSysList::ThreadLast->Next = tmpTh;
		CSysList::ThreadLast = tmpTh;
	}
	else
	{
		auto tmpMod = (PLIST_MODULE)CSysList::Alloc(sizeof(LIST_MODULE), NonPagedPool, TAG::MODULE);
		if (!tmpMod)
			return;

		RtlCopyMemory(tmpMod, pEntry, sizeof(LIST_MODULE));

		if (!CSysList::ModuleHead)
		{
			tmpMod->Next = nullptr;
			CSysList::ModuleHead = tmpMod;
			CSysList::ModuleLast = tmpMod;
			return;
		}

		tmpMod->Next = nullptr;
		tmpMod->Last = ModuleLast;
		CSysList::ModuleLast->Next = tmpMod;
		CSysList::ModuleLast = tmpMod;
	}
}

void CSysList::Erease(USHORT ID)
{
	if (!CSysList::GetHead(ID) || !CSysList::GetLast(ID))
		return;

	if (ID == ID::PROCESS)
	{
		auto currProcEntry = CSysList::ProcessHead;
		while (currProcEntry)
		{
			auto nextEntry = currProcEntry->Next;
			CSysList::Free(currProcEntry, TAG::PROCESS);
			currProcEntry = nextEntry;
		}
	}
	else if (ID == ID::DRIVER)
	{
		auto currDriverEntry = CSysList::DriverHead;
		while (currDriverEntry)
		{
			auto nextEntry = currDriverEntry->Next;
			CSysList::Free(currDriverEntry, TAG::DRIVER);
			currDriverEntry = nextEntry;
		}
	}
	else if (ID == ID::PROTECT)
	{
		auto currProtectEntry = CSysList::ProtectHead;
		while (currProtectEntry)
		{
			auto nextEntry = currProtectEntry->Next;
			CSysList::Free(currProtectEntry, TAG::PROTECT);
			currProtectEntry = nextEntry;
		}
	}
	else if (ID == ID::BLACKLIST)
	{
		auto currBlacklistEntry = CSysList::BlacklistHead;
		while (currBlacklistEntry)
		{
			auto nextEntry = currBlacklistEntry->Next;
			CSysList::Free(currBlacklistEntry, TAG::BLACKLIST);
			currBlacklistEntry = nextEntry;
		}
	}
	else if(ID == ID::THREAD)
	{
		auto currThreadEntry = CSysList::ThreadHead;
		while (currThreadEntry)
		{
			auto nextEntry = currThreadEntry->Next;
			CSysList::Free(currThreadEntry, TAG::THREAD);
			currThreadEntry = nextEntry;
		}
	}
	else
	{
		auto currModEntry = CSysList::ModuleHead;
		while (currModEntry)
		{
			auto nextEntry = currModEntry->Next;
			CSysList::Free(currModEntry, TAG::MODULE);
			currModEntry = nextEntry;
		}
	}

	CSysList::SetHead(ID, nullptr);
	CSysList::SetLast(ID, nullptr);
}

ULONG CSysList::GetCount(USHORT ID)
{
	ULONG ListCount = 0;

	if (!CSysList::GetHead(ID))
		return ListCount;

	if (ID == ID::PROCESS)
	{
		auto currProcEntry = CSysList::ProcessHead;
		while (currProcEntry)
		{
			ListCount++;
			currProcEntry = currProcEntry->Next;
		}
	}
	else if (ID == DRIVER)
	{
		auto currDriverEntry = CSysList::DriverHead;
		while (currDriverEntry)
		{
			ListCount++;
			currDriverEntry = currDriverEntry->Next;
		}
	}
	else if (ID == PROTECT)
	{
		auto currProtectEntry = CSysList::ProtectHead;
		while (currProtectEntry)
		{
			ListCount++;
			currProtectEntry = currProtectEntry->Next;
		}
	}
	else if (ID == BLACKLIST)
	{
		auto currBlacklistEntry = CSysList::BlacklistHead;
		while (currBlacklistEntry)
		{
			ListCount++;
			currBlacklistEntry = currBlacklistEntry->Next;
		}
	}
	else if(ID == THREAD)
	{
		auto currThreadEntry = CSysList::ThreadHead;
		while (currThreadEntry)
		{
			ListCount++;
			currThreadEntry = currThreadEntry->Next;
		}
	}
	else
	{
		auto currModEntry = CSysList::ModuleHead;
		while (currModEntry)
		{
			ListCount++;
			currModEntry = currModEntry->Next;
		}
	}

	return ListCount;
}

void* CSysList::Get(ULONG index, USHORT ID) 
{
	void* foundEntry = nullptr;
	if (!CSysList::GetHead(ID))
		return foundEntry;

	if (ID == ID::PROCESS)
	{
		auto currProcEntry = CSysList::ProcessHead;
		for (ULONG i = 0; currProcEntry; i++)
		{
			if (i == index)
			{
				foundEntry = currProcEntry;
				break;
			}
			currProcEntry = currProcEntry->Next;
		}
	}
	else if (ID == ID::DRIVER)
	{
		auto currDriverEntry = CSysList::DriverHead;
		for (ULONG i = 0; currDriverEntry; i++)
		{
			if (i == index)
			{
				foundEntry = currDriverEntry;
				break;
			}
			currDriverEntry = currDriverEntry->Next;
		}
	}
	else if(ID == ID::THREAD)
	{
		auto currThreadEntry = CSysList::ThreadHead;
		for (ULONG i = 0; currThreadEntry; i++)
		{
			if (i == index)
			{
				foundEntry = currThreadEntry;
				break;
			}
			currThreadEntry = currThreadEntry->Next;
		}
	}
	else
	{
		auto currModEntry = CSysList::ModuleHead;
		for (ULONG i = 0; currModEntry; i++)
		{
			if (i == index)
			{
				foundEntry = currModEntry;
				break;
			}
			currModEntry = currModEntry->Next;
		}
	}

	return foundEntry;
}

void CSysList::Remove(void* Entry, USHORT ID)
{
	if (!CSysList::GetHead(ID))
		return;

	if (CSysList::GetHead(ID) == CSysList::GetLast(ID))
	{
		CSysList::Erease(ID);
		return;
	}

	if (ID == ID::PROCESS)
	{
		auto ProcID = *(PHANDLE)Entry;
		auto currProcEntry = CSysList::ProcessHead;
		while (currProcEntry)
		{
			if (currProcEntry->PID == ProcID)
			{
				auto NodeLast = currProcEntry->Last;
				auto NodeNext = currProcEntry->Next;

				if (!NodeNext)
				{
					NodeLast->Next = nullptr;
					CSysList::ProcessLast = NodeLast;
				}
				else if (!NodeLast)
				{
					NodeNext->Last = nullptr;
					CSysList::ProcessHead = NodeNext;
				}
				else
				{
					NodeLast->Next = NodeNext;
					NodeNext->Last = NodeLast;
				}

				CSysList::Free(currProcEntry, TAG::PROCESS);
				currProcEntry = nullptr;
				break;
			}
			currProcEntry = currProcEntry->Next;
		}
	}
	else if (ID == ID::DRIVER)
	{
		auto drvBase = (uintptr_t)Entry;
		auto currDriverEntry = CSysList::DriverHead;
		while (currDriverEntry)
		{
			if (currDriverEntry->ImageBase == drvBase)
			{
				auto NodeLast = currDriverEntry->Last;
				auto NodeNext = currDriverEntry->Next;
				
				if (!NodeNext)
				{
					NodeLast->Next = nullptr;
					CSysList::DriverLast = NodeLast;
				}
				else if (!NodeLast)
				{
					NodeNext->Last = nullptr;
					CSysList::DriverHead = NodeNext;
				}
				else
				{
					NodeLast->Next = NodeNext;
					NodeNext->Last = NodeLast;
				}

				CSysList::Free(currDriverEntry, TAG::DRIVER);
				currDriverEntry = nullptr;
				break;
			}
			currDriverEntry = currDriverEntry->Next;
		}
	}
	else if (ID == ID::PROTECT)
	{
		auto ProcID = *(PHANDLE)Entry;
		auto currProtectEntry = CSysList::ProtectHead;
		while (currProtectEntry)
		{
			if (currProtectEntry->PID == ProcID)
			{
				auto NodeLast = currProtectEntry->Last;
				auto NodeNext = currProtectEntry->Next;

				if (!NodeNext)
				{
					NodeLast->Next = nullptr;
					CSysList::ProtectLast = NodeLast;
				}
				else if (!NodeLast)
				{
					NodeNext->Last = nullptr;
					CSysList::ProtectHead = NodeNext;
				}
				else
				{
					NodeLast->Next = NodeNext;
					NodeNext->Last = NodeLast;
				}

				CSysList::Free(currProtectEntry, TAG::PROTECT);
				currProtectEntry = nullptr;
				break;
			}
			currProtectEntry = currProtectEntry->Next;
		}
	}
	else if (ID == ID::BLACKLIST)
	{
		auto wName = (wchar_t*)Entry;
		auto currBlacklistEntry = CSysList::BlacklistHead;
		while (currBlacklistEntry)
		{
			if (wcscmp(wName, currBlacklistEntry->wName) == 0)
			{
				auto NodeLast = currBlacklistEntry->Last;
				auto NodeNext = currBlacklistEntry->Next;

				if (!NodeNext)
				{
					NodeLast->Next = nullptr;
					CSysList::BlacklistLast = NodeLast;
				}
				else if (!NodeLast)
				{
					NodeNext->Last = nullptr;
					CSysList::BlacklistHead = NodeNext;
				}
				else
				{
					NodeLast->Next = NodeNext;
					NodeNext->Last = NodeLast;
				}

				CSysList::Free(currBlacklistEntry, TAG::BLACKLIST);
				currBlacklistEntry = nullptr;
				break;
			}

			currBlacklistEntry = currBlacklistEntry->Next;
		}
	}
	else if(ID == THREAD)
	{
		auto ThreadID = *(PHANDLE)Entry;
		auto currThreadEntry = CSysList::ThreadHead;
		while (currThreadEntry)
		{
			if (currThreadEntry->TID == ThreadID)
			{
				auto NodeLast = currThreadEntry->Last;
				auto NodeNext = currThreadEntry->Next;

				if (!NodeNext)
				{
					NodeLast->Next = nullptr;
					CSysList::ThreadLast = NodeLast;
				}
				else if (!NodeLast)
				{
					NodeNext->Last = nullptr;
					CSysList::ThreadHead = NodeNext;
				}
				else
				{
					NodeLast->Next = NodeNext;
					NodeNext->Last = NodeLast;
				}

				CSysList::Free(currThreadEntry, TAG::THREAD);
				currThreadEntry = nullptr;
				break;
			}
			currThreadEntry = currThreadEntry->Next;
		}
	}
	else
	{
		auto modBase = (uintptr_t)Entry;
		auto currModEntry = CSysList::ModuleHead;
		while (currModEntry)
		{
			if (currModEntry->DllBase == modBase)
			{
				auto NodeLast = currModEntry->Last;
				auto NodeNext = currModEntry->Next;

				if (!NodeNext)
				{
					NodeLast->Next = nullptr;
					CSysList::ModuleLast = NodeLast;
				}
				else if (!NodeLast)
				{
					NodeNext->Last = nullptr;
					CSysList::ModuleHead = NodeNext;
				}
				else
				{
					NodeLast->Next = NodeNext;
					NodeNext->Last = NodeLast;
				}

				CSysList::Free(currModEntry, TAG::MODULE);
				currModEntry = nullptr;
				break;
			}
			currModEntry = currModEntry->Next;
		}
	}
}

void* CSysList::GetHead(USHORT ID)
{
	switch (ID)
	{
	case ID::BLACKLIST:
		return CSysList::BlacklistHead;
	case ID::PROTECT:
		return CSysList::ProtectHead;
	case ID::DRIVER:
		return CSysList::DriverHead;
	case ID::MODULE:
		return CSysList::ModuleHead;
	case ID::THREAD:
		return CSysList::ThreadHead;
	case ID::PROCESS:
		return CSysList::ProcessHead;
	default:
		return nullptr;
	}
}

void* CSysList::GetLast(USHORT ID)
{
	switch (ID)
	{
	case ID::BLACKLIST:
		return CSysList::BlacklistLast;
	case ID::PROTECT:
		return CSysList::ProtectLast;
	case ID::DRIVER:
		return CSysList::DriverLast;
	case ID::MODULE:
		return CSysList::ModuleLast;
	case ID::THREAD:
		return CSysList::ThreadLast;
	case ID::PROCESS:
		return CSysList::ProcessLast;
	default:
		return nullptr;
	}
}

template <typename T>
void CSysList::SetHead(USHORT ID, T p)
{
	switch (ID)
	{
	case ID::BLACKLIST:
		CSysList::BlacklistHead = p;
		break;
	case ID::PROTECT:
		CSysList::ProtectHead = p;
		break;
	case ID::DRIVER:
		CSysList::DriverHead = p;
		break;
	case ID::MODULE:
		CSysList::ModuleHead = p;
		break;
	case ID::THREAD:
		CSysList::ThreadHead = p;
		break;
	case ID::PROCESS:
		CSysList::ProcessHead = p;
		break;
	default:
		break;
	}
}

template <typename X>
void CSysList::SetLast(USHORT ID, X p)
{
	switch (ID)
	{
	case ID::BLACKLIST:
		CSysList::BlacklistLast = p;
		break;
	case ID::PROTECT:
		CSysList::ProtectLast = p;
		break;
	case ID::DRIVER:
		CSysList::DriverLast = p;
		break;
	case ID::MODULE:
		CSysList::ModuleLast = p;
		break;
	case ID::THREAD:
		CSysList::ThreadLast = p;
		break;
	case ID::PROCESS:
		CSysList::ProcessLast = p;
		break;
	default:
		break;
	}
}