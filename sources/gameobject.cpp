#include "GameObject.h"
#include "Game.h"
#include <vector>

using namespace DirectX;


static std::vector<GameObject*> mGameObjects;

GameObject::GameObject()
{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		rx = ry = rz = 0.0f;
		sx = sy = sz = 1.0f;
		speed = 0.0f;
		forwardVec = { 0,0,0 };
		m_oldPos = { 0,0,0 };
		mUvAnim = nullptr;
		mCollider = nullptr;
	
}

void GameObject::Update()
{
	// １フレーム前の座標を保存
	m_oldPos = XMFLOAT3(x, y, z);
	// 前向きベクトルとスピードでの移動処理
	x += forwardVec.x * speed;
	z += forwardVec.z * speed;

	// 当たり判定のパラメータ更新
	if (mCollider != nullptr)
		mCollider->Update();

	// UVアニメーションの更新
	if (mUvAnim != nullptr) {
		mUvAnim->Update();
		/*mConstBufferData.uv = mUvAnim->GetLeftTop();*/
	}
	else {
		//mConstBufferData.uv = XMFLOAT2(); // アニメーションしない
	}

	/*XMMATRIX mxRot;*/

}

void GameObject::Draw()
{

}
