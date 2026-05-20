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

GameScene::GameScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{
}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{


	new Player( 
		VGet( (float)Utility::SCREEN_WIDTH / 9 , (float)Utility::SCREEN_HEIGHT / 2, 0.0f) 
	);

	// テスト用にワイヤーターゲットをいくつか空中に配置
	new WireTarget(VGet(300.0f, 100.0f, 0.0f));
	new WireTarget(VGet(500.0f, 80.0f, 0.0f));
	new WireTarget(VGet(700.0f, 120.0f, 0.0f));

		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/kusikatsu_gyu.png", 1, 1
		);
		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/torimomoniku.png", 2, 1
		);
		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
			"Resource/hamukatsu.png", 3, 1
		);
		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 140.0f, GetRand(380), 0.0f),
			"Resource/Rebakatsu.png", 4, 1
		);

		// 串揚げ塩
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/kusikatsu_ton.png", 1, 2
		);
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/tamanegi.png", 2, 2
		);
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
			"Resource/renkon.png", 3, 2
		);
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 150.0f, GetRand(380), 0.0f),
			"Resource/torisasami.png", 4, 2
		);
	
}

void GameScene::Update()
{


	 //敵情報を取得
	auto pEnemyList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);



	// 敵がいないのであればリザルト画面へ
	// .empty() ... 空っぽかどうか、という関数  	//	Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
	if (pEnemyList.empty())
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
	}


	// 基底クラスの更新処理を呼び出す
	Scene::Update();

}

void GameScene::Draw()
{

	// 背景の表示
	DrawGraph(0, 0, grHandle, false);

	// プレイヤーの情報を取得
	auto pObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);

	Player* pPlayer = dynamic_cast<Player*>(pObj);

	// プレイヤーのHPゲージの描画
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




