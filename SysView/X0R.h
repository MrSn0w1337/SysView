#pragma once

#include <msclr/marshal_cppstd.h>
#include <vcclr.h>
#include "KeInterface.hpp"
#include "Threads.h"
#include "Blacklist.h"
#include "Drivers.h"
#include "Modules.h"

#pragma comment (lib, "user32.lib")

namespace X0RCSGO {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class X0R : public System::Windows::Forms::Form
	{
	public:
		X0R(void)
		{
			this->listFont = gcnew System::Drawing::Font("Segoe UI", 9.f);
			InitializeComponent();
		}

	protected:
		~X0R()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	private: System::Windows::Forms::ContextMenuStrip^ processStripMenu;

	private: System::Windows::Forms::ToolStripMenuItem^ threadsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ modulesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ killToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ suspendToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ refreshToolStripMenuItem1;


	protected:
	private: System::ComponentModel::IContainer^ components;
	private: System::Windows::Forms::ListBox^ listBox1;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::MenuStrip^ menuStripOptions;
	private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
	private: System::Windows::Forms::ListView^ processListView;
	private: System::Windows::Forms::ColumnHeader^ columnHeaderPID;
	private: System::Windows::Forms::ColumnHeader^ ImageName;
	private: System::Windows::Forms::ColumnHeader^ ImagePath;
	private: System::Windows::Forms::ColumnHeader^ Threads;
	private: System::Windows::Forms::ToolStripMenuItem^ optionsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ protectedProcessesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ blacklistedProcessesToolStripMenuItem;
	private:
		
		System::Drawing::Font^ listFont;


#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->processStripMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->threadsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modulesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->killToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->suspendToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->refreshToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->menuStripOptions = (gcnew System::Windows::Forms::MenuStrip());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->optionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->protectedProcessesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->blacklistedProcessesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->processListView = (gcnew System::Windows::Forms::ListView());
			this->columnHeaderPID = (gcnew System::Windows::Forms::ColumnHeader());
			this->ImageName = (gcnew System::Windows::Forms::ColumnHeader());
			this->ImagePath = (gcnew System::Windows::Forms::ColumnHeader());
			this->Threads = (gcnew System::Windows::Forms::ColumnHeader());
			this->processStripMenu->SuspendLayout();
			this->menuStripOptions->SuspendLayout();
			this->SuspendLayout();
			// 
			// processStripMenu
			// 
			this->processStripMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->threadsToolStripMenuItem,
					this->modulesToolStripMenuItem, this->killToolStripMenuItem, this->suspendToolStripMenuItem, this->refreshToolStripMenuItem1
			});
			this->processStripMenu->Name = L"contextMenuStrip1";
			this->processStripMenu->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->processStripMenu->Size = System::Drawing::Size(121, 114);
			this->processStripMenu->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &X0R::processStripMenu_Opening);
			// 
			// threadsToolStripMenuItem
			// 
			this->threadsToolStripMenuItem->Name = L"threadsToolStripMenuItem";
			this->threadsToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->threadsToolStripMenuItem->Text = L"Threads";
			this->threadsToolStripMenuItem->Click += gcnew System::EventHandler(this, &X0R::threadsToolStripMenuItem_Click);
			// 
			// modulesToolStripMenuItem
			// 
			this->modulesToolStripMenuItem->Name = L"modulesToolStripMenuItem";
			this->modulesToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->modulesToolStripMenuItem->Text = L"Modules";
			this->modulesToolStripMenuItem->Click += gcnew System::EventHandler(this, &X0R::modulesToolStripMenuItem_Click);
			// 
			// killToolStripMenuItem
			// 
			this->killToolStripMenuItem->Name = L"killToolStripMenuItem";
			this->killToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->killToolStripMenuItem->Text = L"Kill";
			this->killToolStripMenuItem->Click += gcnew System::EventHandler(this, &X0R::killToolStripMenuItem_Click);
			// 
			// suspendToolStripMenuItem
			// 
			this->suspendToolStripMenuItem->Name = L"suspendToolStripMenuItem";
			this->suspendToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->suspendToolStripMenuItem->Text = L"Suspend";
			this->suspendToolStripMenuItem->Click += gcnew System::EventHandler(this, &X0R::suspendToolStripMenuItem_Click);
			// 
			// refreshToolStripMenuItem1
			// 
			this->refreshToolStripMenuItem1->Name = L"refreshToolStripMenuItem1";
			this->refreshToolStripMenuItem1->Size = System::Drawing::Size(120, 22);
			this->refreshToolStripMenuItem1->Text = L"Refresh";
			this->refreshToolStripMenuItem1->Click += gcnew System::EventHandler(this, &X0R::refreshToolStripMenuItem1_Click);
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->ItemHeight = 16;
			this->listBox1->Location = System::Drawing::Point(6, 16);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(388, 100);
			this->listBox1->TabIndex = 0;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(6, 128);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(241, 20);
			this->textBox1->TabIndex = 1;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(0, 0);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(0, 0);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 0;
			// 
			// menuStripOptions
			// 
			this->menuStripOptions->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->exitToolStripMenuItem,
					this->optionsToolStripMenuItem
			});
			this->menuStripOptions->Location = System::Drawing::Point(0, 0);
			this->menuStripOptions->Name = L"menuStripOptions";
			this->menuStripOptions->Padding = System::Windows::Forms::Padding(0);
			this->menuStripOptions->Size = System::Drawing::Size(1004, 24);
			this->menuStripOptions->TabIndex = 2;
			this->menuStripOptions->Text = L"menuStrip1";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(38, 24);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &X0R::exitToolStripMenuItem_Click);
			// 
			// optionsToolStripMenuItem
			// 
			this->optionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->protectedProcessesToolStripMenuItem,
					this->blacklistedProcessesToolStripMenuItem
			});
			this->optionsToolStripMenuItem->Name = L"optionsToolStripMenuItem";
			this->optionsToolStripMenuItem->Size = System::Drawing::Size(61, 24);
			this->optionsToolStripMenuItem->Text = L"Options";
			// 
			// protectedProcessesToolStripMenuItem
			// 
			this->protectedProcessesToolStripMenuItem->Name = L"protectedProcessesToolStripMenuItem";
			this->protectedProcessesToolStripMenuItem->Size = System::Drawing::Size(184, 22);
			this->protectedProcessesToolStripMenuItem->Text = L"Protected Processes";
			this->protectedProcessesToolStripMenuItem->Click += gcnew System::EventHandler(this, &X0R::protectedProcessesToolStripMenuItem_Click);
			// 
			// blacklistedProcessesToolStripMenuItem
			// 
			this->blacklistedProcessesToolStripMenuItem->Name = L"blacklistedProcessesToolStripMenuItem";
			this->blacklistedProcessesToolStripMenuItem->Size = System::Drawing::Size(184, 22);
			this->blacklistedProcessesToolStripMenuItem->Text = L"Blacklisted Processes";
			this->blacklistedProcessesToolStripMenuItem->Click += gcnew System::EventHandler(this, &X0R::blacklistedProcessesToolStripMenuItem_Click);
			// 
			// processListView
			// 
			this->processListView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {
				this->columnHeaderPID,
					this->ImageName, this->ImagePath, this->Threads
			});
			this->processListView->ContextMenuStrip = this->processStripMenu;
			this->processListView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->processListView->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->processListView->FullRowSelect = true;
			this->processListView->HideSelection = false;
			this->processListView->Location = System::Drawing::Point(0, 24);
			this->processListView->Name = L"processListView";
			this->processListView->Size = System::Drawing::Size(1004, 513);
			this->processListView->TabIndex = 3;
			this->processListView->UseCompatibleStateImageBehavior = false;
			this->processListView->View = System::Windows::Forms::View::Details;
			// 
			// columnHeaderPID
			// 
			this->columnHeaderPID->Text = L"PID";
			// 
			// ImageName
			// 
			this->ImageName->Text = L"Image Name";
			this->ImageName->Width = 250;
			// 
			// ImagePath
			// 
			this->ImagePath->Text = L"Image Path";
			this->ImagePath->Width = 350;
			// 
			// Threads
			// 
			this->Threads->Text = L"Threads";
			// 
			// X0R
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1004, 537);
			this->Controls->Add(this->processListView);
			this->Controls->Add(this->menuStripOptions);
			this->MainMenuStrip = this->menuStripOptions;
			this->Name = L"X0R";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"SysView";
			this->Load += gcnew System::EventHandler(this, &X0R::X0R_Load);
			this->processStripMenu->ResumeLayout(false);
			this->menuStripOptions->ResumeLayout(false);
			this->menuStripOptions->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void X0R_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		String^ sTitle = "SysView [";
		sTitle = sTitle + Environment::UserName + "/" + Environment::MachineName + "]";
		this->Text = sTitle;
		this->refreshToolStripMenuItem1_Click(sender, e);
	}

private: System::Void refreshToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e) 
{
	//Get list of processes
	if (!CDriver::GetProcList())
	{
		MessageBoxA(GetForegroundWindow(), "Failed to get processes", "SysView", MB_OK | MB_ICONERROR);
		return;
	}

	//Clear listview
	this->processListView->Items->Clear();
	auto pProcVec = CDriver::GetProcVector();
	for (auto& x : pProcVec)
	{
		auto sName = gcnew String(x.ImageName);
		auto sPath = gcnew String(x.ImagePath);
		auto sPID = System::Convert::ToString(reinterpret_cast<unsigned long long>(x.PID));
		auto sThreads = System::Convert::ToString(static_cast<unsigned long long>(x.ThreadsCount));
		auto listItem = gcnew System::Windows::Forms::ListViewItem(sPID);
		listItem->SubItems->Add(sName);
		listItem->SubItems->Add(sPath);
		listItem->SubItems->Add(sThreads);
		
		if (x.IsSuspended)
		{
			listItem->ForeColor = Color::White;
			listItem->BackColor = Color::Gray;
		}
		
		listItem->Font = this->listFont;
		this->processListView->Items->Add(listItem);
	}
	pProcVec.clear();
}
private: System::Void killToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	//Kill Process
	if (!this->processListView->SelectedItems->Count)
	{
		MessageBoxA(GetForegroundWindow(), "No process selected", "SysView", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = this->processListView->SelectedIndices[0]; this->processListView->SelectedIndices->Contains(i); i++)
	{
		long long PID = 0;
		if (System::Int64::TryParse(this->processListView->Items[i]->Text, PID))
			CDriver::KillProcess((HANDLE)PID);
	}

	//Refresh list
	this->refreshToolStripMenuItem1_Click(sender, e);
}
private: System::Void suspendToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	//Suspend Process
	if (!this->processListView->SelectedItems)
	{
		MessageBoxA(GetForegroundWindow(), "No process selected", "SysView", MB_OK | MB_ICONERROR);
		return;
	}
	
	for (int i = this->processListView->SelectedIndices[0]; this->processListView->SelectedIndices->Contains(i); i++)
	{
		long long PID = 0;
		if (System::Int64::TryParse(this->processListView->Items[i]->Text, PID))
		{
			if (processListView->Items[i]->ForeColor == Color::White)
				CDriver::ResumeProcess((HANDLE)PID);
			else
				CDriver::SuspendProcess((HANDLE)PID);
		}
	}

	//Refresh list
	this->refreshToolStripMenuItem1_Click(sender, e);
}
private: System::Void processStripMenu_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) 
{
	if (!this->processListView->SelectedItems->Count)
		return;

	int Index = this->processListView->SelectedIndices[0];
	if (this->processListView->Items[Index]->ForeColor == Color::White)
		this->suspendToolStripMenuItem->Text = "Resume";
	else
		this->suspendToolStripMenuItem->Text = "Suspend";
}
private: System::Void threadsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	if (!this->processListView->SelectedItems->Count)
	{
		MessageBoxA(GetForegroundWindow(), "No process selected", "SysView", MB_OK | MB_ICONERROR);
		return;
	}
	else if (this->processListView->SelectedItems->Count > 1)
	{
		MessageBoxA(GetForegroundWindow(), "Please select only one process", "SysView", MB_OK | MB_ICONINFORMATION);
		return;
	}

	long long PID = 0;
	if (System::Int64::TryParse(this->processListView->SelectedItems[0]->Text, PID))
	{
		Form^ thForm = gcnew SysView::Threads((HANDLE)PID);
		thForm->ShowDialog();
	}
}
private: System::Void protectedProcessesToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{

}
private: System::Void blacklistedProcessesToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	auto blacklistForm = gcnew SysViewBlacklist::Blacklist();
	blacklistForm->ShowDialog();
}
private: System::Void modulesToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	if (!this->processListView->SelectedItems->Count)
	{
		MessageBoxA(GetForegroundWindow(), "No process selected", "SysView", MB_OK | MB_ICONERROR);
		return;
	}
	else if (this->processListView->SelectedItems->Count > 1)
	{
		MessageBoxA(GetForegroundWindow(), "Please select only one process", "SysView", MB_OK | MB_ICONINFORMATION);
		return;
	}

	long long PID = 0;
	if (System::Int64::TryParse(this->processListView->SelectedItems[0]->Text, PID))
	{
		if (PID == 4)
		{
			auto driverForm = gcnew SysViewDriver::Drivers();
			driverForm->ShowDialog();
		}
		else
		{
			auto moduleForm = gcnew SysViewModule::Modules((HANDLE)PID);
			moduleForm->ShowDialog();
		}
	}
}
private: System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	auto result = MessageBoxA(GetForegroundWindow(), "Are you sure to exit?", "SysView", MB_YESNO | MB_ICONINFORMATION);
	if (result == IDYES)
		Application::Exit();
}
};
}
