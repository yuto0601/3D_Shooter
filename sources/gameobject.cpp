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
	// �P�t���[���O�̍��W��ۑ�
	m_oldPos = XMFLOAT3(x, y, z);
	// �O�����x�N�g���ƃX�s�[�h�ł̈ړ�����
	x += forwardVec.x * speed;
	z += forwardVec.z * speed;

	// �����蔻��̃p�����[�^�X�V
	if (mCollider != nullptr)
		mCollider->Update();

	// UV�A�j���[�V�����̍X�V
	if (mUvAnim != nullptr) {
		mUvAnim->Update();
		/*mConstBufferData.uv = mUvAnim->GetLeftTop();*/
	}
	else {
		//mConstBufferData.uv = XMFLOAT2(); // �A�j���[�V�������Ȃ�
	}

	/*XMMATRIX mxRot;*/

}

void GameObject::Draw()
{

}
