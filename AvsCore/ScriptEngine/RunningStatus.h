#pragma once

namespace avs {
	//! ���s���̏��
	enum RunningStatus {
		CONTINUE = 0,	//!< �p��
		FINISH,			//!< �X�N���v�g���I��
		WAIT,			//!< ���ԑ҂�
		LABEL_JUMP,		//!< ���x���W�����v
		SELECT,			//!< �I��҂�
		BUSY,			//!< �Ȃ�炩�̏����҂�
		CHILD_SCRIPT,	//!< �}���`�X�N���v�g
		WHILE,			//!< �J��Ԃ�
		EVENT,			//!< �C�x���g
		CLICK,
	};
}