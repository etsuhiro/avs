#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace yayoi {

	/// <summary>
	/// SelectWindow の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class SelectWindow : public System::Windows::Forms::Form
	{
	public:
		SelectWindow(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクタ コードを追加します
			//
		}
		void AddOption(String^ option, int idx)
		{
			listBox1->Items->Add(option);
			m_OptionIdx.Add(idx);
		}
		int Selected()
		{
			return safe_cast<int>(m_OptionIdx[listBox1->SelectedIndex]);
		}
		void ItemsClear()
		{
			listBox1->Items->Clear();
			m_OptionIdx.Clear();
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~SelectWindow()
		{
			if (components)
			{

			}
		}

	public: System::Windows::Forms::ListBox^  listBox1;
	private: ArrayList	m_OptionIdx;

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
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listBox1->Font = (gcnew System::Drawing::Font(L"メイリオ", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->listBox1->FormattingEnabled = true;
			this->listBox1->ItemHeight = 28;
			this->listBox1->Location = System::Drawing::Point(0, 0);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(362, 116);
			this->listBox1->TabIndex = 1;
			// 
			// SelectWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(362, 142);
			this->ControlBox = false;
			this->Controls->Add(this->listBox1);
			this->Location = System::Drawing::Point(200, 90);
			this->Name = L"SelectWindow";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"選択ウィンドウ";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
