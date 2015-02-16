#pragma once

#include <vector>
#include "ScriptWindow.h"
#include "TextWindow.h"
#include "SelectWindow.h"
#include "LogWindow.h"
#include "VariableWindow.h"
#include "windowdef.h"
class XmlEnum;

namespace yayoi {
	class YayoiScriptEngine;

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(XmlEnum&);

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  m_MenuItemFile;
	protected: 

	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^  m_MenuItemLoadScript;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBar1;
	private: System::Windows::Forms::ToolStripMenuItem^  m_MenuItemWindow;
	private: System::Windows::Forms::ToolStripMenuItem^  m_MenuItemDebug;
	private: System::Windows::Forms::ToolStripMenuItem^  m_MenuItemDebugWindow;
	private: System::Windows::Forms::ToolStripMenuItem^  menuItemScriptWindow;


	private: System::Windows::Forms::ToolStripMenuItem^  m_MenuItemRun;
	private: System::Windows::Forms::ToolStripMenuItem^  m_MenuItemStop;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Diagnostics::EventLog^  eventLog1;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  m_PlayButton;
	private: System::Windows::Forms::ToolStripButton^  m_StopButton;
	private: System::Windows::Forms::ToolStripMenuItem^  menuItemLogWindow;
	private: System::Windows::Forms::ToolStripMenuItem^  menuItemVariableWindow;
	private: System::Windows::Forms::ToolStripMenuItem^  menuItemTextWindow;











	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->m_MenuItemFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_MenuItemLoadScript = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_MenuItemDebug = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_MenuItemRun = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_MenuItemStop = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->m_MenuItemDebugWindow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuItemScriptWindow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuItemLogWindow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuItemVariableWindow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuItemTextWindow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_MenuItemWindow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripProgressBar1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->eventLog1 = (gcnew System::Diagnostics::EventLog());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->m_StopButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->m_PlayButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->eventLog1))->BeginInit();
			this->toolStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->m_MenuItemFile, this->m_MenuItemDebug, 
				this->m_MenuItemWindow});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->MdiWindowListItem = this->m_MenuItemWindow;
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(784, 26);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// m_MenuItemFile
			// 
			this->m_MenuItemFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->m_MenuItemLoadScript});
			this->m_MenuItemFile->Name = L"m_MenuItemFile";
			this->m_MenuItemFile->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::F));
			this->m_MenuItemFile->ShowShortcutKeys = false;
			this->m_MenuItemFile->Size = System::Drawing::Size(92, 22);
			this->m_MenuItemFile->Text = L"ファイル（F)";
			// 
			// m_MenuItemLoadScript
			// 
			this->m_MenuItemLoadScript->Name = L"m_MenuItemLoadScript";
			this->m_MenuItemLoadScript->Size = System::Drawing::Size(196, 22);
			this->m_MenuItemLoadScript->Text = L"スクリプトの読み込み";
			this->m_MenuItemLoadScript->Click += gcnew System::EventHandler(this, &Form1::MenuItemLoadScript_Click);
			// 
			// m_MenuItemDebug
			// 
			this->m_MenuItemDebug->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->m_MenuItemRun, 
				this->m_MenuItemStop, this->toolStripSeparator1, this->m_MenuItemDebugWindow});
			this->m_MenuItemDebug->Name = L"m_MenuItemDebug";
			this->m_MenuItemDebug->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::D));
			this->m_MenuItemDebug->Size = System::Drawing::Size(104, 22);
			this->m_MenuItemDebug->Text = L"デバッグ（Ｄ）";
			// 
			// m_MenuItemRun
			// 
			this->m_MenuItemRun->Enabled = false;
			this->m_MenuItemRun->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"m_MenuItemRun.Image")));
			this->m_MenuItemRun->Name = L"m_MenuItemRun";
			this->m_MenuItemRun->Size = System::Drawing::Size(164, 22);
			this->m_MenuItemRun->Text = L"実行";
			this->m_MenuItemRun->Click += gcnew System::EventHandler(this, &Form1::MenuItemRun_Click);
			// 
			// m_MenuItemStop
			// 
			this->m_MenuItemStop->Enabled = false;
			this->m_MenuItemStop->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"m_MenuItemStop.Image")));
			this->m_MenuItemStop->Name = L"m_MenuItemStop";
			this->m_MenuItemStop->Size = System::Drawing::Size(164, 22);
			this->m_MenuItemStop->Text = L"停止";
			this->m_MenuItemStop->Click += gcnew System::EventHandler(this, &Form1::MenuItemStop_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(161, 6);
			// 
			// m_MenuItemDebugWindow
			// 
			this->m_MenuItemDebugWindow->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->menuItemScriptWindow, 
				this->menuItemLogWindow, this->menuItemVariableWindow, this->menuItemTextWindow});
			this->m_MenuItemDebugWindow->Name = L"m_MenuItemDebugWindow";
			this->m_MenuItemDebugWindow->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::W));
			this->m_MenuItemDebugWindow->ShowShortcutKeys = false;
			this->m_MenuItemDebugWindow->Size = System::Drawing::Size(164, 22);
			this->m_MenuItemDebugWindow->Text = L"ウィンドウ（Ｗ）";
			// 
			// menuItemScriptWindow
			// 
			this->menuItemScriptWindow->Enabled = false;
			this->menuItemScriptWindow->Name = L"menuItemScriptWindow";
			this->menuItemScriptWindow->Size = System::Drawing::Size(160, 22);
			this->menuItemScriptWindow->Text = L"スクリプト";
			// 
			// menuItemLogWindow
			// 
			this->menuItemLogWindow->Enabled = false;
			this->menuItemLogWindow->Name = L"menuItemLogWindow";
			this->menuItemLogWindow->Size = System::Drawing::Size(160, 22);
			this->menuItemLogWindow->Text = L"Logウィンドウ";
			// 
			// menuItemVariableWindow
			// 
			this->menuItemVariableWindow->Enabled = false;
			this->menuItemVariableWindow->Name = L"menuItemVariableWindow";
			this->menuItemVariableWindow->Size = System::Drawing::Size(160, 22);
			this->menuItemVariableWindow->Text = L"変数ウィンドウ";
			// 
			// menuItemTextWindow
			// 
			this->menuItemTextWindow->Enabled = false;
			this->menuItemTextWindow->Name = L"menuItemTextWindow";
			this->menuItemTextWindow->Size = System::Drawing::Size(160, 22);
			this->menuItemTextWindow->Text = L"会話ウィンドウ";
			// 
			// m_MenuItemWindow
			// 
			this->m_MenuItemWindow->Name = L"m_MenuItemWindow";
			this->m_MenuItemWindow->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::W));
			this->m_MenuItemWindow->ShowShortcutKeys = false;
			this->m_MenuItemWindow->Size = System::Drawing::Size(109, 22);
			this->m_MenuItemWindow->Text = L"ウィンドウ（W)";
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripStatusLabel1, 
				this->toolStripProgressBar1});
			this->statusStrip1->Location = System::Drawing::Point(0, 540);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(784, 22);
			this->statusStrip1->TabIndex = 2;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(769, 17);
			this->toolStripStatusLabel1->Spring = true;
			this->toolStripStatusLabel1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// toolStripProgressBar1
			// 
			this->toolStripProgressBar1->Name = L"toolStripProgressBar1";
			this->toolStripProgressBar1->Size = System::Drawing::Size(100, 16);
			this->toolStripProgressBar1->Visible = false;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// eventLog1
			// 
			this->eventLog1->SynchronizingObject = this;
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->m_StopButton, this->m_PlayButton});
			this->toolStrip1->Location = System::Drawing::Point(0, 26);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(784, 25);
			this->toolStrip1->TabIndex = 4;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// m_StopButton
			// 
			this->m_StopButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->m_StopButton->Enabled = false;
			this->m_StopButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"m_StopButton.Image")));
			this->m_StopButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->m_StopButton->Name = L"m_StopButton";
			this->m_StopButton->Size = System::Drawing::Size(23, 22);
			this->m_StopButton->Text = L"toolStripButton1";
			this->m_StopButton->Click += gcnew System::EventHandler(this, &Form1::MenuItemStop_Click);
			// 
			// m_PlayButton
			// 
			this->m_PlayButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->m_PlayButton->Enabled = false;
			this->m_PlayButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"m_PlayButton.Image")));
			this->m_PlayButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->m_PlayButton->Name = L"m_PlayButton";
			this->m_PlayButton->Size = System::Drawing::Size(23, 22);
			this->m_PlayButton->Text = L"スクリプト実行";
			this->m_PlayButton->Click += gcnew System::EventHandler(this, &Form1::MenuItemRun_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 562);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->IsMdiContainer = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Yayoi";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->eventLog1))->EndInit();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private:
			WINDOW_DEF(Script);
			WINDOW_DEF(Log);
			WINDOW_DEF(Variable);
			WINDOW_DEF(Text);
			System::Void Form1_Closing(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e); 
			System::Void TextWindow_Click(System::Object^  sender, System::EventArgs^  e); 
			System::Void SelectWindow_Click(System::Object^  sender, System::EventArgs^  e); 
			System::Void ScriptView_DoubleClick(System::Object^  sender, System::EventArgs^  e);
			System::Void MenuItemLoadScript_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void MenuItemRun_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void MenuItemStop_Click(System::Object^  sender, System::EventArgs^  e);
			void RunScript();
			void StopScript();

			SelectWindow^	m_SelectWindow;
			String^			m_ScriptDirectory;
			yayoi::YayoiScriptEngine*	m_pScriptEngine;
			int				m_ScriptRunningStatus;
			bool			m_ScriptRunning;
};
}
