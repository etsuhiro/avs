#include "stdafx.h"
#include "Form1.h"
#include "YayoiScriptEngine.h"
#include "AvsCore/VariableListener.h"
#include "YayoiText.h"
#include "common/crc32.h"

using namespace yayoi;

static std::vector<char> scriptBuf;
static hashmap_t hashmap;
static XmlEnum*	s_pMikuEnum;

class Variable : public avs::VariableListener 
{
public:
	Variable(VariableWindow^ hWindow) : m_VariableWindow(hWindow){
	}
private:
	void Set(const char* name, int val)
	{
		m_item[name] =val;
		m_VariableWindow->Add(gcnew String(name), val);
	}
	int Get(const char* name)
	{
		std::map<std::string, int>::iterator it = m_item.find(name);
		if (it==m_item.end()){
//			MessageBox::Show("変数は登録されていません", gcnew String(name));
			return 0;
		}
		return m_item[name];
	}

	gcroot<VariableWindow^>		m_VariableWindow;
	std::map<std::string, int>	m_item;
};

bool LoadScript(std::vector<char>& scriptbuf, String^ path)
{
	using namespace System::Runtime::InteropServices;

	TiXmlDocument xml;
	// スクリプトを読む
	char* p = (char*)Marshal::StringToHGlobalAnsi(path).ToPointer();
	if (!xml.LoadFile(p)){
		MessageBox::Show(path, "file read error");
		return false;
	}

	// バイナリに変換
	scriptBuf.clear();
	hashmap.clear();
	XmlBin().Conv(scriptbuf, hashmap, xml, s_pMikuEnum );
	return true;
}


Form1::Form1(XmlEnum& mikuEnum)
{
	s_pMikuEnum = &mikuEnum;

	InitializeComponent();
	Closing += gcnew CancelEventHandler(this, &Form1::Form1_Closing);

	m_ScriptWindow = gcnew ScriptWindow();
	m_ScriptWindow->MdiParent = this;
	m_ScriptWindow->Closing += gcnew CancelEventHandler(this, &Form1::ScriptWindow_Closing);
	m_ScriptWindow->scriptView->DoubleClick += gcnew System::EventHandler( this, &Form1::ScriptView_DoubleClick );

	m_TextWindow = gcnew TextWindow();
	m_TextWindow->MdiParent = this;
	m_TextWindow->Closing += gcnew CancelEventHandler(this, &Form1::TextWindow_Closing);
	m_TextWindow->button1->Click += gcnew System::EventHandler(this, &Form1::TextWindow_Click);

	m_SelectWindow = gcnew SelectWindow();
	m_SelectWindow->MdiParent = this;
	m_SelectWindow->listBox1->Click += gcnew System::EventHandler(this, &Form1::SelectWindow_Click);

	m_LogWindow = gcnew LogWindow();
	m_LogWindow->MdiParent = this;
	m_LogWindow->Closing += gcnew CancelEventHandler(this, &Form1::LogWindow_Closing);
	m_LogWindow->Show();

	m_VariableWindow = gcnew VariableWindow();
	m_VariableWindow->MdiParent = this;
	m_VariableWindow->Closing += gcnew CancelEventHandler(this, &Form1::VariableWindow_Closing);
	m_VariableWindow->Show();

	m_pScriptEngine = new YayoiScriptEngine(mikuEnum, m_LogWindow);
	m_pScriptEngine->AddTextListener(new YayoiText(m_TextWindow, m_SelectWindow));	// Todo deleteやろうぜ
	m_pScriptEngine->AddVariableListener(new Variable(m_VariableWindow));	// Todo deleteやろうぜ
	m_pScriptEngine->AddErrorMessageListener(m_pScriptEngine);

	menuItemScriptWindow->Click += gcnew System::EventHandler(this, &Form1::MenuItemScriptWindow_Click);
	menuItemLogWindow->Click += gcnew System::EventHandler(this, &Form1::MenuItemLogWindow_Click);
	menuItemTextWindow->Click += gcnew System::EventHandler(this, &Form1::MenuItemTextWindow_Click);
	menuItemVariableWindow->Click += gcnew System::EventHandler(this, &Form1::MenuItemVariableWindow_Click);
}

System::Void Form1::MenuItemLoadScript_Click(System::Object^  sender, System::EventArgs^  e)
{
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

//	openFileDialog1->InitialDirectory = ".\\";
	openFileDialog1->Filter = "script files (*.xml)|*.xml|All files (*.*)|*.*";
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->RestoreDirectory = true;

	if ( openFileDialog1->ShowDialog() == ::DialogResult::OK )
	{
		if (LoadScript(scriptBuf, openFileDialog1->FileName)){
			m_pScriptEngine->Init();
			m_pScriptEngine->SetScript(&scriptBuf[0]);
			toolStripStatusLabel1->Text = openFileDialog1->FileName;
			m_ScriptDirectory = System::IO::Path::GetDirectoryName(openFileDialog1->FileName);
			m_ScriptWindow->Text = "スクリプト - " + System::IO::Path::GetFileNameWithoutExtension(openFileDialog1->FileName);
			m_ScriptWindow->MikuPrint(&scriptBuf[0], *s_pMikuEnum, hashmap);
			m_ScriptWindow->Show();
			m_MenuItemRun->Enabled = true;
			m_PlayButton->Enabled = true;
			menuItemScriptWindow->Enabled = false;	// 再表示ボタン
		} else {
			MessageBox::Show("スクリプトファイルエラー", openFileDialog1->FileName);
		}
	}
}

// アプリの終了を確認
System::Void Form1::Form1_Closing(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	if ( MessageBox::Show( "終了しますか？", "Yayoi", MessageBoxButtons::YesNo ) == ::DialogResult::Yes )
	{
		// Cancel the Closing event from closing the form.
		e->Cancel = false;
	} else {
		e->Cancel = true;
	}
} 

// 実行ボタン
System::Void Form1::MenuItemRun_Click(System::Object^  sender, System::EventArgs^  e)
{
	m_MenuItemRun->Enabled = false;
	m_PlayButton->Enabled = false;
	m_StopButton->Enabled = true;
	m_TextWindow->textBox1->Clear();
	m_TextWindow->Show();

	m_pScriptEngine->Init();
	RunScript();
}

// 停止ボタン
System::Void Form1::MenuItemStop_Click(System::Object^  sender, System::EventArgs^  e)
{
	StopScript();
}

// スクリプトダブルクリック
System::Void Form1::ScriptView_DoubleClick(System::Object^  sender, System::EventArgs^  e)
{
	int idx = m_ScriptWindow->GetSelectedIndex();
	m_pScriptEngine->SetIndex(idx);
}

// テキストウインドウクリック
System::Void Form1::TextWindow_Click(System::Object^  sender, System::EventArgs^  e)
{
	m_TextWindow->button1->Enabled = false;
	RunScript();
}

// 選択ウィンドウクリック
System::Void Form1::SelectWindow_Click(System::Object^  sender, System::EventArgs^  e)
{
	int n = m_SelectWindow->Selected();
	if (n>=0){
		m_SelectWindow->Hide();
		m_SelectWindow->ItemsClear();
		m_pScriptEngine->GotoOption(n);
		m_ScriptRunningStatus = avs::LABEL_JUMP;
		RunScript();
	}
}

// スクリプト実行
void Form1::RunScript()
{
	m_ScriptRunning = true;

LOOP:
		m_ScriptRunningStatus = m_pScriptEngine->Run();
		switch (m_ScriptRunningStatus){
		case avs::FINISH:
			StopScript();
			m_pScriptEngine->Init();
			break;
		case avs::CLICK:
			m_TextWindow->button1->Enabled = true;
			break;
		case avs::SELECT:
			m_SelectWindow->Show();
			break;
		case avs::LABEL_JUMP:
			{
				String^ filename = gcnew String( m_pScriptEngine->getJumpFile() );
				String^ path = m_ScriptDirectory + "\\" + filename + ".xml";
				if (LoadScript(scriptBuf, path)){
					m_pScriptEngine->SetScript(&scriptBuf[0]);

					toolStripStatusLabel1->Text = path;
					m_ScriptWindow->Text = "スクリプト - " + filename;
					m_ScriptWindow->MikuPrint(&scriptBuf[0], *s_pMikuEnum, hashmap);
					goto LOOP;
				} else {
					MessageBox::Show("スクリプト読み込みエラー", path);
					StopScript();
				}
			}
			break;
		}
//	} while(m_ScriptRunningStatus==avs::ScriptEngine::CONTINUE);
		m_ScriptWindow->CheckLine(m_pScriptEngine->GetMsgIndex());
		m_ScriptWindow->SelectedLine(m_pScriptEngine->GetIndex());
}

void Form1::StopScript()
{
	m_TextWindow->Hide();	// 非表示
	m_SelectWindow->Hide();
	m_SelectWindow->ItemsClear();
	m_MenuItemRun->Enabled = true;
	m_PlayButton->Enabled = true;
	m_StopButton->Enabled = false;
	m_ScriptRunning = false;
}
