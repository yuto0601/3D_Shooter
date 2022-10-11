#pragma once

#include	<DirectXMath.h>
#include	<vector>
#include	<memory>
#include	"gameobject.h"
#include	"dx11mathutil.h"

// 前方宣言
class CModel;

// 弾ステータス
enum class HOMINGMISSILESTATUS {
	LIVE,
	DEAD
};

class HomingMissile :public GameObject {
private:
	HOMINGMISSILESTATUS	m_sts;			// ステータス
	CModel*		m_pmodel;				// ３Ｄモデル
	DirectX::XMFLOAT3	m_direction;	// 発射方向
	float		m_speed = 2.0f;			// スピード
	uint32_t	m_life = 240;			// 寿命	

	DirectX::XMFLOAT3		m_Velocity;		// ミサイルの現在速度
	DirectX::XMFLOAT4		m_Quaternion;	// 現在の姿勢を表すクォータニオン
	float					m_AddRotMax= DirectX::XM_PI*2.0f / 180.0f;	// 最大回転角度

	DirectX::XMFLOAT3		m_target;		// ホーミングするターゲット位置

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
		DX11GetQtfromMatrix(mtx, m_Quaternion);		// 行列からクオータニオンを生成
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
