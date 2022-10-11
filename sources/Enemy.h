#pragma once
#include <random>
#include"gameobject.h"
#include"CModel.h"
#include"dx11mathutil.h"
#include"CDirectInput.h"
#include"BoundingSphere.h"
class Enemy :public GameObject {
private:
	CModel* m_model;           //3D���f��
	BoundingSphere m_bs;
	//���
	enum class STATUS {
		S1_MOVE_FLY = 0,
		S1_MOVE_BOMB,
	};
	enum class LIVE_OR_DEAD {
		LIVE,
		DEAD
	};
	enum class ACTION {
		ACT_RIGHT = 0,
		ACT_LEFT,
		ACT_UP,
		ACT_DOWN,
		ACT_ROLLP,
		ACT_ROLLM,
		ACT_STRAIGHT,
		MAX_ACTION
	};
	float m_speed = 0.01;//�X�s�[�h
	XMFLOAT3 m_angle;//��]��
	std::mt19937 m_mt;//��������

	ACTION m_action;//�A�N�V�������
	
	unsigned int m_actionKeepTime=2;//�A�N�V�����J�E���^
	LIVE_OR_DEAD m_sts;
	
public:
	bool kill_flg;
	bool Init();
	void Draw();
	void BSDraw();
	void DrawWithAxis();
	void Update(int act, int time);
	void Finalize();
	void AutoPilot(int act,int time);
    void  Getm_sts(Enemy g_enemy);
	CModel* GetModel()
	{
		return m_model;
	}
	void SetModel(CModel* pmodel_enemy) {
		m_model = pmodel_enemy;
	}
	HITDATA GetBS() { return m_bs.GetHitdata(); }
	bool isDeath() {
		if (kill_flg==true)
		{
			return true;
		}
		else {
			return false;
		}
	}

	bool isOut() {
		float distance;
		DX11p2pLength(m_pos, DirectX::XMFLOAT3{ 0,0,0 }, distance);
		if (distance >= 1000) { return true; }
		else { return false; }
	}

	void attraction() {
		m_pos = DirectX::XMFLOAT3{ 0,0,0 };
		DX11MtxIdentity(m_mtx);
			
	}
};