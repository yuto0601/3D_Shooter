#pragma once
#include	<DirectXMath.h>

void UpdateSphereCamera(
	float radius,					// ���a(����)
	float elevation,				// �p�i���́j
	float azimuth,					// ���ʊp�i���́j
	const DirectX::XMFLOAT3& lookat);		// �����_�i���́j
