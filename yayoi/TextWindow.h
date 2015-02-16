#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace yayoi {

	/// <summary>
	/// TextWindow の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class TextWindow : public System::Windows::Forms::Form
	{
	public:
		TextWindow(void)
		{
			InitializeComponent();
		}

	protected:
		~TextWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	public: System::Windows::Forms::TextBox^  textBox1;
	public: System::Windows::Forms::Button^  button1;

	public: 

	public: 
	private: 

	private: 



	protected: 

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
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"ＭＳ ゴシック", 14, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->textBox1->Location = System::Drawing::Point(0, 0);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(409, 135);
			this->textBox1->TabIndex = 1;
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(357, 99);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(40, 24);
			this->button1->TabIndex = 2;
			this->button1->Text = L"▼";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// TextWindow
			// 
			this->AcceptButton = this->button1;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(409, 135);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox1);
			this->Location = System::Drawing::Point(200, 300);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"TextWindow";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"会話ウィンドウ";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
