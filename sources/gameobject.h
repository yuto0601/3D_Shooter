#pragma once
#include	<directxmath.h>
#include "Collider.h"
#include "UvAnimation.h"
#include"dx11mathutil.h"



class GameObject {
protected:
	DirectX::XMFLOAT3 m_pos{};				// 位置
	DirectX::XMFLOAT4X4 m_mtx{};				// 姿勢
	int HP = 1;									// ヒットポイント
public:
	uint64_t m_id = 0;						// 識別子
	GameObject();//初期化
	 void Update();		// 更新処理
	 void Draw();			// 描画処理
	virtual ~GameObject() {};//解放

	// トランスフォーム（移動）情報
	float x, y, z;  // 位置（ワールド座標）
	float rx, ry, rz;  // 回転角度
	float sx, sy, sz;
    float speed;
	// 前向きベクトルを持つ変数
	DirectX::XMFLOAT3 forwardVec;
	
	//引き戻し処理
	void attraction() {
		float distance;
		DX11p2pLength(m_pos, DirectX::XMFLOAT3{ 0,0,0 }, distance);
		if (distance >= 1000) {
			m_pos = DirectX::XMFLOAT3{ 0,0,0 };
			DX11MtxIdentity(m_mtx);
		}
	}

	// １フレーム前の座標
	DirectX::XMFLOAT3 m_oldPos;

	// 当たり判定クラスの変数
	Collider* mCollider;

	// UVアニメーションクラスの変数
	UvAnimation* mUvAnim;
	// 姿勢を取得
	DirectX::XMFLOAT4X4 GetMtx() {
		return m_mtx;
	}

	// 位置を取得
	DirectX::XMFLOAT3 GetPos() {
		return m_pos;
	}

	// ＩＤを取得
	uint64_t GetID() {
		return m_id;
	}
	
};

