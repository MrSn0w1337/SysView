#pragma once

namespace SysViewDriver {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für Drivers
	/// </summary>
	public ref class Drivers : public System::Windows::Forms::Form
	{
	public:
		Drivers(void)
		{
			this->listFont = gcnew System::Drawing::Font("Segoe UI", 9.f);
			InitializeComponent();
		}

	protected:
		~Drivers()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^ driversListView;
	protected:
	private: System::Windows::Forms::ColumnHeader^ columnHeader5;
	private: System::Windows::Forms::ColumnHeader^ columnHeader6;
	private: System::Windows::Forms::ColumnHeader^ columnHeader4;
	private: System::Windows::Forms::ColumnHeader^ columnHeader7;
	private: System::Windows::Forms::ContextMenuStrip^ driversStripMenu;
	private: System::Windows::Forms::ToolStripMenuItem^ refreshToolStripMenuItem;
	private: System::ComponentModel::IContainer^ components;

	private:
		System::Drawing::Font^ listFont;


#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->driversListView = (gcnew System::Windows::Forms::ListView());
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader7 = (gcnew System::Windows::Forms::ColumnHeader());
			this->driversStripMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->refreshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->driversStripMenu->SuspendLayout();
			this->SuspendLayout();
			// 
			// driversListView
			// 
			this->driversListView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {
				this->columnHeader5,
					this->columnHeader6, this->columnHeader4, this->columnHeader7
			});
			this->driversListView->ContextMenuStrip = this->driversStripMenu;
			this->driversListView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->driversListView->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->driversListView->FullRowSelect = true;
			this->driversListView->HideSelection = false;
			this->driversListView->Location = System::Drawing::Point(0, 0);
			this->driversListView->Name = L"driversListView";
			this->driversListView->Size = System::Drawing::Size(822, 435);
			this->driversListView->TabIndex = 2;
			this->driversListView->UseCompatibleStateImageBehavior = false;
			this->driversListView->View = System::Windows::Forms::View::Details;
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
			this->columnHeader7->Text = L"Image Size";
			this->columnHeader7->Width = 100;
			// 
			// driversStripMenu
			// 
			this->driversStripMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->refreshToolStripMenuItem });
			this->driversStripMenu->Name = L"contextMenuStrip1";
			this->driversStripMenu->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->driversStripMenu->Size = System::Drawing::Size(114, 26);
			// 
			// refreshToolStripMenuItem
			// 
			this->refreshToolStripMenuItem->Name = L"refreshToolStripMenuItem";
			this->refreshToolStripMenuItem->Size = System::Drawing::Size(113, 22);
			this->refreshToolStripMenuItem->Text = L"Refresh";
			this->refreshToolStripMenuItem->Click += gcnew System::EventHandler(this, &Drivers::refreshToolStripMenuItem_Click);
			// 
			// Drivers
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(822, 435);
			this->Controls->Add(this->driversListView);
			this->Name = L"Drivers";
			this->ShowIcon = false;
			this->Text = L"Drivers";
			this->Load += gcnew System::EventHandler(this, &Drivers::Drivers_Load);
			this->driversStripMenu->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void refreshToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		if (!CDriver::GetDriverList())
		{
			MessageBoxA(GetForegroundWindow(), "Failed to get drivers", "SysView", MB_OK | MB_ICONERROR);
			return;
		}

		this->driversListView->Items->Clear();
		auto pDriverVec = CDriver::GetDriverVector();
		for (auto& x : pDriverVec)
		{
			auto sName = gcnew String(x.ImageName);
			auto sPath = gcnew String(x.Path);
			auto sBase = x.ImageBase.ToString("X");
			auto sSize = x.ImageSize.ToString();
			auto listItem = gcnew System::Windows::Forms::ListViewItem(sName);
			listItem->SubItems->Add(sPath);
			listItem->SubItems->Add(sBase);
			listItem->SubItems->Add(sSize);
			listItem->Font = this->listFont;
			driversListView->Items->Add(listItem);
		}
		pDriverVec.clear();
	}
private: System::Void Drivers_Load(System::Object^ sender, System::EventArgs^ e) 
{
	this->refreshToolStripMenuItem_Click(sender, e);
	this->Text = "SysView - Drivers";
}
};
}
