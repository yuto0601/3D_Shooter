#include "Collider.h"
#include <DirectXMath.h>
#include "GameObject.h"

using namespace DirectX;

bool Collider::Test(Collider* c1, Collider* c2)
{
	// �����Ŏ󂯎����Collider�������������𔻒肷��
	if (c1->mType == SPHERE)
	{
		if (c2->mType == SPHERE)
		{
			// ��vs���̓����蔻������s
			// ���S���m�����ԃx�N�g�������
			XMVECTOR centerVec = c2->centerPos - c1->centerPos;
			// ������x�N�g���̒������v��
			XMVECTOR centerLen = XMVector3Length(centerVec);
			// ���S�x�N�g���̒������A���@�A�̔��a�𑫂����l��菬�������H
			if (XMVectorGetX(centerLen) < c1->radius + c2->radius)
			{
				return true;
			}
		}
		else if (c2->mType == LINE)
		{
			// ��vs����
		}
	}
	else if (c1->mType == LINE)
	{
		if (c2->mType == SPHERE)
		{
			// ��vs����
		}
	}

	return false;
}

void Collider::Update()
{
	// ���̒��S�_���X�V
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
