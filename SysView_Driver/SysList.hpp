#pragma once

void* __cdecl operator new(size_t size, POOL_TYPE pool, ULONG tag);
void __cdecl operator delete(void* p, unsigned __int64);

typedef class CSysList
{
	enum TAG
	{
		PROCESS = 0x555,
		MODULE,
		DRIVER,
		BLACKLIST,
		PROTECT,
		THREAD
	};

protected:
	static PLIST_PROCESS ProcessHead;
	static PLIST_PROCESS ProcessLast;
	static PLIST_THREAD ThreadHead;
	static PLIST_THREAD ThreadLast;
	static PLIST_MODULE ModuleHead;
	static PLIST_MODULE ModuleLast;
	static PLIST_BLACKLIST BlacklistHead;
	static PLIST_BLACKLIST BlacklistLast;
	static PLIST_PROTECT ProtectHead;
	static PLIST_PROTECT ProtectLast;
	static PLIST_DRIVER DriverHead;
	static PLIST_DRIVER DriverLast;

	CSysList();
	~CSysList();
	static void* Alloc(size_t, POOL_TYPE pool, ULONG Tag);
	static void Free(void* p, ULONG Tag);
	static ULONG GetCount(USHORT ID);
	static void* GetHead(USHORT ID);
	static void* GetLast(USHORT ID);

	template <typename T>
	static void SetHead(USHORT ID, T p);
	template <typename X>
	static void SetLast(USHORT ID, X p);

public:
	static void* Get(ULONG index, USHORT ID);
	static void Erease(USHORT ID);
	static void Insert(void* pEntry, USHORT ID);
	static void Remove(void* Entry, USHORT ID);

}SYSLIST, *PSYSLIST;