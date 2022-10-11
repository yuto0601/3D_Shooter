#pragma once
#include <DirectXMath.h>
#include<vector>
#include<memory>
#include "gameobject.h"

//�O���錾
class CModel;
//�e�X�e�[�^�X
enum class BULLETSTATUS {
	LIVE,
	DEAD
};
class Bullet:public GameObject{
private:
	BULLETSTATUS m_sts;//�X�e�[�^�X
	CModel* m_pmodel;//�RD���f��
	DirectX::XMFLOAT3 m_direction;//���˕���
	float m_speed = 4.0f;
	uint32_t m_life = 60;
public:
	bool Init();
	void SetInitialPos(DirectX::XMFLOAT3 Player_Pos);
	void Draw();
	void Update();
	void Finalize();
	void SetModel(CModel* p) {
		m_pmodel = p;
	}
	
	CModel* GetModel() {
		return m_pmodel;
	}
	void SetDirection(DirectX::XMFLOAT4X4 mtx) {
		m_mtx = mtx;
		m_direction = DirectX::XMFLOAT3(mtx._31, mtx._32, mtx._33);
	}
	bool isLive() {
		if (m_sts == BULLETSTATUS::LIVE)
		{
			return true;
		}
		else {
			return false;
		}
	}
};

