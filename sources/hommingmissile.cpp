#include	<memory>
#include	"hommingmissile.h"
#include	"dx11mathutil.h"
#include	"CModel.h"
#include	"CDirectInput.h"
#include	"ModelMgr.h"

constexpr float BULLETSPEED = 5.0f;
constexpr uint32_t BULLETLIFE = 60;


bool HomingMissile::Init() {

	bool sts = true;

	// �s�񏉊���
	DX11MtxIdentity(m_mtx);

	return sts;
}

void HomingMissile::Draw() {

	// ���f���`��
	if (m_sts == HOMINGMISSILESTATUS::LIVE) {
		m_pmodel->Draw(m_mtx);
	}
}

void HomingMissile::Update() {

	m_life--;
	if (m_life > 0) {

		// �^�[�Q�b�g�ɂނ��Ĉʒu���X�V
		bool sts = Update(m_target, 5.0f);
		if (sts) {
			m_sts = HOMINGMISSILESTATUS::DEAD;
		}
	}
	else {
		m_sts = HOMINGMISSILESTATUS::DEAD;
	}

}

void HomingMissile::Finalize() {

}

/*---------------------------------------
�Q�̃x�N�g������N�I�[�^�j�I�����쐬����
-----------------------------------------*/
DirectX::XMFLOAT4 HomingMissile::RotationArc(DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, float& d) {

	DirectX::XMFLOAT3		Axis;		// ��
	DirectX::XMFLOAT4		q;			// �N�I�[�^�j�I��

	//  ���K��(�O���Ő��K���ς�)
	//	D3DXVec3Normalize(&v0,&v0);
	//	D3DXVec3Normalize(&v1,&v1);

	DX11Vec3Cross(Axis, v0, v1);

	DX11Vec3Dot(d, v0, v1);
	// �^�[�Q�b�g�̕����Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l���P�𒴂���ꍇ������i���������_�̌덷�̂��߁j
	if (d > 1.0) d = 1.0;							// �␳���Ă��
	// �^�[�Q�b�g�̕����Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l��-�P�������ꍇ������i���������_�̌덷�̂��߁j
	if (d <= -1.0) d = -1.0f;						// �␳���Ă��

	float s = (float)sqrtf((1 + d) * 2);
	if (s <= FLT_EPSILON) {

		//		DX11QtIdentity(q);			// �^�[�Q�b�g�ǂ��z����
		float radian = acos(d);

		q.x = Axis.x *sin(radian / 2.0f);
		q.y = Axis.y *sin(radian / 2.0f);
		q.z = Axis.z *sin(radian / 2.0f);
		q.w = cos(radian / 2.0f);

	}
	else {
		q.x = Axis.x / s;
		q.y = Axis.y / s;
		q.z = Axis.z / s;
		q.w = s / 2;
	}

	return q;
}

/*----------------------------------------
	�~�T�C���̃f�[�^���X�V����

	(�^�[�Q�b�g�Ƃ̋�����lng��菬�����Ȃ�����true��Ԃ�)
-----------------------------------------*/
bool HomingMissile::Update(const DirectX::XMFLOAT3 TargetPosition, float l) {

	bool sts = false;
	float lng;

	DirectX::XMFLOAT4 TargetQt;					// �^�[�Q�b�g�����̎p��

	DirectX::XMFLOAT3 TargetVector;

	// ���݈ʒu����^�[�Q�b�g�����ւ̃x�N�g�������߂�
	TargetVector.x = TargetPosition.x - m_pos.x;
	TargetVector.y = TargetPosition.y - m_pos.y;
	TargetVector.z = TargetPosition.z - m_pos.z;

	DX11Vec3Length(TargetVector, lng);
	if (lng <= l) {
		return true;
	}

	DirectX::XMFLOAT3 ZDir = m_Velocity;			// �~�T�C���̑��x�x�N�g���iZ�����j

	DX11Vec3Normalize(TargetVector, TargetVector);	// ���K��
	DX11Vec3Normalize(ZDir, ZDir);					// ���K��

	float	Dot;										// �Q�{�̃x�N�g���̓��ϒl�i���݂̐i�s�����ƃ^�[�Q�b�g�����j
	TargetQt = RotationArc(ZDir, TargetVector, Dot);	// �Q�{�̃x�N�g������ׂ��p�x�ƃN�I�[�^�j�I�������߂�
	float angleDiff = acosf(Dot);						// ���W�A���p�x

	/*
	���̕��������w�I�Ȏ�_�����\�[�X�i�q���������������`�����ɒu��������j

	�P�j�p�x�̔����Q�̃x�N�g���̊O�ς͌���Ȃ��������Ȃ�i�덷�𐶂ށj
	�Q�j�^�[�Q�b�g�̕����Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l���P�𒴂���ꍇ������i���������_�̌덷�̂��߁j

	float Dot = D3DXVec3Dot(&TargetVector,&ZDir);	// �^�[�Q�b�g�̕����Ǝ��@�����̊p�x��
	// �^�[�Q�b�g�̕����Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l���P�𒴂���ꍇ������i���������_�̌덷�̂��߁j
	if(Dot>1.0f) Dot=1.0f;							// �␳���Ă��
	// �^�[�Q�b�g�̕����Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l��-�P�������ꍇ������i���������_�̌덷�̂��߁j
	if(Dot<-1.0f) Dot=-1.0f;							// �␳���Ă��
	float angleDiff = acos(Dot);					// ���W�A���p�x

	// �^�[�Q�b�g�����������N�H�[�^�j�I�����v�Z����
	D3DXVECTOR3 Axis;
	D3DXVec3Cross(&Axis,&ZDir,&TargetVector);

	// Y������]���Ƃ��ăN�H�[�^�j�I������]������B
	D3DXQuaternionRotationAxis( &TargetQt, &Axis, angleDiff);
	*/

	// �~�T�C���̎p�������肷��
	if (m_AddRotMax >= angleDiff) {
		// 1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���������ꍇ
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		// 1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���傫���ꍇ
		float t = m_AddRotMax / angleDiff;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);
	}

	// �N�H�[�^�j�I�����s��ɂ���
	// ���݂̎p�����N�I�[�^�j�I���ɂ���
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// �~�T�C���̐i�s�����ɃX�s�[�h���|���Ĉړ��ʁi���x�x�N�g�������߂�j
	m_Velocity.x = m_mtx._31*m_speed;
	m_Velocity.y = m_mtx._32*m_speed;
	m_Velocity.z = m_mtx._33*m_speed;

	// �ʒu���X�V�i���x�x�N�g���ōX�V�j
	m_pos.x += m_Velocity.x;
	m_pos.y += m_Velocity.y;
	m_pos.z += m_Velocity.z;

	// �s��Ɉʒu���W���Z�b�g
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	return sts;
}
