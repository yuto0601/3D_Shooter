#pragma once
#include "quad2d.h"

//中間とかに入るフェードイン

//タイトル
 void Title();
 void TitleUpdate();
 void TitleDraw();
 void TitleExit();
 //ゲーム
void PlayGameInit();
void PlayGameUpdate();
void PlayGameDraw();
void PlayGameExit();
//リザルト
 void ResultInit();
 void ResultUpdate();
 void ResultDraw();
 void ResultExit();
 // 画面定数
 enum class SCENENUM {
	 TITLE_INIT, TITLE,  // タイトル
	 GAME_INIT, GAME,    // ゲーム
	 RESULT_INIT, RESULT, // リザルト

	 FADE
 };
class Game
{

public: // クラスの公開要素

	// メンバー関数
	//static void Initialize();	// ゲームの初期化処理
	//static void Update();		// 更新処理
	//static void Draw();			// 描画処理
	//static void Release();		// 解放処理



	//// GameObject生成関数
	//static GameObject* CreateRoad(); // 道路
	//static GameObject* CreateShot(); // 弾
	//static GameObject* CreateExplosion(); // 爆発
	//static GameObject* CreateTitleLogo(); // タイトルロゴ
	//static GameObject* CreateFadePanel(); // 

	// このゲームに登場するゲームオブジェクト
	static std::vector<GameObject*> mGameObjects;

private: // クラスの非公開要素
public:
	// 弾管理用配列
	static std::vector<GameObject*> mShots;

	// エフェクト管理用配列
	static std::vector<GameObject*> mEffects;
	static std::vector<Quad2D*> g_quad2Ds;

    

	static int mNextScene; // 次のシーン
	static SCENENUM mCurrentScene;  // 現在のシーン（画面）

	static void SetNextScene(int next) {
		mNextScene = next;
		mCurrentScene = SCENENUM::FADE;
	}

	


};