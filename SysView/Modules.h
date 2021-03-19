#pragma once

namespace SysViewModule {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für Modules
	/// </summary>
	public ref class Modules : public System::Windows::Forms::Form
	{
	public:
		Modules(HANDLE PID)
		{
			this->myPID = PID;
			InitializeComponent();
		}

	protected:

		~Modules()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^ modulesListView;
	protected:
	private: System::Windows::Forms::ColumnHeader^ columnHeader5;
	private: System::Windows::Forms::ColumnHeader^ columnHeader6;
	private: System::Windows::Forms::ColumnHeader^ columnHeader4;
	private: System::Windows::Forms::ColumnHeader^ columnHeader7;
	private: System::Windows::Forms::ContextMenuStrip^ moduleMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^ refreshToolStripMenuItem;
	private: System::ComponentModel::IContainer^ components;
	private: System::Windows::Forms::ColumnHeader^ columnHeader1;

	private:
		HANDLE myPID;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->modulesListView = (gcnew System::Windows::Forms::ListView());
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader7 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->moduleMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->refreshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->moduleMenuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// modulesListView
			// 
			this->modulesListView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(5) {
				this->columnHeader5,
					this->columnHeader6, this->columnHeader4, this->columnHeader7, this->columnHeader1
			});
			this->modulesListView->ContextMenuStrip = this->moduleMenuStrip;
			this->modulesListView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->modulesListView->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->modulesListView->FullRowSelect = true;
			this->modulesListView->HideSelection = false;
			this->modulesListView->Location = System::Drawing::Point(0, 0);
			this->modulesListView->Name = L"modulesListView";
			this->modulesListView->Size = System::Drawing::Size(1004, 537);
			this->modulesListView->TabIndex = 3;
			this->modulesListView->UseCompatibleStateImageBehavior = false;
			this->modulesListView->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"Image Name";
			this->columnHeader5->Width = 100;
			// 
			// columnHeader6
			// 
			this->columnHeader6->Text = L"Image Path";
			this->columnHeader6->Width = 200;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"Image Base";
			this->columnHeader4->Width = 250;
			// 
			// columnHeader7
			// 
			this->columnHeader7->Text = L"Image Entry";
			this->columnHeader7->Width = 250;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Image Size";
			// 
			// moduleMenuStrip
			// 
			this->moduleMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->refreshToolStripMenuItem });
			this->moduleMenuStrip->Name = L"moduleMenuStrip";
			this->moduleMenuStrip->Size = System::Drawing::Size(114, 26);
			// 
			// refreshToolStripMenuItem
			// 
			this->refreshToolStripMenuItem->Name = L"refreshToolStripMenuItem";
			this->refreshToolStripMenuItem->Size = System::Drawing::Size(113, 22);
			this->refreshToolStripMenuItem->Text = L"Refresh";
			this->refreshToolStripMenuItem->Click += gcnew System::EventHandler(this, &Modules::refreshToolStripMenuItem_Click);
			// 
			// Modules
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1004, 537);
			this->Controls->Add(this->modulesListView);
			this->Name = L"Modules";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Modules";
			this->Load += gcnew System::EventHandler(this, &Modules::Modules_Load);
			this->moduleMenuStrip->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void refreshToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		if (!CDriver::GetModuleList(this->myPID))
		{
			MessageBoxA(GetForegroundWindow(), "Failed obtaining module list", "SysView", MB_OK | MB_ICONERROR);
			return;
		}

		this->modulesListView->Items->Clear();
		auto modVec = CDriver::GetModuleVector();
		for (auto x : modVec)
		{
			auto sModName = gcnew String(x.BaseDllName);
			auto sPath = gcnew String(x.FullDllName);
			auto sEntry = x.EntryPoint.ToString("X");
			auto sBase = x.DllBase.ToString("X");
			auto sSize = x.ImageSize.ToString();
			auto modItem = gcnew System::Windows::Forms::ListViewItem(sModName);
			modItem->SubItems->Add(sPath);
			modItem->SubItems->Add(sBase);
			modItem->SubItems->Add(sEntry);
			modItem->SubItems->Add(sSize);
			this->modulesListView->Items->Add(modItem);
		}

		modVec.clear();
	}
private: System::Void Modules_Load(System::Object^ sender, System::EventArgs^ e) 
{
	auto sPID = System::Convert::ToString(reinterpret_cast<unsigned long long>(this->myPID));
	this->Text = "SysView - Modules [" + sPID + "]";
	this->refreshToolStripMenuItem_Click(sender, e);
}
};
}
