#define WINDOW_DEF(xxx)	\
	System::Void xxx##Window_Closing(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)	\
	{	\
		e->Cancel = true;	\
		m_##xxx##Window->Hide();	\
		menuItem##xxx##Window->Enabled = true;	\
	}	\
	System::Void MenuItem##xxx##Window_Click(System::Object^  sender, System::EventArgs^  e)	\
	{	\
		m_##xxx##Window->Show();	\
		menuItem##xxx##Window->Enabled = false;	\
	}	\
	xxx##Window^	m_##xxx##Window

