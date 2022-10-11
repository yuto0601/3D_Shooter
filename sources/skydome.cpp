#include "skydome.h"
#include "dx11mathutil.h"
#include "gameobject.h"

bool Skydome::Init() {
	//s—ñ‰Šú‰»
	DX11MtxIdentity(m_mtx);

	//ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý

	return true;
}
void Skydome::Draw() {
	//ƒ‚ƒfƒ‹‚Ì•`‰æ
	m_model->Draw(m_mtx);
}
void Skydome::Update() {

}
void Skydome::Finalize() {
	m_model->Uninit();
}