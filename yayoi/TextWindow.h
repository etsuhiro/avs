#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace yayoi {

	/// <summary>
	/// TextWindow �̊T�v
	///
	/// �x��: ���̃N���X�̖��O��ύX����ꍇ�A���̃N���X���ˑ����邷�ׂĂ� .resx �t�@�C���Ɋ֘A�t����ꂽ
	///          �}�l�[�W ���\�[�X �R���p�C�� �c�[���ɑ΂��� 'Resource File Name' �v���p�e�B��
	///          �ύX����K�v������܂��B���̕ύX���s��Ȃ��ƁA
	///          �f�U�C�i�ƁA���̃t�H�[���Ɋ֘A�t����ꂽ���[�J���C�Y�ς݃��\�[�X�Ƃ��A
	///          ���������݂ɗ��p�ł��Ȃ��Ȃ�܂��B
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
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �f�U�C�i �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�ŕύX���Ȃ��ł��������B
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
			this->textBox1->Font = (gcnew System::Drawing::Font(L"�l�r �S�V�b�N", 14, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
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
			this->button1->Text = L"��";
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
			this->Text = L"��b�E�B���h�E";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
