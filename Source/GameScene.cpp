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

float gCameraX = 0.0f;

GameScene::GameScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{
}

GameScene::~GameScene()
{
	DeleteGraph(mBgDayHandle);
	DeleteGraph(mBgEveningHandle);
	DeleteGraph(mBgNightHandle);
}

void GameScene::Initialize()
{
	// 背景の読み込み
	mBgDayHandle = LoadGraph("Resource/bg_day.png");
	mBgEveningHandle = LoadGraph("Resource/bg_evening.png");
	mBgNightHandle = LoadGraph("Resource/bg_night.png");

	new Player( 
		"Resource/Player.bmp", VGet( (float)Utility::SCREEN_WIDTH / 9 , (float)Utility::SCREEN_HEIGHT / 2, 0.0f)
	);

	// テスト用にワイヤーターゲットを配置
	new WireTarget("Resource/target_building_1.png", VGet(500.0f, 300.0f, 0.0f));
	new WireTarget("Resource/target_mansion_1.png", VGet(1200.0f, 200.0f, 0.0f));
	new WireTarget("Resource/target_building_1.png", VGet(1800.0f, 400.0f, 0.0f));
	new WireTarget("Resource/target_mansion_1.png", VGet(2600.0f, 250.0f, 0.0f));
	
}

void GameScene::Update()
{

	// プレイヤーの位置を取得してカメラを更新
	auto pObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pObj);
	if (pPlayer != nullptr)
	{
		float playerX = pPlayer->GetPosition().x;
		// プレイヤーが画面の中心（1920/2）を超えたらカメラを動かす
		float targetCameraX = playerX - Utility::SCREEN_WIDTH / 3.0f;
		float targetCameraX2 = playerX - Utility::SCREEN_WIDTH / 0.5f;
		if (targetCameraX > gCameraX) {
			gCameraX = targetCameraX; // 右スクロールの許容
		}
		if (targetCameraX2 < gCameraX)
		{
			gCameraX = targetCameraX; // 左スクロールも許容
		}
	}

	// 基底クラスの更新処理を呼び出す
	Scene::Update();

}

void GameScene::Draw()
{
	// 背景のループ描画
	int bgWidth, bgHeight;
	GetGraphSize(mBgEveningHandle, &bgWidth, &bgHeight);
	
	if (bgWidth > 0)
	{
		// カメラXに合わせて背景をスクロール
		// スクロール速度を遅くしてパララックス効果を出す
		float scrollSpeed = 0.5f;
		int offsetX = (int)(gCameraX * scrollSpeed) % bgWidth;
		
		// offsetXが負になる場合の対策
		if (offsetX < 0) offsetX += bgWidth;

		// 画面サイズをカバーするのに必要な回数だけループして描画
		for (int x = -offsetX; x < Utility::SCREEN_WIDTH; x += bgWidth)
		{
			DrawGraph(x, 0, mBgEveningHandle, false);
		}
	}


	// プレイヤーのHPゲージの描画
	auto pObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pObj);
	if (pPlayer != nullptr)
	{
		pPlayer->HPGaugeDraw();
	}


	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

void GameScene::Finalize()
{

}




