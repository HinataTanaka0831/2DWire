#include "GameScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Scene.h"
#include <cmath>
#include "Loading.h"
#include <memory>


float gCameraX = 0.0f;
float gCameraY = 0.0f;
int gCurrentStage = 1;

GameScene::GameScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
	, mBg_NightHandle(-1)
	, mpPlayer(nullptr)
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	// ローディングマネージャーの作成
	// ここでタスクとして、ステージデータの読み込み（ローディング）を行う
	LoadingManager loader;
	loader.AddTask(std::make_unique<InitializeLoadStageData>());
	loader.ExecuteAll();
}

void GameScene::LoadStageData()
{
	// 背景の読み込み
	if (mBg_NightHandle == -1)
	{
		mBg_NightHandle = LoadGraph("Resource/BackGround/bg_night.png");
	}

	// カメラを初期位置にリセット
	gCameraX = 0.0f;
	gCameraY = 0.0f;

	// gCurrentStage を使用してステージを読み込む
	mpMap = new Map();
	mpMap->LoadStage(gCurrentStage);
	mStageInfo = mpMap->GetStageInfo();

	// ステージ設定からプレイヤーを生成
	mpPlayer = new Player("Resource/Player/Anim_Player_Idle.png", VGet(mStageInfo.playerStartX, mStageInfo.playerStartY, 0.0f), 3, 3, 1, 20, 1.0f, true);

}

void GameScene::Update()
{

	if (mpPlayer == nullptr)
	{
		return;
	}

	// プレイヤーの位置を取得してカメラを更新
		float playerX = mpPlayer->GetPosition().x;
		float playerY = mpPlayer->GetPosition().y;
		
		// X軸カメラ追従
		// 右スクロールと左スクロールの許容
		float targetCameraX = playerX - Utility::SCREEN_WIDTH / 3.0f;

		if (targetCameraX > gCameraX) 
		{
			gCameraX = targetCameraX; 
		}
		else if (targetCameraX < gCameraX)
		{
			gCameraX = targetCameraX;
		}


		// Y軸カメラ追従: プレイヤーが画面上半部に入ったら上に追従
		float screenCenterY = Utility::SCREEN_HEIGHT / 1.5f;
		float targetCameraY = playerY - screenCenterY;
		// 地面以下にはカメラを下げない（地面の見える高さに固定）
		if (gCameraY > Utility::SCREEN_HEIGHT)
		{
			gCameraY = Utility::SCREEN_HEIGHT;
		}

		// スムーズに追従
		gCameraY += (targetCameraY - gCameraY) * 0.1f;


		// カメラの左右移動をステージ範囲内に制限
		// 左端やゴールより先にはいかないようにする
		if (gCameraX < mStageInfo.cameraMinX)
		{
			gCameraX = mStageInfo.cameraMinX;
		}
		if (gCameraX > mStageInfo.cameraMaxX)
		{
			gCameraX = mStageInfo.cameraMaxX;
		}

		// プレイヤーの左右移動も制限
		if (playerX < mStageInfo.playerMinX)
		{
			VECTOR pos = mpPlayer->GetPosition();
			pos.x = mStageInfo.playerMinX;
			mpPlayer->SetPosition(pos);
		}
		if (playerX > mStageInfo.playerMaxX)
		{
			VECTOR pos = mpPlayer->GetPosition();
			pos.x = mStageInfo.playerMaxX;
			mpPlayer->SetPosition(pos);
		}


		// ゴール判定
		if (!mIsGoalReached)
		{
			auto goalList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Goal2D);

			for (auto* goal : goalList)
			{
				float dx = mpPlayer->GetPosition().x - goal->GetPosition().x;
				float dy = mpPlayer->GetPosition().y - goal->GetPosition().y;
				float dist = std::sqrt(dx*dx + dy*dy);
				// 少し余裕を持たせて判定
				if (dist < goal->GetSizeX() / 2.0f + 50.0f)
				{
					// 連続で何度も遷移しないようにして、まだ次のステージがあるなら次のステージに行き、最終ステージならばリザルト画面へ遷移する
					mIsGoalReached = true;
					if (gCurrentStage < MaxStage)
					{
						gCurrentStage++;
						Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME_STAGE2);
					}
					else
					{
						Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
					}

					break;
				}
			}
		}

	

	// 基底クラスの更新処理を呼び出す
	Scene::Update();

}

void GameScene::Draw()
{
	//1. 空のグラデーションを最初に全画面描画（上が高いほど淡い青　1080px分）
	for (int y = 0; y < Utility::SCREEN_HEIGHT; y++)
	{
		// 上から下へ: 混天 -> 夜空
		int r = (int)(10 + 30.0f * (1.0f - (float)y / Utility::SCREEN_HEIGHT));
		int g = (int)(10 + 50.0f * (1.0f - (float)y / Utility::SCREEN_HEIGHT));
		int b = (int)(30 + 120.0f * (1.0f - (float)y / Utility::SCREEN_HEIGHT));
		DrawLine(0, y, Utility::SCREEN_WIDTH, y, GetColor(r, g, b));
	}

	// 2. 都市背景をgCameraY分下にシフトして描画
	int bgWidth, bgHeight;
	GetGraphSize(mBg_NightHandle, &bgWidth, &bgHeight);
	
	if (bgWidth > 0)
	{
		// カメラXに合わせて背景をスクロール
		float scrollSpeed = 0.5f;
		int offsetX = (int)(gCameraX * scrollSpeed) % bgWidth;
		
		// offsetXが負になる場合の対策
		if (offsetX < 0)
		{
			offsetX += bgWidth;
		}

		// gCameraY分下にシフトして描画（上へ飛ぶとスカイラインが触れます）
		int bgOffsetY = (int)gCameraY;

		const int Bg_Y_Offset = 120;

		// スクロールしたら背景をループさせて描画
		for (int x = -offsetX; x < Utility::SCREEN_WIDTH; x += bgWidth)
		{
			DrawGraph(x, -bgOffsetY + Bg_Y_Offset, mBg_NightHandle, TRUE);
		}
	}



	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

void GameScene::Finalize()
{
	// 背景の削除
	if (mBg_NightHandle != -1)
	{
		DeleteGraph(mBg_NightHandle);
		mBg_NightHandle = -1;
	}

	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2D();

	mpPlayer = nullptr;

	// Map の解放
	if (mpMap != nullptr)
	{
		delete mpMap;
		mpMap = nullptr;
	}

}
