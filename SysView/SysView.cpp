#include "Includes.h"
#include "X0R.h"
#include "Threads.h"

//Namespaces
using namespace std;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;

//Globals
CDriver Driver;

[STAThread]
int main()
{
	//Console
	SetConsoleTitleA("SysView");
	printf("[SysView] Loading driver...\n");
	Sleep(1000);

	if (!CDriver::IsInit())
	{
		printf("[-] Failed to initialize driver (0x%d)\n\n", GetLastError());
		system("pause");
		ExitProcess(1);
	}

	FreeConsole();
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew X0RCSGO::X0R);
}