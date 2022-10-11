#include	"player.h"
#include	"drawaxis.h"


bool Player::Init() {
	// 行列初期化
	DX11MtxIdentity(m_mtx);
	m_pos = DirectX::XMFLOAT3{ 0,0,0 };
	m_speed = 2.0f;
	HP = 20;
	return true;
}

void Player::Draw() {
	// モデル描画
	m_model->Draw(m_mtx);
}

void Player::DrawWithAxis() {
	// モデル描画
	drawaxis(m_mtx, 200, m_pos);
	Draw();
}
float rot=3.0f;
void Player::Update() {
	XMFLOAT3 angle = { 0,0,0 };
	XMFLOAT3 camera_angle = { 0,0,0 };
	bool keyinput = false;
	
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
		angle.x -= rot;
		camera_angle.x -= rot;
		keyinput = true;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
		angle.x += rot;
		camera_angle.x += rot;
		keyinput = true;
	}if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
		angle.y -= rot;
		camera_angle.y -= rot;
		keyinput = true;
	}if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
		angle.y += rot;
		camera_angle.y += rot;
		keyinput = true;
	}if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Q)) {
		angle.z += rot;
		camera_angle.z += rot;
		keyinput = true;
	}if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_E)) {
		angle.z -= rot;
		camera_angle.z -= rot;
		keyinput = true;
	}
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//X軸を取り出す
	axisX.x = m_mtx._11;
	axisX.y = m_mtx._12;
	axisX.z = m_mtx._13;
	axisX.w =0.0f;

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

	if (keyinput)
	{
		XMFLOAT4 qt;//クォータニオン
		//行列からクォータニオンを生成
		DX11GetQtfromMatrix(m_mtx, qt);
		DX11MtxScale(1,1,1,m_mtx);
		XMFLOAT4 qtx, qty, qtz;//クォータニオン

		//指定軸回転のクォータニオンを生成
		DX11QtRotationAxis(qtx, axisX, angle.x);
		DX11QtRotationAxis(qty, axisY, angle.y);
		DX11QtRotationAxis(qtz, axisZ, angle.z);
		//クォータニオンを合成
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		//クォータニオンをノーマライズ
		DX11QtNormalize(tempqt3,tempqt3);
		//クォータニオンから行列を作成
		DX11MtxFromQt(m_mtx, tempqt3);
	}
	//Z軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w= 0.0f;

	m_oldPos = m_pos;

	m_pos.x += axisZ.x *m_speed;
	m_pos.y += axisZ.y * m_speed;
	m_pos.z += axisZ.z * m_speed;

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
	
	}

void Player::Finalize() {
	m_model->Uninit();
}


