#pragma once

#include "AddBlacklist.h"

namespace SysViewBlacklist {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für Blacklist
	/// </summary>
	public ref class Blacklist : public System::Windows::Forms::Form
	{
	public:
		Blacklist(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Blacklist()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^ blacklistListView;
	protected:
	private: System::Windows::Forms::ColumnHeader^ columnHeader1;
	private: System::Windows::Forms::ContextMenuStrip^ contextMenuStripBlacklist;
	private: System::Windows::Forms::ToolStripMenuItem^ addToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ removeToolStripMenuItem;
	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->blacklistListView = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->contextMenuStripBlacklist = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->addToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->removeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStripBlacklist->SuspendLayout();
			this->SuspendLayout();
			// 
			// blacklistListView
			// 
			this->blacklistListView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) { this->columnHeader1 });
			this->blacklistListView->ContextMenuStrip = this->contextMenuStripBlacklist;
			this->blacklistListView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->blacklistListView->FullRowSelect = true;
			this->blacklistListView->GridLines = true;
			this->blacklistListView->HideSelection = false;
			this->blacklistListView->Location = System::Drawing::Point(0, 0);
			this->blacklistListView->Name = L"blacklistListView";
			this->blacklistListView->Size = System::Drawing::Size(388, 249);
			this->blacklistListView->TabIndex = 2;
			this->blacklistListView->UseCompatibleStateImageBehavior = false;
			this->blacklistListView->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Image Name";
			this->columnHeader1->Width = 336;
			// 
			// contextMenuStripBlacklist
			// 
			this->contextMenuStripBlacklist->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->addToolStripMenuItem,
					this->removeToolStripMenuItem
			});
			this->contextMenuStripBlacklist->Name = L"contextMenuStripBlacklist";
			this->contextMenuStripBlacklist->Size = System::Drawing::Size(118, 48);
			// 
			// addToolStripMenuItem
			// 
			this->addToolStripMenuItem->Name = L"addToolStripMenuItem";
			this->addToolStripMenuItem->Size = System::Drawing::Size(117, 22);
			this->addToolStripMenuItem->Text = L"Add";
			this->addToolStripMenuItem->Click += gcnew System::EventHandler(this, &Blacklist::addToolStripMenuItem_Click);
			// 
			// removeToolStripMenuItem
			// 
			this->removeToolStripMenuItem->Name = L"removeToolStripMenuItem";
			this->removeToolStripMenuItem->Size = System::Drawing::Size(117, 22);
			this->removeToolStripMenuItem->Text = L"Remove";
			this->removeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Blacklist::removeToolStripMenuItem_Click);
			// 
			// Blacklist
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(388, 249);
			this->Controls->Add(this->blacklistListView);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Blacklist";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"SysView - Process Blacklist";
			this->Load += gcnew System::EventHandler(this, &Blacklist::Blacklist_Load);
			this->contextMenuStripBlacklist->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Blacklist_Load(System::Object^ sender, System::EventArgs^ e) {
		//Fill listview with current blacklist
		auto vBlacklist = CDriver::GetBlacklistVector();
		for (auto x : vBlacklist)
		{
			auto sProc = gcnew String(x.c_str());
			this->blacklistListView->Items->Add(sProc);
		}
	}
private: System::Void addToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	std::wstring sInput = { 0 };
	auto inputForm = gcnew SysViewAddBlacklist::AddBlacklist(&sInput);
	inputForm->ShowDialog();

	if (!sInput.size())
		return;

	if (CDriver::IsProcessAlreadyBlacklisted(sInput))
	{
		MessageBoxA(GetForegroundWindow(), "The given process is already blacklisted",
			"SysView - Process Blacklist", MB_OK | MB_ICONERROR);
		return;
	}

	CDriver::BlockProcess(sInput, false);
	auto sProc = gcnew String(sInput.c_str());
	this->blacklistListView->Items->Add(sProc);
}
private: System::Void removeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!this->blacklistListView->SelectedItems->Count)
	{
		MessageBoxA(GetForegroundWindow(), "No process selected to be removed", "SpyView", MB_OK | MB_ICONWARNING);
		return;
	}

	auto sEntry = this->blacklistListView->SelectedItems[0]->Text;
	auto sProc = msclr::interop::marshal_as<std::wstring>(sEntry);
	CDriver::BlockProcess(sProc, true);
	this->blacklistListView->SelectedItems[0]->Remove();
}
};
}
