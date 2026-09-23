#include "DxLib.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "Player.h"
#include "Collision.h"
#include "SceneManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "Utility.h"
#include "Loading.h"
#include <memory>
#include <cmath>

// 全システムから参照されるシングルトン的マネージャーの静的インスタンス
SceneManager* Master::m_sceneManager = new SceneManager();
SoundManager* Master::m_soundManager = new SoundManager();

// アプリケーションのエントリーポイントおよびメインループ制御
// 入力: hInstance, hPrevInstance, lpCmdLine, nCmdShow / 出力: 0(正常終了), -1(初期化失敗) / 副作用: ウィンドウ生成、DXライブラリ初期化・終了、全リソース破棄
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// デバッグ・操作性確保のためウィンドウモードで起動
	ChangeWindowMode(true);

	// ゲーム描画解像度を固定仕様（1920x1080）に設定
	SetGraphMode(Utility::ScreenWidth, Utility::ScreenHeight, 32);

	// DXライブラリ初期化失敗時はプロセスを終了し後続の不正アクセスを防ぐ
	if(DxLib_Init() == -1)
	{
		return -1;
	}

	// 起動時の初期化負荷を管理するためのローダー
	LoadingManager loader;
	loader.AddTask(std::make_unique<InitializeSoundManagerTask>());
	loader.AddTask(std::make_unique<InitializeSceneManagerTask>());
	loader.ExecuteAll();

	// 画面チラつき（ティアリング）防止のため描画先をバックバッファに指定
	SetDrawScreen(DX_SCREEN_BACK);

	// ウィンドウ破棄またはESCキー押下までフレーム処理を継続
	int animationCounter = 0;
	int textureCurrentNum = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		int time = GetNowCount();

		// 更新
		Master::m_sceneManager->Update();

		// 毎フレームのクリック・リリース状態を検知するため入力状態を更新
		InputManager::GetInstance().MouseUpdate();

		// 描画
		Master::m_sceneManager->Draw();
		

		if (Master::m_sceneManager->IsQuitRequest())
		{
			break;
		}

		ScreenFlip();

		// 秒間60フレーム（約16.6ms/frame）の動作速度を一定に保つための同期待機
		while (GetNowCount() - time < 17)
		{
		}

		// イテレーション中の削除による不整合を防ぐため、フレーム終了時に不要オブジェクトを一括安全削除
		Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();

		// 更新・描画完了後に安全に次フレーム用シーンへの切り替えを行う
		Master::m_sceneManager->ChangeSceneIfNeeded();
	}

	// 静的マネージャーのメモリ解放およびDXライブラリリソースの破棄
	Master::m_sceneManager->Finalize();
	delete Master::m_sceneManager;
	Master::m_soundManager->Finalize();
	delete Master::m_soundManager;

	DxLib_End();

	return 0;
}