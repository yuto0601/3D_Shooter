#pragma once

#include	<DirectXMath.h>
#include	<vector>
#include	<memory>
#include	"gameobject.h"
#include	"dx11mathutil.h"

// �O���錾
class CModel;

// �e�X�e�[�^�X
enum class HOMINGMISSILESTATUS {
	LIVE,
	DEAD
};

class HomingMissile :public GameObject {
private:
	HOMINGMISSILESTATUS	m_sts;			// �X�e�[�^�X
	CModel*		m_pmodel;				// �R�c���f��
	DirectX::XMFLOAT3	m_direction;	// ���˕���
	float		m_speed = 2.0f;			// �X�s�[�h
	uint32_t	m_life = 240;			// ����	

	DirectX::XMFLOAT3		m_Velocity;		// �~�T�C���̌��ݑ��x
	DirectX::XMFLOAT4		m_Quaternion;	// ���݂̎p����\���N�H�[�^�j�I��
	float					m_AddRotMax= DirectX::XM_PI*2.0f / 180.0f;	// �ő��]�p�x

	DirectX::XMFLOAT3		m_target;		// �z�[�~���O����^�[�Q�b�g�ʒu

	DirectX::XMFLOAT4 RotationArc(DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, float& d);

	bool Update(const DirectX::XMFLOAT3 TargetPosition, float l);

public:
	bool Init();

	void Draw();

	void Update();

	void Update(XMFLOAT3 target);

	void Finalize();

	void SetModel(CModel* p) {
		m_pmodel = p;
	}

	CModel* GetModel() {
		return m_pmodel;
	}

	void SetDirection(DirectX::XMFLOAT4X4 mtx) {
		m_direction = DirectX::XMFLOAT3(mtx._31, mtx._32, mtx._33);
	}

	void SetVelocity(DirectX::XMFLOAT3 vel) {
		m_Velocity = vel;
	}

	void SetInitailQuaternion(DirectX::XMFLOAT4X4 mtx) {
		DX11GetQtfromMatrix(mtx, m_Quaternion);		// �s�񂩂�N�I�[�^�j�I���𐶐�
	}

	bool isLive() {
		if (m_sts == HOMINGMISSILESTATUS::LIVE) {
			return true;
		}
		else {
			return false;
		}
	}

	void SetTarget(DirectX::XMFLOAT3 targetpos) {
		m_target = targetpos;
	}
};
