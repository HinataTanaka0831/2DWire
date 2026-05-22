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
	DeleteGraph(mBgNightHandle);
	DeleteGraph(mBgNight_SkyHandle);
}

void GameScene::Initialize()
{
	// 背景の読み込み
	mBgNightHandle = LoadGraph("Resource/bg_night.png");
	mBgNight_SkyHandle = LoadGraph("Resource/bg_night_sky.png");

	new Player( 
		"Resource/Player.bmp", VGet( (float)Utility::SCREEN_WIDTH / 9 , 950.0f, 0.0f)
	);

	// マップの読み込み
	Map* map = new Map();
	map->LoadStage(1);
	delete map; // 初期化だけなので即破棄するか、メンバ変数に持つかは自由。ここでは配置だけ行う。
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


		// ゴール判定
		auto goalList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Goal2D);
		for (auto* goal : goalList)
		{
			float dx = pPlayer->GetPosition().x - goal->GetPosition().x;
			float dy = pPlayer->GetPosition().y - goal->GetPosition().y;
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
	// 背景のループ描画
	int bgWidth, bgHeight;
	GetGraphSize(mBgNightHandle, &bgWidth, &bgHeight);
	
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
			DrawGraph(x, 0, mBgNightHandle, false);
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




