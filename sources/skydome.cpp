#include "skydome.h"
#include "dx11mathutil.h"
#include "gameobject.h"

bool Skydome::Init() {
	//�s�񏉊���
	DX11MtxIdentity(m_mtx);

	//���f���̓ǂݍ���

	return true;
}
void Skydome::Draw() {
	//���f���̕`��
	m_model->Draw(m_mtx);
}
void Skydome::Update() {

}
void Skydome::Finalize() {
	m_model->Uninit();
}