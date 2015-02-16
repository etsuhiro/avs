#pragma once

//#pragma warning(push)
//#pragma warning(disable : 4005)
#include <d3d11_1.h>
//#pragma warning(pop)

class DX11 {
public:
	DX11();
	~DX11();

	// DirectX11������ �������̎��s���l�����ăR���X�g���N�^�ł͂Ȃ��A�֐��ɂ���
	// hwnd : �\������E�C���h�E�̃n���h��
	HRESULT InitDX11(HWND hwnd);

	// DirectX11�I��
	void ExitDX11();

	// �����_�����O
	void Clear();
	void Flip();

	ID3D11Device* Device() const;
	ID3D11DeviceContext* DeviceContext() const;
};
