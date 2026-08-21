#include "DxLib.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "Player.h"
#include "Collision.h"
#include "SceneManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "MouseManager.h"
#include "Loading.h"
#include <memory>
#include <math.h>


/* @note リファレンス https://dxlib.xsrv.jp/dxfunc.html
*/

// Master クラスの静的メンバ変数定義
SceneManager* Master::mpSceneManager = new SceneManager();
SoundManager* Master::mpSoundManager = new SoundManager();
MouseManager gMouseManager; // グローバルなマウスマネージャーのインスタンス

/**
* @fn WinMain
* @brief Main関数
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 正常終了／-1 エラー
* @details Main関数
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(true);

	// 画面サイズの設定
	SetGraphMode(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, 32);

	// DXライブラリ初期化
	if(DxLib_Init() == -1)
	{
		return -1;
	}

	// ローディングマネージャーの作成
	LoadingManager loader;

	// タスクを追加
	// サウンドマネージャーの初期化
	loader.AddTask(std::make_unique<InitializeSoundManagerTask>());
	// シーンマネージャーの初期化
	loader.AddTask(std::make_unique<InitializeSceneManagerTask>());

	// ローディング実行
	loader.ExecuteAll();


	// 描画先画面を裏画面に設定する
	SetDrawScreen(DX_SCREEN_BACK);



	// ゲームのメインループ
	// ProcessMessage() == 0 -> ウィンドウの×ボタンを押されていないかどうか
	// CheckHitKey(KEY_INPUT_ESCAPE) == 0 -> エスケープキーが押されていないかどうか
	int animationCounter = 0;
	int textureCurrentNum = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面を初期化する
		ClearDrawScreen();

		int time = GetNowCount();

		 // 更新
		Master::mpSceneManager->Update();

		// 描画
		Master::mpSceneManager->Draw();
		
		// マウスの状態を更新する
		gMouseManager.MouseClick();

		// 裏画面の内容を表画面に映す
		ScreenFlip();

		// 17ミリ秒（秒間約60フレームだった場合の1フレーム当たりの経過時間）
		// 経過するまでここで待つ
		while (GetNowCount() - time < 17)
		{
			// 待つだけなのでここでは何も処理はしない
		}

		// 削除する必要のあるオブジェクトがあれば削除する
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();

		// ループする直前にシーン遷移チェックを入れておく
		Master::mpSceneManager->ChangeSceneIfNeeded();

	}

	

	// 終了処理
	//Finalize();
	Master::mpSceneManager->Finalize();
	delete Master::mpSceneManager;
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;

	// DXライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}


