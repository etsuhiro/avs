/*! @file VariablListener.h */
/*!
	@brief �ϐ��C���^�[�t�F�C�X
	
	ScriptEngine�Ƀe�L�X�g�̏o�͂���������ƃR�[���o�b�N�����B
 */

namespace avs
{

class VariableListener {
protected:
	~VariableListener(){}
public:

	//! �ϐ��ɒl���Z�b�g����
	virtual void Set(const char* name, int val) = 0;
	//! �ϐ��̒l�𓾂�
	virtual int Get(const char* name) = 0;
	//! �L�[���𓾂�
	virtual int KeyOn(int key) = 0;
};

}
