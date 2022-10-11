#pragma once
#include	<directxmath.h>
#include "Collider.h"
#include "UvAnimation.h"
#include"dx11mathutil.h"



class GameObject {
protected:
	DirectX::XMFLOAT3 m_pos{};				// �ʒu
	DirectX::XMFLOAT4X4 m_mtx{};				// �p��
	int HP = 1;									// �q�b�g�|�C���g
public:
	uint64_t m_id = 0;						// ���ʎq
	GameObject();//������
	 void Update();		// �X�V����
	 void Draw();			// �`�揈��
	virtual ~GameObject() {};//���

	// �g�����X�t�H�[���i�ړ��j���
	float x, y, z;  // �ʒu�i���[���h���W�j
	float rx, ry, rz;  // ��]�p�x
	float sx, sy, sz;
    float speed;
	// �O�����x�N�g�������ϐ�
	DirectX::XMFLOAT3 forwardVec;
	
	//�����߂�����
	void attraction() {
		float distance;
		DX11p2pLength(m_pos, DirectX::XMFLOAT3{ 0,0,0 }, distance);
		if (distance >= 1000) {
			m_pos = DirectX::XMFLOAT3{ 0,0,0 };
			DX11MtxIdentity(m_mtx);
		}
	}

	// �P�t���[���O�̍��W
	DirectX::XMFLOAT3 m_oldPos;

	// �����蔻��N���X�̕ϐ�
	Collider* mCollider;

	// UV�A�j���[�V�����N���X�̕ϐ�
	UvAnimation* mUvAnim;
	// �p�����擾
	DirectX::XMFLOAT4X4 GetMtx() {
		return m_mtx;
	}

	// �ʒu���擾
	DirectX::XMFLOAT3 GetPos() {
		return m_pos;
	}

	// �h�c���擾
	uint64_t GetID() {
		return m_id;
	}
	
};

