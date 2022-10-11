#include "Enemy.h"
#include "drawaxis.h"
#include <time.h>
bool Enemy::Init() {
	//生きているかどうか
	bool m_sts = true;
	m_pos = DirectX::XMFLOAT3{ 0,0,0 };
	//行列初期化
	DX11MtxIdentity(m_mtx);
	m_mtx._41 = 1;
	m_mtx._42 = 1;
	m_mtx._43 = 1000;
	m_speed = (rand()%6+11)/10;
	//モデル表示

		//Enemyの境界球を作成
	{
		//モデルの全頂点を抜き出す
		const CModel* pmodel = GetModel();
		const ModelData& md = pmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		std::vector<XMFLOAT3>vertices;

		for (auto& m : meshes) {
			for (auto& v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}
		m_bs.SetR(5.0f);
		//境界球初期化
		m_bs.Init(
			vertices,                           //頂点群
			XMFLOAT4(1, 0.4, 1, 0.3f));//頂点カラー
	}
	return m_sts;
}

void Enemy::AutoPilot(int act,int time)
{
	//回転角をいったんリセット
	m_angle.x = m_angle.y = m_angle.z = 0.0f;
	//アクションにより観点角をセット
	switch (m_action) {
	case Enemy::ACTION::ACT_STRAIGHT://直進
		break;
	case Enemy::ACTION::ACT_LEFT:
		m_angle.y = 1.0f;
		break;
	case Enemy::ACTION::ACT_RIGHT:
		m_angle.y = -1.0f;
		case Enemy::ACTION::ACT_UP:
			m_angle.x = 1.0f;
			break;
		case Enemy::ACTION::ACT_DOWN:
			m_angle.x = -1.0f;
			break;
		case Enemy::ACTION::ACT_ROLLP:
			m_angle.z = 1.0f;
			break;
			case Enemy::ACTION::ACT_ROLLM:
				m_angle.z = 1.0f;
				break;
	}
	//カウンター減算
	if (--m_actionKeepTime <= 0) {
		//乱数により次のアクションと持続時間を設定
		m_action = static_cast<ACTION>(act);
		m_actionKeepTime = time;//rand()%30+1;
	}
	
	m_bs.UpdatePosition(m_mtx);

}
void Enemy::Getm_sts(Enemy g_enemy)
{
	g_enemy.kill_flg=false;
	
}
void Enemy::Draw() {
	//モデル描画
	m_model->Draw(m_mtx);
}
void Enemy::BSDraw() {
	//モデル描画
	m_bs.Draw();
}
void Enemy::DrawWithAxis()
{
	//モデル描画
	drawaxis(m_mtx, 200, m_pos);
	Draw();
}
//actは0～6のみ
void Enemy::Update(int act, int time) {
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	AutoPilot(act , time);

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

	//Ｚ軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;

	//
	XMFLOAT4 qt;//クォータニオン

	//行列カラクオータニオンを生成
	DX11GetQtfromMatrix(m_mtx, qt);
	XMFLOAT4 qtx, qty, qtz;//クォータニオン
	//指定軸回転のクォータニオンを生成
	DX11QtRotationAxis(qtx, axisX, m_angle.x);
		DX11QtRotationAxis(qty, axisY, m_angle.y);
		DX11QtRotationAxis(qtz, axisZ, m_angle.z);
		//クォータニオンを合成
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		//クォータニオンをノーマライズ
		DX11QtNormalize(tempqt3, tempqt3);
		//クオータニオンから行列を作成
		DX11MtxFromQt(m_mtx, tempqt3);
		axisZ.x= m_mtx._31;
		axisZ.y = m_mtx._32;
		axisZ.z = m_mtx._33;
		axisZ.w = 0.0f;
		m_pos.x += axisZ.x * m_speed;
		m_pos.y += axisZ.y * m_speed;
		m_pos.z += axisZ.z * m_speed;
		m_pos.x += axisZ.x * m_speed;

		m_oldPos = m_pos;

        m_mtx._41 = m_pos.x;
		m_mtx._42 = m_pos.y;
		m_mtx._43 = m_pos.z;
}

void Enemy::Finalize() {
	m_model->Uninit();
	m_bs.Exit();
}