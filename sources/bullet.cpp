#include <memory>
#include "bullet.h"
#include "dx11mathutil.h"
#include "CModel.h"
#include "CDirectInput.h"
#include "ModelMgr.h"

constexpr float BULLETSPEED = 5.0f;
constexpr uint32_t BULLETLIFE = 60;

bool Bullet::Init() {
	bool sts = true;

	//�s�񏉊���
	DX11MtxIdentity(m_mtx);
	return sts;
}
void Bullet::SetInitialPos(DirectX::XMFLOAT3 Player_Pos)
{
                       m_pos= Player_Pos;
					  return;
}
void Bullet::Draw() {

	//���f���`��
	if (m_sts == BULLETSTATUS::LIVE) {
		m_pmodel->Draw(m_mtx);
	}
}
void Bullet::Update() {
	m_life--;
	if (m_life > 0) {
		m_pos.x = m_pos.x + m_speed * m_direction.x;
		m_pos.y = m_pos.y + m_speed * m_direction.y;
		m_pos.z = m_pos.z + m_speed * m_direction.z;

		m_mtx._41=m_pos.x;
		m_mtx._42 = m_pos.y;
		m_mtx._43 = m_pos.z;
}
	else {
		m_sts = BULLETSTATUS::DEAD;
	}
}
void Bullet::Finalize() {

}