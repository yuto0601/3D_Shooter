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
//1番目の処理（初期化）
void GameInit() {
	// DX11初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	imguiInit();

	// カメラが必要
	DirectX::XMFLOAT3 eye(0, 00, -30);		// カメラの位置
	DirectX::XMFLOAT3 lookat(0, 0, 0);		// 注視点
	DirectX::XMFLOAT3 up(0, 1, 0);			// カメラの上向きベクトル

	CCamera::GetInstance()->Init(
		10.0f,							// ニアクリップ
		10000.0f,						// ファークリップ
		XM_PI / 5.0f,					// 視野角
		static_cast<float>(Application::CLIENT_WIDTH),		// スクリーン幅
		static_cast<float>(Application::CLIENT_HEIGHT),		// スクリーンの高さ
		eye, lookat, up);					// カメラのデータ

	// 平行光源をセット
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,			// スクリーン幅
		Application::CLIENT_HEIGHT			// スクリーンの高さ
		);

	// アルファブレンド有効化
	TurnOnAlphablend();
sceneNUM= SCENENUM::TITLE_INIT;
	switch (sceneNUM) {
	case SCENENUM::TITLE_INIT:
		FaderInit();                         //フェーダーを挟む
		    Title();                    //タイトル画面
		PlayGameInit();             //メインゲーム
		    ResultInit();                 //リザルト
			sceneNUM = SCENENUM::TITLE;

}
	nextflg = false;

}
//入力処理
void GameInput(uint64_t dt) {
	CDirectInput::GetInstance().GetKeyBuffer();
}
//2番目の処理（更新）
void GameUpdate(uint64_t dt) {
	switch (sceneNUM) {
	case SCENENUM::TITLE_INIT:
		FaderUpdate();         //フェーダー
	
		if (nextflg == true)
		{
			PlayGameInit();
			FaderInit();
			nextflg = false;
			sceneNUM = SCENENUM::TITLE;
	}
		break;
	case SCENENUM::TITLE:
		TitleUpdate();                            //タイトル画面の更新
		if (nextflg == true)
		{
			FaderInit();
			nextflg = false;
			sceneNUM = SCENENUM::GAME;
		}
		break;
	case SCENENUM::GAME_INIT:
		FaderUpdate();                       //フェーダー
		if (nextflg == true)
		{
			FaderInit();
			nextflg = false;
			sceneNUM = SCENENUM::GAME;
		}
		break;
	case SCENENUM::GAME:
		PlayGameUpdate();               // メインゲーム更新
		if (nextflg == true)
		{
			
			nextflg = false;
			sceneNUM = SCENENUM::RESULT_INIT;
			FaderInit();
		}
		break;
	case SCENENUM::RESULT_INIT:
		FaderUpdate();                     //フェーダー
		if (nextflg == true)
		{
			nextflg = false;
			sceneNUM = SCENENUM::RESULT;
		}
		break;
	case SCENENUM::RESULT:                          //リザルトの更新
		ResultUpdate();
		if (nextflg == true)
		{
			nextflg = false;
			sceneNUM = SCENENUM::TITLE_INIT;
		}
		break;
	}

	
}
//3番目の処理(描画)
void GameRender(uint64_t dt) {

	float col[4] = { 1,1,1,1 };

	// 描画前処理
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// プロジェクション変換行列取得
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// ビュー変換行列を取得
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// ゲーム描画
	switch (sceneNUM) {
	case SCENENUM::TITLE_INIT:	       // フェーダー
		FaderDraw();
		break;
	case SCENENUM::TITLE:               //タイトル画面の描画
		TitleDraw();
		break;
	case SCENENUM::GAME_INIT:     //フェーダー
		FaderDraw();
		break;
	case SCENENUM::GAME:             //メインゲームの描画
		PlayGameDraw();
		break;
	case SCENENUM::RESULT_INIT:  //フェーダー
		FaderDraw();
		break;

	case SCENENUM::RESULT:       //リザルト画面の描画
		ResultDraw();
		break;
	}

//	skydome.Draw();
	// 描画後処理
	DX11AfterRender();
}
//4番目の処理(解放)
void GameDispose() {
	TitleExit();
	PlayGameExit();
	FaderExit();
	ResultExit();
	imguiExit();
	DX11Uninit();//位置とグラフィックの解放（これは全部のケースで使用する）
}