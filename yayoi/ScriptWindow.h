#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

class XmlEnum;

namespace yayoi {

	/// <summary>
	/// ScriptWindow の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class ScriptWindow : public System::Windows::Forms::Form
	{
	public:
		ScriptWindow(void);
		void MikuPrint(const char *pBuf, XmlEnum& xmls, const hashmap_t& hashmap);
		void SelectedLine(int idx);
		void CheckLine(int idx);
		int GetSelectedIndex();

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~ScriptWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TreeView^  scriptView;
	protected: 

	private:
		List<TreeNode^> m_NodeList;
		List<int>		m_IndexList;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  MenuItemJump;

	private: System::ComponentModel::IContainer^  components;


	protected: 

	protected: 

	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>



#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ScriptWindow::typeid));
			this->scriptView = (gcnew System::Windows::Forms::TreeView());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->MenuItemJump = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// scriptView
			// 
			this->scriptView->ContextMenuStrip = this->contextMenuStrip1;
			this->scriptView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->scriptView->FullRowSelect = true;
			this->scriptView->HideSelection = false;
			this->scriptView->Location = System::Drawing::Point(0, 0);
			this->scriptView->Name = L"scriptView";
			this->scriptView->Size = System::Drawing::Size(526, 259);
			this->scriptView->TabIndex = 0;
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->MenuItemJump});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(153, 48);
			// 
			// MenuItemJump
			// 
			this->MenuItemJump->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"MenuItemJump.Image")));
			this->MenuItemJump->Name = L"MenuItemJump";
			this->MenuItemJump->Size = System::Drawing::Size(152, 22);
			this->MenuItemJump->Text = L"ジャンプ";
			// 
			// ScriptWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(526, 259);
			this->Controls->Add(this->scriptView);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ScriptWindow";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"スクリプト";
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
