#include "Collider.h"
#include <DirectXMath.h>
#include "GameObject.h"

using namespace DirectX;

bool Collider::Test(Collider* c1, Collider* c2)
{
	// 引数で受け取ったColliderが球か線分かを判定する
	if (c1->mType == SPHERE)
	{
		if (c2->mType == SPHERE)
		{
			// 球vs球の当たり判定を実行
			// 中心同士を結ぶベクトルを作る
			XMVECTOR centerVec = c2->centerPos - c1->centerPos;
			// 作ったベクトルの長さを計測
			XMVECTOR centerLen = XMVector3Length(centerVec);
			// 中心ベクトルの長さが、球①②の半径を足した値より小さいか？
			if (XMVectorGetX(centerLen) < c1->radius + c2->radius)
			{
				return true;
			}
		}
		else if (c2->mType == LINE)
		{
			// 球vs線分
		}
	}
	else if (c1->mType == LINE)
	{
		if (c2->mType == SPHERE)
		{
			// 球vs線分
		}
	}

	return false;
}

void Collider::Update()
{
	// 球の中心点を更新
	centerPos = XMVectorSet(mOwner->x, mOwner->y, mOwner->z, 0);
}

Collider* Collider::CreateSphere(GameObject* pOwner, DirectX::XMFLOAT3 centerPos, float radius)
{
	Collider* pCollider = new Collider();
	pCollider->mType = SPHERE;
	pCollider->mOwner = pOwner;
	pCollider->centerPos = XMLoadFloat3(&centerPos);
	pCollider->radius = radius;
	return pCollider;
}
