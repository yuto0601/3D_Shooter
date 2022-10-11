#include		<DirectXMath.h>
#include		"CCamera.h"

// �p�x�����W�A���ɕύX
inline float ToRad(float angle) {
	return (angle *  DirectX::XM_PI) / 180.0f;
}

//  
// ���ʑ����J����(�y����]�Ȃ��o�[�W����)
//
void UpdateSphereCamera(
	float radius,						// ���a(����)
	float elevation,					// �p�i���́j
	float azimuth,						// ���ʊp�i���́j
	const DirectX::XMFLOAT3& lookat)	// �����_�i���́j
{		

	DirectX::XMFLOAT3 eye;				// ���_

	// �p�A���ʊp�����W�A���ɕϊ�
	elevation = ToRad(elevation);
	azimuth = ToRad(azimuth);

	// �p�ɉ��������a���v�Z
	float r = radius*sinf(elevation);

	// ���_���W���v�Z
	eye.x = r*cosf(azimuth);
	eye.y = radius*cosf(elevation);
	eye.z = r*sinf(azimuth);

	// ������x�N�g�����v�Z(�p�����X�O�x��]������Ƃł���)
	DirectX::XMFLOAT3 up;
	float elevationplus90 = elevation + ToRad(90.0f);			// �p���X�O�x����

	r = radius * sinf(elevationplus90);	

	up.x = r * cosf(azimuth);				
	up.y = radius * cosf(elevationplus90);
	up.z = r * sinf(azimuth);

	// ���_�x�N�g���𒍎��_�����s�ړ�
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;

	// �r���[�ϊ��s����쐬����
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetUp(up);
	CCamera::GetInstance()->CreateCameraMatrix();
}
