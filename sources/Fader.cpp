#include "Fader.h"
#include"quad2d.h"
#include "CDirectInput.h"
#include "AirFight.h"

bool nextflg;
static bool* nextflgBuffer = &nextflg;

extern SCENENUM sceneNUM;

STATE mState;
Quad2D Fader;
Quad2D* FaderBuffer;
DirectX::XMFLOAT2 uvfader[4] = { XMFLOAT2(0,0),XMFLOAT2(1,0) ,XMFLOAT2(0,1) ,XMFLOAT2(1,1) };//uv座標
void FaderInit()
{
	switch (sceneNUM)
	{
	case SCENENUM::TITLE_INIT:
		//タイトル画面の初期化
		Fader.Init(1280, 1280, { 1,1,1,1 }, uvfader);
		Fader.SetScale(1, 1, 1);
		Fader.SetPosition(640, 360, 1);
		Fader.LoadTexture("assets/starwars/texture/TitleScreen.jpg");
		mState = NONE;
		break;
	case SCENENUM::TITLE:
		
		break;
	case SCENENUM::GAME_INIT:
		
		break;
	case SCENENUM::GAME:
		
		break;
	case SCENENUM::RESULT_INIT:
		//タイトル画面の初期化
		Fader.Init(1280, 1280, { 1,1,1,1 }, uvfader);
		Fader.SetScale(1, 1, 1);
		Fader.SetPosition(640, 360, 1);
		Fader.LoadTexture("assets/starwars/texture/gameoverscreen.png");
		mState = NONE;
		break;
	case SCENENUM::RESULT:
		
		break;
	case SCENENUM::FADE:
		
		break;
	default:
		
		break;
	}
	//フェーダー
	a = 0;

	FaderFadeOut();

	//ゲーム本編初期化

}

bool FaderUpdate()
{
	 // パネルの透明度への参照を取得
	
	
	switch (FaderGetState())
	{
	case FADEIN:
		a -= 0.01f;
		if (a <= 0.0f)
		{
			a = 0;
			mState =NONE;
		}
		break;

	case FADEOUT:
		a += 0.01f;
		if (a >= 1.0f)
		{
			a = 1;
			mState = NONE;
		}
		break;
	case NONE:
	
		break;
	}

	//２D短形描画
	TurnOffZbuffer();//Zバッファオフ
	static int loop = 0;
	loop++;
	static int idx = 0;
	if (loop % 30 == 29) {
		idx++;
		idx = idx % 7;
	}
	XMFLOAT2 uv[7][4] = {
		{
			{0.0f / 8.0f,0.0f},
			{1.0f / 8.0f,0.0},
			{1.0f / 8.0f,1.0f / 4.0f},
			{1.0f / 8.0f,1.0f / 4.0f}
		},
			{
			{1.0f / 8.0f,0.0f},
			{2.0f / 8.0f,0.0},
			{1.0f / 8.0f,1.0f / 4.0f},
			{2.0f / 8.0f,1.0f / 4.0f}
		},
			{
			{2.0f / 8.0f,0.0f},
			{3.0f / 8.0f,0.0},
			{2.0f / 8.0f,1.0f / 4.0f},
			{1.0f / 8.0f,1.0f / 4.0f}
		},
		{
			{3.0f / 8.0f,0.0f},
			{4.0f / 8.0f,0.0},
			{3.0f / 8.0f,1.0f / 4.0f},
			{4.0f / 8.0f,1.0f / 4.0f}
		},
			{
			{4.0f / 8.0f,0.0f},
			{5.0f / 8.0f,0.0},
			{4.0f / 8.0f,1.0f / 4.0f},
			{5.0f / 8.0f,1.0f / 4.0f}
		},
			{
			{6.0f / 8.0f,0.0f},
			{7.0f / 8.0f,0.0},
			{6.0f / 8.0f,1.0f / 4.0f},
			{7.0f / 8.0f,1.0f / 4.0f}
		},
	};
	
	Fader.Init(1280, 1280, XMFLOAT4(1, 1, 1, a), uvfader);
	Fader.SetScale(1, 1, 1);
	Fader.SetPosition(640, 360, 1);
	FaderBuffer =& Fader;
	
	if (mState==NULL&&a==1) {
		// FaderFadeIn();
		nextflg = true;
		
	}
	
	if (NONE==FaderGetState()&&a==0) {
			 nextflg = true;
		
	}
	return nextflg;
}
void FaderDraw() {
	FaderBuffer->Draw();
	/*Fader.Draw();*/
};
void FaderExit()
{
	Fader.UnInit();
}
void FaderFadeIn()
{
	mState = FADEIN;
	a = 1.0f;
}

void FaderFadeOut()
{
	mState = FADEOUT;
	a = 0.0f;
}

int FaderGetState()
{
	return mState;
}
