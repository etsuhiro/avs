/*! @file TextCanvas.h */
/*!
	�e�L�X�g�L�����o�X
 */

#ifndef avs_text_canvas_h_
#define avs_text_canvas_h_

namespace avs
{
	struct SetupData;

	enum Status {
		EMPTY,
		BUSY,
		READY
	};

	class TextCanvas {
	public:
		TextCanvas(avs::SetupData& );
		~TextCanvas(void);

		void Put(const char *);
		void Update(int msec_elapse_time);
		void Draw();
		void Clear();
		void Flush();

		Status GetStatus(){ return tcStatus; }

	private:
		virtual void draw(int x, int y, const unsigned short *str, int n){}
		virtual void se(){}

		Status	tcStatus;

		int					m_nLine;	//!< �A�N�e�B�u�ȃe�L�X�g�̍s��
		int					m_nChr;		//!< �A�N�e�B�u�ȃe�L�X�g�̕�����
		unsigned short*		m_pCurrent;	//!< ���݂̃o�b�t�@�|�C���^
		unsigned short*		m_pBuffer;	//!< �e�L�X�g�o�b�t�@
		unsigned short**	m_pLinebuf;	//!< �e�s�̃e�L�X�g�|�C���^�[
		int*				m_nChrLine;	//!< �e�s�A�N�e�B�u�ȃe�L�X�g������
		int					m_chrCnt;	//!< ���ݕ\������镶����
		int					m_pool;		//!< ��������p���ԃv�[���i�������ǂ��̒|�j

		int					m_ChrSpeed;	//!< ��������̎���
		avs::SetupData&		m_Setup;	//!< �Z�b�g�A�b�v�f�[�^
	};
}

#endif
