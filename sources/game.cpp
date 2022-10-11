#include <cinttypes>
#include "dx11util.h"
#include "CModel.h"
#include "CCamera.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "Application.h"
#include "CDirectInput.h"
#include "AirFight.h"
#include "skydome.h"
#include "game.h"
#include "Fader.h"
#include"myimgui.h"
SCENENUM sceneNUM;
extern bool nextflg;
extern bool nextflg;
//1�Ԗڂ̏����i�������j
void GameInit() {
	// DX11������
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	imguiInit();

	// �J�������K�v
	DirectX::XMFLOAT3 eye(0, 00, -30);		// �J�����̈ʒu
	DirectX::XMFLOAT3 lookat(0, 0, 0);		// �����_
	DirectX::XMFLOAT3 up(0, 1, 0);			// �J�����̏�����x�N�g��

	CCamera::GetInstance()->Init(
		10.0f,							// �j�A�N���b�v
		10000.0f,						// �t�@�[�N���b�v
		XM_PI / 5.0f,					// ����p
		static_cast<float>(Application::CLIENT_WIDTH),		// �X�N���[����
		static_cast<float>(Application::CLIENT_HEIGHT),		// �X�N���[���̍���
		eye, lookat, up);					// �J�����̃f�[�^

	// ���s�������Z�b�g
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

	// DIRECTINPUT������
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,			// �X�N���[����
		Application::CLIENT_HEIGHT			// �X�N���[���̍���
		);

	// �A���t�@�u�����h�L����
	TurnOnAlphablend();
sceneNUM= SCENENUM::TITLE_INIT;
	switch (sceneNUM) {
	case SCENENUM::TITLE_INIT:
		FaderInit();                         //�t�F�[�_�[������
		    Title();                    //�^�C�g�����
		PlayGameInit();             //���C���Q�[��
		    ResultInit();                 //���U���g
			sceneNUM = SCENENUM::TITLE;

}
	nextflg = false;

}
//���͏���
void GameInput(uint64_t dt) {
	CDirectInput::GetInstance().GetKeyBuffer();
}
//2�Ԗڂ̏����i�X�V�j
void GameUpdate(uint64_t dt) {
	switch (sceneNUM) {
	case SCENENUM::TITLE_INIT:
		FaderUpdate();         //�t�F�[�_�[
	
		if (nextflg == true)
		{
			PlayGameInit();
			FaderInit();
			nextflg = false;
			sceneNUM = SCENENUM::TITLE;
	}
		break;
	case SCENENUM::TITLE:
		TitleUpdate();                            //�^�C�g����ʂ̍X�V
		if (nextflg == true)
		{
			FaderInit();
			nextflg = false;
			sceneNUM = SCENENUM::GAME;
		}
		break;
	case SCENENUM::GAME_INIT:
		FaderUpdate();                       //�t�F�[�_�[
		if (nextflg == true)
		{
			FaderInit();
			nextflg = false;
			sceneNUM = SCENENUM::GAME;
		}
		break;
	case SCENENUM::GAME:
		PlayGameUpdate();               // ���C���Q�[���X�V
		if (nextflg == true)
		{
			
			nextflg = false;
			sceneNUM = SCENENUM::RESULT_INIT;
			FaderInit();
		}
		break;
	case SCENENUM::RESULT_INIT:
		FaderUpdate();                     //�t�F�[�_�[
		if (nextflg == true)
		{
			nextflg = false;
			sceneNUM = SCENENUM::RESULT;
		}
		break;
	case SCENENUM::RESULT:                          //���U���g�̍X�V
		ResultUpdate();
		if (nextflg == true)
		{
			nextflg = false;
			sceneNUM = SCENENUM::TITLE_INIT;
		}
		break;
	}

	
}
//3�Ԗڂ̏���(�`��)
void GameRender(uint64_t dt) {

	float col[4] = { 1,1,1,1 };

	// �`��O����
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// �v���W�F�N�V�����ϊ��s��擾
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// �r���[�ϊ��s����擾
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// �Q�[���`��
	switch (sceneNUM) {
	case SCENENUM::TITLE_INIT:	       // �t�F�[�_�[
		FaderDraw();
		break;
	case SCENENUM::TITLE:               //�^�C�g����ʂ̕`��
		TitleDraw();
		break;
	case SCENENUM::GAME_INIT:     //�t�F�[�_�[
		FaderDraw();
		break;
	case SCENENUM::GAME:             //���C���Q�[���̕`��
		PlayGameDraw();
		break;
	case SCENENUM::RESULT_INIT:  //�t�F�[�_�[
		FaderDraw();
		break;

	case SCENENUM::RESULT:       //���U���g��ʂ̕`��
		ResultDraw();
		break;
	}

//	skydome.Draw();
	// �`��㏈��
	DX11AfterRender();
}
//4�Ԗڂ̏���(���)
void GameDispose() {
	TitleExit();
	PlayGameExit();
	FaderExit();
	ResultExit();
	imguiExit();
	DX11Uninit();//�ʒu�ƃO���t�B�b�N�̉���i����͑S���̃P�[�X�Ŏg�p����j
}