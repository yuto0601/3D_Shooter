#pragma once
#include	"gameobject.h"
#include	"CModel.h"
#include	"dx11mathutil.h"
#include	"CDirectInput.h"

class Player :public GameObject {
private:
	CModel*	m_model;						// ３Ｄモデル
	float m_speed = 1.5f;//スピード
public:
	bool Init();

	void Draw();

	void DrawWithAxis();

	void Update();

	void Finalize();
	float GetM_Speed() {
		return m_speed;
	}
	float SetM_Speed(float speed) {
		m_speed = speed;
	}
 const CModel*  GetModel()  {
		return m_model;
	}
	void SetModel(CModel* pmodel) {
		m_model = pmodel;
	}
	void attraction() {
		float distance;
		DX11p2pLength(m_pos, DirectX::XMFLOAT3{ 0,0,0 }, distance);
		if (distance >= 1000) {

			m_pos = m_oldPos;

			XMFLOAT4 axisX;
			XMFLOAT4 axisY;
			XMFLOAT4 axisZ;

			//X軸を取り出す
			axisX.x = m_mtx._11;
			axisX.y = m_mtx._12;
			axisX.z = m_mtx._13;
			axisX.w = 0.0f;

			//Y軸を取り出す
			axisY.x = m_mtx._21;
			axisY.y = m_mtx._22;
			axisY.z = m_mtx._23;
			axisY.w = 0.0f;

			//Z軸を取り出す
			axisZ.x = m_mtx._31;
			axisZ.y = m_mtx._32;
			axisZ.z = m_mtx._33;
			axisZ.w = 0.0f;

			XMFLOAT4 qt;//クォータニオン
//行列からクォータニオンを生成
			DX11GetQtfromMatrix(m_mtx, qt);
			DX11MtxScale(1, 1, 1, m_mtx);
			XMFLOAT4 qtx, qty, qtz;//クォータニオン

			//指定軸回転のクォータニオンを生成
			DX11QtRotationAxis(qtx, axisX, 0);
			DX11QtRotationAxis(qty, axisY, 180);
			DX11QtRotationAxis(qtz, axisZ, 0);
			//クォータニオンを合成
			XMFLOAT4 tempqt1;
			DX11QtMul(tempqt1, qt, qtx);

			XMFLOAT4 tempqt2;
			DX11QtMul(tempqt2, qty, qtz);

			XMFLOAT4 tempqt3;
			DX11QtMul(tempqt3, tempqt1, tempqt2);

			//クォータニオンをノーマライズ
			DX11QtNormalize(tempqt3, tempqt3);
			//クォータニオンから行列を作成
			DX11MtxFromQt(m_mtx, tempqt3);
			//m_pos = DirectX::XMFLOAT3{ 0,0,0 };
			//DX11MtxIdentity(m_mtx);
		}
	}
};