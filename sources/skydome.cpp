#include "skydome.h"
#include "dx11mathutil.h"
#include "gameobject.h"

bool Skydome::Init() {
	//行列初期化
	DX11MtxIdentity(m_mtx);

	//モデルの読み込み

	return true;
}
void Skydome::Draw() {
	//モデルの描画
	m_model->Draw(m_mtx);
}
void Skydome::Update() {

}
void Skydome::Finalize() {
	m_model->Uninit();
}