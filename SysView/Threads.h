#pragma once

#include "KeInterface.hpp"
#include "Private.hpp"

#pragma comment(lib, "user32.lib")

namespace SysView {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Threads : public System::Windows::Forms::Form
	{
	public:
		Threads(HANDLE PID)
		{
			//Store PID
			this->myPID = PID;
			this->listFont = gcnew System::Drawing::Font("Microsoft Sans Serif", 9.f);
			InitializeComponent();
		}

	protected:
		~Threads()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^ threadListView;
	protected:

	protected:
	private: System::Windows::Forms::ColumnHeader^ columnHeader1;
	private: System::Windows::Forms::ColumnHeader^ columnHeader2;
	private: System::Windows::Forms::ColumnHeader^ columnHeader3;
	private: System::Windows::Forms::ContextMenuStrip^ threadMenuStrip;

	private: System::Windows::Forms::ToolStripMenuItem^ killToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ suspendToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^ refreshToolStripMenuItem;
	private: System::ComponentModel::IContainer^ components;


	private:
		HANDLE myPID;
	private: System::Windows::Forms::ColumnHeader^ columnHeader4;
		   System::Drawing::Font^ listFont;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->threadListView = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->threadMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->killToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->suspendToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->refreshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->threadMenuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// threadListView
			// 
			this->threadListView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {
				this->columnHeader1,
					this->columnHeader2, this->columnHeader3, this->columnHeader4
			});
			this->threadListView->ContextMenuStrip = this->threadMenuStrip;
			this->threadListView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->threadListView->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->threadListView->FullRowSelect = true;
			this->threadListView->HideSelection = false;
			this->threadListView->Location = System::Drawing::Point(0, 0);
			this->threadListView->Name = L"threadListView";
			this->threadListView->Size = System::Drawing::Size(1004, 537);
			this->threadListView->TabIndex = 1;
			this->threadListView->UseCompatibleStateImageBehavior = false;
			this->threadListView->View = System::Windows::Forms::View::Details;
			this->threadListView->SelectedIndexChanged += gcnew System::EventHandler(this, &Threads::threadListView_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"TID";
			this->columnHeader1->Width = 80;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Start Address";
			this->columnHeader2->Width = 300;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"State";
			this->columnHeader3->Width = 146;
			// 
			// threadMenuStrip
			// 
			this->threadMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->killToolStripMenuItem,
					this->suspendToolStripMenuItem, this->refreshToolStripMenuItem
			});
			this->threadMenuStrip->Name = L"threadMenuStrip1";
			this->threadMenuStrip->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->threadMenuStrip->Size = System::Drawing::Size(181, 92);
			this->threadMenuStrip->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &Threads::threadMenuStrip_Opening);
			// 
			// killToolStripMenuItem
			// 
			this->killToolStripMenuItem->Name = L"killToolStripMenuItem";
			this->killToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->killToolStripMenuItem->Text = L"Kill";
			this->killToolStripMenuItem->Click += gcnew System::EventHandler(this, &Threads::killToolStripMenuItem_Click);
			// 
			// suspendToolStripMenuItem
			// 
			this->suspendToolStripMenuItem->Name = L"suspendToolStripMenuItem";
			this->suspendToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->suspendToolStripMenuItem->Text = L"Suspend";
			this->suspendToolStripMenuItem->Click += gcnew System::EventHandler(this, &Threads::suspendToolStripMenuItem_Click);
			// 
			// refreshToolStripMenuItem
			// 
			this->refreshToolStripMenuItem->Name = L"refreshToolStripMenuItem";
			this->refreshToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->refreshToolStripMenuItem->Text = L"Refresh";
			this->refreshToolStripMenuItem->Click += gcnew System::EventHandler(this, &Threads::refreshToolStripMenuItem_Click);
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"Module";
			this->columnHeader4->Width = 150;
			// 
			// Threads
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1004, 537);
			this->Controls->Add(this->threadListView);
			this->Name = L"Threads";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Threads";
			this->Load += gcnew System::EventHandler(this, &Threads::Threads_Load);
			this->threadMenuStrip->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Threads_Load(System::Object^ sender, System::EventArgs^ e) 
	{
		auto sTitle = System::Convert::ToString(reinterpret_cast<unsigned long long>(this->myPID));
		this->Text = "SysView - Threads [" + sTitle + "]";

		//Gather threads
		this->refreshToolStripMenuItem_Click(sender, e);
	}
	private: System::Void threadListView_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void threadMenuStrip_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) 
{
	if (!this->threadListView->SelectedItems->Count)
		return;

	int Index = this->threadListView->SelectedIndices[0];
	if (this->threadListView->Items[Index]->ForeColor == Color::White) //Suspended
		this->suspendToolStripMenuItem->Text = "Resume";
	else
		this->suspendToolStripMenuItem->Text = "Suspend";
}
private: System::Void refreshToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	if (!CDriver::GetThreadList(this->myPID))
	{
		MessageBoxA(GetForegroundWindow(), "Could not obtain target process threads", "SysView", MB_OK | MB_ICONERROR);
		return;
	}

	this->threadListView->Items->Clear();
	auto vThreads = CDriver::GetThreadVector();

	for (auto x : vThreads)
	{
		if (!x.TID)
			continue;

		auto sTID = System::Convert::ToString(reinterpret_cast<unsigned long long>(x.TID));
		auto sAddress = x.StartAddress.ToString("X");

		String^ sModule = "";
		if(this->myPID != (HANDLE)4)
			sModule = gcnew String(x.ModuleName);
		else
			sModule = gcnew String(x.szModuleName);

		auto thItem = gcnew System::Windows::Forms::ListViewItem(sTID);
		thItem->SubItems->Add(sAddress);

		//Get thread status
		String^ sStatus = "";
		switch (x.Status)
		{
		case KTHREAD_STATE::Initialized:
			sStatus = "Initialized";
			break;
		case KTHREAD_STATE::Ready:
			sStatus = "Ready";
			break;
		case KTHREAD_STATE::Running:
			sStatus = "Running";
			break;
		case KTHREAD_STATE::Standby:
			sStatus = "Standby";
			break;
		case KTHREAD_STATE::Terminated:
			sStatus = "Terminated";
			break;
		case KTHREAD_STATE::Waiting:
			sStatus = "Waiting";
			break;
		case KTHREAD_STATE::Transition:
			sStatus = "Transition";
			break;
		case KTHREAD_STATE::DeferredReady:
			sStatus = "DeferredReady";
			break;
		case KTHREAD_STATE::GateWaitObsolete:
			sStatus = "GateWaitObsolete";
			break;
		case KTHREAD_STATE::WaitingForProcessInSwap:
			sStatus = "WaitingForProcessInSwap";
			break;
		case KTHREAD_STATE::MaximumThreadState:
			sStatus = "MaximumThreadState";
			break;
		default:
			sStatus = "Unknown";
			break;
		}

		thItem->SubItems->Add(sStatus);

		if (x.IsSuspended)
		{
			thItem->ForeColor = Color::White;
			thItem->BackColor = Color::Gray;
		}
		else if (sModule == "Unknown")
		{
			thItem->ForeColor = Color::Red;
		}

		thItem->SubItems->Add(sModule);
		thItem->Font = this->listFont;
		this->threadListView->Items->Add(thItem);
	}
	vThreads.clear();
}
private: System::Void killToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	if (!this->threadListView->SelectedItems->Count)
	{
		MessageBoxA(GetForegroundWindow(), "No thread selected", "SysView", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = this->threadListView->SelectedIndices[0]; this->threadListView->SelectedIndices->Contains(i); i++)
	{
		long long TID = 0;
		if (System::Int64::TryParse(this->threadListView->Items[i]->Text, TID))
			CDriver::ThreadOperation((HANDLE)TID, ID_KILL_THREAD);
	}

	refreshToolStripMenuItem_Click(sender, e);	
}
private: System::Void suspendToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	if (!threadListView->SelectedItems->Count)
	{
		MessageBoxA(GetForegroundWindow(), "No thread selected", "SysView", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = this->threadListView->SelectedIndices[0]; this->threadListView->SelectedIndices->Contains(i); i++)
	{
		long long TID = 0;
		if (System::Int64::TryParse(this->threadListView->Items[i]->Text, TID))
		{
			if (this->threadListView->Items[i]->ForeColor == Color::White)
				CDriver::ThreadOperation((HANDLE)TID, ID_RESUME_THREAD);
			else
				CDriver::ThreadOperation((HANDLE)TID, ID_SUSPEND_THREAD);
		}
	}

	refreshToolStripMenuItem_Click(sender, e);
}
};
}
