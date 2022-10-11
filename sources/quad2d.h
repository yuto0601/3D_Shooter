#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<string>
#include	"dx11mathutil.h"
#include	"dx11util.h"
#include	"Shader.h"
#include	"shaderhashmap.h"
#include	"DX11Settransform.h"
#include	"uncopyable.h"

// comptr
using Microsoft::WRL::ComPtr;

// ��`�N���X
class Quad2D : public Uncopyable{
public:
	// ���_�f�[�^
	struct Vertex {	
		DirectX::XMFLOAT3 pos;		// ���W		
		DirectX::XMFLOAT4 color;	// �J���[
		DirectX::XMFLOAT2 tex;		// �e�N�X�`��
	};

	// �r���[�|�[�g�̃T�C�Y���Z�b�g
	bool SetViewPortSize(uint32_t width,uint32_t height) {

		ComPtr<ID3D11Buffer> CBViewPort;

		// �r���[�|�[�g�p�̒萔�o�b�t�@
		struct ConstantBufferViewPort {
			uint32_t	Width[4];			// �r���[�|�[�g��
			uint32_t	Height[4];			// �r���[�|�[�g����
		};

		// �r���[�|�[�g�萔�o�b�t�@�ɒl���Z�b�g
		bool sts = CreateConstantBufferWrite(
			GetDX11Device(), 
			sizeof(ConstantBufferViewPort), 
			&CBViewPort);
		if (!sts) {
			return false;
		}

		ConstantBufferViewPort cb;
		cb.Width[0] = width;
		cb.Height[0] = height;

		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = GetDX11DeviceContext()->Map(CBViewPort.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(ConstantBufferViewPort));
			GetDX11DeviceContext()->Unmap(CBViewPort.Get(), 0);
		}
		else {
			return false;
		}

		// ���_�V�F�[�_�[�ɃZ�b�g
		GetDX11DeviceContext()->VSSetConstantBuffers(5, 1, CBViewPort.GetAddressOf());

		return true;
	}

	// ��`�̏�����
	bool Init(uint32_t width, uint32_t height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv);

	// �`��
	void Draw();

	void UnInit();

	// �g��A�k��
	void SetScale(float sx, float sy, float sz);

	// �ʒu���Z�b�g
	void SetPosition(float x, float y, float z);

	// Z����]
	void SetRotation(float angle);

	// ��`�̐i�s�������擾�i���摜��������ɍ���Ă��邱�Ƃ�O��j
	DirectX::XMFLOAT3 GetForward() {
		DirectX::XMFLOAT3 forward;
		forward.x = m_worldmtx._21;
		forward.y = m_worldmtx._22;
		forward.z = m_worldmtx._23;
		return forward;
	}

	// ��`�̉��������擾
	DirectX::XMFLOAT3 GetSide() {
		DirectX::XMFLOAT3 side;
		side.x = m_worldmtx._11;
		side.y = m_worldmtx._12;
		side.z = m_worldmtx._13;
		return side;
	}

	// ���_�f�[�^�X�V
	void updateVertex(uint32_t width, uint32_t height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv);
	// ���_�o�b�t�@�X�V
	void updateVbuffer();

	// �e�N�X�`���Z�b�g
	void LoadTexture(const char* filename) {
		// �e�N�X�`���ǂݍ���
		bool sts = CreateSRVfromFile(filename,
			GetDX11Device(),
			GetDX11DeviceContext(),
			&m_tex, &m_srv);
		if (!sts) {
			std::string str("SRV �쐬�G���[ ");
			std::string strfile(filename);
			MessageBox(nullptr, (str+strfile).c_str(), "error", MB_OK);
			return;
		}
	}

private:
	DirectX::XMFLOAT4X4		m_worldmtx;				// ���[���h�ϊ��s��
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// �C���f�b�N�X�o�b�t�@
	Quad2D::Vertex			m_vertex[4];			// ��`�S���_
	float					m_width;				// ��
	float					m_height;				// ����
	ComPtr<ID3D11ShaderResourceView> m_srv;			// SRV
	ComPtr<ID3D11Resource> m_tex;					// �e�N�X�`�����\�[�X
};