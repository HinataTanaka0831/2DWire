#include "GameScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Player.h"
#include "Enemy.h"
#include "BossEnemy.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Bullet.h"
#include "WireTarget.h"
#include "Map.h"
#include <cmath>

float gCameraX = 0.0f;
float gCameraY = 0.0f;

GameScene::GameScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

GameScene::~GameScene()
{
	// 背景の削除
	DeleteGraph(mBgNightHandle);
}

void GameScene::Initialize()
{
	// 背景の読み込み
	mBgNightHandle = LoadGraph("Resource/bg_night.png");

	// マップの読み込み
	Map* map = new Map();
	map->LoadStage(1);
	delete map; // 初期化だけなので即破棄するか、メンバ変数に持つかは自由。ここでは配置だけ行う。

	// プレイヤーの生成
	mpPlayer = new Player( 
		"Resource/Player.png", VGet( (float)Utility::SCREEN_WIDTH / 9 , 1000.0f, 0.0f)
	);

}

void GameScene::Update()
{

	// プレイヤーの位置を取得してカメラを更新
	if (mpPlayer != nullptr)
	{
		float playerX = mpPlayer->GetPosition().x;
		float playerY = mpPlayer->GetPosition().y;
		
		// X軸カメラ追従
		float targetCameraX = playerX - Utility::SCREEN_WIDTH / 3.0f;
		float targetCameraX2 = playerX - Utility::SCREEN_WIDTH / 0.5f;
		if (targetCameraX > gCameraX) {
			gCameraX = targetCameraX; // 右スクロールの許容
		}
		if (targetCameraX2 < gCameraX)
		{
			gCameraX = targetCameraX; // 左スクロールも許容
		}

		// Y軸カメラ追従: プレイヤーが画面上半部に入ったら上に追従
		float screenCenterY = Utility::SCREEN_HEIGHT / 1.5f;
		float targetCameraY = playerY - screenCenterY;
		// 地面以下にはカメラを下げない（地面の見える高さに固定）
		if (gCameraY > Utility::SCREEN_HEIGHT) gCameraY = Utility::SCREEN_HEIGHT;
		// スムーズに追従
		gCameraY += (targetCameraY - gCameraY) * 0.1f;


		// ゴール判定
		auto goalList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Goal2D);
		for (auto* goal : goalList)
		{
			float dx = mpPlayer->GetPosition().x - goal->GetPosition().x;
			float dy = mpPlayer->GetPosition().y - goal->GetPosition().y;
			float dist = std::sqrt(dx*dx + dy*dy);
			// 少し余裕を持たせて判定
			if (dist < goal->GetSizeX() / 2.0f + 50.0f)
			{
				// リザルト画面へ
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
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
	GetGraphSize(mBgNightHandle, &bgWidth, &bgHeight);
	
	if (bgWidth > 0)
	{
		// カメラXに合わせて背景をスクロール
		float scrollSpeed = 0.5f;
		int offsetX = (int)(gCameraX * scrollSpeed) % bgWidth;
		
		// offsetXが負になる場合の対策
		if (offsetX < 0) offsetX += bgWidth;

		// gCameraY分下にシフトして描画（上へ飛ぶとスカイラインが触れます）
		int bgOffsetY = (int)gCameraY;

		// スクロールしたら背景をループさせて描画
		for (int x = -offsetX; x < Utility::SCREEN_WIDTH; x += bgWidth)
		{
			DrawGraph(x, -bgOffsetY, mBgNightHandle, TRUE);
		}
	}



	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

void GameScene::Finalize()
{

}




