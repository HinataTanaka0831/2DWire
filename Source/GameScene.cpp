#include "GameScene.h"
#include "Player.h"
#include "Collision.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Utility.h"
#include "Stage.h"
#include "Loading.h"
#include "SoundManager.h"
#include "Goal.h"
#include <cmath>

float gCameraX = 0.0f;
float gCameraY = 0.0f;
int gCurrentStage = 1;

GameScene::GameScene()
	: Scene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	if (mnBackGroundHandle == -1)
	{
		mnBackGroundHandle = LoadGraph("Resource/BackGround/bg_night.png");
	}

	LoadingManager loader;
	loader.AddTask(std::make_unique<InitializeLoadStageData>());
	loader.ExecuteGameScene();
}

void GameScene::LoadStageData()
{
	mpStage = new Stage();
	mpStage->LoadStage(gCurrentStage);
	mStageInfo = mpStage->GetStageInfo();

	mpPlayer = new Player(
		"Resource/Player/anim_idle.png",
		VGet(mStageInfo.playerStartX, mStageInfo.playerStartY, 0.0f),
		3, 3, 1, 5, 1.0f, true
	);
}

void GameScene::Update()
{
	if (mpPlayer == nullptr)
	{
		return;
	}

		float playerX = mpPlayer->GetPosition().x;
		float playerY = mpPlayer->GetPosition().y;

		// プレイヤー進行方向の前方視界を確保するため画面左1/3位置にプレイヤーを配置
		float targetCameraX = playerX - Utility::SCREEN_WIDTH / 3.0f;
		gCameraX = targetCameraX;

		// 上空へのジャンプ時にスムーズにカメラを追従（地面以下への潜り込みは防止）
		float screenCenterY = Utility::SCREEN_HEIGHT / 1.5f;
		float targetCameraY = playerY - screenCenterY;
		if (gCameraY > Utility::SCREEN_HEIGHT)
		{
			gCameraY = (float)Utility::SCREEN_HEIGHT;
		}
		gCameraY += (targetCameraY - gCameraY) * 0.1f;

		// カメラおよびプレイヤーの移動をステージ境界内に制限
		if (gCameraX < mStageInfo.cameraMinX)
		{
			gCameraX = mStageInfo.cameraMinX;
		}
		if (gCameraX > mStageInfo.cameraMaxX)
		{
			gCameraX = mStageInfo.cameraMaxX;
		}

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

		if (mpPlayer->IsDead())
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAMEOVER);
		}

		// ゴール接触判定とステージ遷移/リザルト遷移のディスパッチ
		if (!mIsGoalReached)
		{
			auto goalList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Goal2D);

			for (auto* goal : goalList)
			{
				float dx = mpPlayer->GetPosition().x - goal->GetPosition().x;
				float dy = mpPlayer->GetPosition().y - goal->GetPosition().y;
				float dist = std::sqrt(dx * dx + dy * dy);

				Goal* pGoal = dynamic_cast<Goal*>(goal);

				if (pGoal == nullptr)
				{
					return;
				}

				if (dist < pGoal->GetWidth() / 2.0f + 50.0f)
				{
					// 多重遷移防止フラグを立ててステージ進行を分岐
					mIsGoalReached = true;
					if (gCurrentStage < MaxStage)
					{
						gCurrentStage++;
						Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
					}
					else
					{
						Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
					}
					break;
				}
			}
		}
	

	Scene::Update();
}

void GameScene::Draw()
{
	// 深度感を演出する空の縦グラデーション背景
	for (int y = 0; y < Utility::SCREEN_HEIGHT; y++)
	{
		int r = (int)(10 + 30.0f * (1.0f - (float)y / Utility::SCREEN_HEIGHT));
		int g = (int)(10 + 50.0f * (1.0f - (float)y / Utility::SCREEN_HEIGHT));
		int b = (int)(30 + 120.0f * (1.0f - (float)y / Utility::SCREEN_HEIGHT));
		DrawLine(0, y, Utility::SCREEN_WIDTH, y, GetColor(r, g, b));
	}

	// 視差（パララックス）効果を適用した都市遠景のループ描画
	int bgWidth, bgHeight;
	GetGraphSize(mnBackGroundHandle, &bgWidth, &bgHeight);
	
	if (bgWidth > 0)
	{
		float scrollSpeed = 0.5f;
		int offsetX = (int)(gCameraX * scrollSpeed) % bgWidth;
		if (offsetX < 0)
		{
			offsetX += bgWidth;
		}

		int bgOffsetY = (int)gCameraY;
		const int bgYOffset = 120;

		for (int x = -offsetX; x < Utility::SCREEN_WIDTH; x += bgWidth)
		{
			if (mnBackGroundHandle != -1)
			{

			}
			DrawGraph(x, -bgOffsetY + bgYOffset, mnBackGroundHandle, TRUE);
		}
	}

	Scene::Draw();
}

void GameScene::Finalize()
{
	if (mnBackGroundHandle != -1)
	{
		DeleteGraph(mnBackGroundHandle);
		mnBackGroundHandle = -1;
	}

	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2D();
	mpPlayer = nullptr;

	if (mpStage != nullptr)
	{
		delete mpStage;
		mpStage = nullptr;
	}
}