#pragma once

#include <DirectXMath.h>

class Collider
{

public:
	static bool Test(Collider* c1, Collider* c2);

	void Update(); // パラメータの更新

	// 球タイプのColliderインスタンスを生成する
	static Collider* CreateSphere(
		class GameObject* pOwner, DirectX::XMFLOAT3 centerPos, float radius);

	// このColliderが球か線分かを表す変数
	enum TYPE {
		SPHERE, // 球
		LINE,
	};

	TYPE mType;

	// 所有者GameObjectのポインタ
	class GameObject* mOwner;

	// 球の場合に必要な変数
	DirectX::XMVECTOR centerPos; // 中心点座標
	float radius; // 半径

};
