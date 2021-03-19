#include "Includes.hpp"

UNICODE_STRING Dev = RTL_CONSTANT_STRING(L"\\Device\\SysView");
UNICODE_STRING Dos = RTL_CONSTANT_STRING(L"\\DosDevices\\SysView");
PSYSVIEW SysView = nullptr;

extern "C" NTSTATUS DriverUnload(PDRIVER_OBJECT pDriverObject);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{	
	UNREFERENCED_PARAMETER(pRegistryPath);

	BOOL Init = FALSE;
	SysView = new (NonPagedPool, SYSVIEW_TAG) CSysView(&Init);
	if (!SysView || !Init)
		return STATUS_UNSUCCESSFUL;
	
	PDEVICE_OBJECT pDeviceObject = nullptr;
	IoCreateDevice(pDriverObject, 0, &Dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	IoCreateSymbolicLink(&Dos, &Dev);

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = SysControll::IoCreateCall;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = SysControll::IoCloseCall;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = SysControll::IoControll;
	pDriverObject->DriverUnload = (PDRIVER_UNLOAD)DriverUnload;

	pDeviceObject->Flags |= DO_DIRECT_IO;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}

extern "C" NTSTATUS DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	IoDeleteSymbolicLink(&Dos);
	IoDeleteDevice(pDriverObject->DeviceObject);
	SysView->~CSysView();
	delete (SysView);
	return STATUS_SUCCESS;
}
