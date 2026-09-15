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

float g_cameraX = 0.0f;
float g_cameraY = 0.0f;
int g_currentStage = 1;

GameScene::GameScene()
	: Scene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	if (m_backGroundHandle == -1)
	{
		m_backGroundHandle = LoadGraph("Resource/BackGround/bg_night.png");
	}

	LoadingManager loader;
	loader.AddTask(std::make_unique<InitializeLoadStageData>());
	loader.ExecuteAll();
}

void GameScene::LoadStageData()
{
	m_stage = new Stage();
	m_stage->LoadStage(g_currentStage);
	m_stageInfo = m_stage->GetStageInfo();

	m_player = new Player(
		"Resource/Player/player_idle.png",
		VGet(m_stageInfo.playerStartX, m_stageInfo.playerStartY, 0.0f),
		3, 3, 1, 8, 1.0f, true
	);
}

void GameScene::Update()
{
	if (m_player == nullptr)
	{
		return;
	}

		float playerX = m_player->GetPosition().x;
		float playerY = m_player->GetPosition().y;

		// プレイヤー進行方向の前方視界を確保するため画面左1/3位置にプレイヤーを配置
		float targetCameraX = playerX - Utility::ScreenWidth / 3.0f;
		g_cameraX = targetCameraX;

		// 上空へのジャンプ時にスムーズにカメラを追従（地面以下への潜り込みは防止）
		float screenCenterY = Utility::ScreenHeight / 1.5f;
		float targetCameraY = playerY - screenCenterY;
		if (g_cameraY > Utility::ScreenHeight)
		{
			g_cameraY = (float)Utility::ScreenHeight;
		}
		g_cameraY += (targetCameraY - g_cameraY) * 0.1f;

		// カメラおよびプレイヤーの移動をステージ境界内に制限
		if (g_cameraX < m_stageInfo.cameraMinX)
		{
			g_cameraX = m_stageInfo.cameraMinX;
		}
		if (g_cameraX > m_stageInfo.cameraMaxX)
		{
			g_cameraX = m_stageInfo.cameraMaxX;
		}

		if (playerX < m_stageInfo.playerMinX)
		{
			VECTOR pos = m_player->GetPosition();
			pos.x = m_stageInfo.playerMinX;
			m_player->SetPosition(pos);
		}
		if (playerX > m_stageInfo.playerMaxX)
		{
			VECTOR pos = m_player->GetPosition();
			pos.x = m_stageInfo.playerMaxX;
			m_player->SetPosition(pos);
		}

		if (m_player->IsDead())
		{
			Master::m_sceneManager->SetNextScene(SceneManager::SCENE_GAMEOVER);
		}

		// ゴール接触判定とステージ遷移/リザルト遷移のディスパッチ
		if (!mIsGoalReached)
		{
			auto goalList = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Goal2D);

			for (auto* goal : goalList)
			{
				float dx = m_player->GetPosition().x - goal->GetPosition().x;
				float dy = m_player->GetPosition().y - goal->GetPosition().y;
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
					if (g_currentStage < MaxStage)
					{
						g_currentStage++;
						Master::m_sceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
					}
					else
					{
						Master::m_sceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
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
	for (int y = 0; y < Utility::ScreenHeight; y++)
	{
		int r = (int)(10 + 30.0f * (1.0f - (float)y / Utility::ScreenHeight));
		int g = (int)(10 + 50.0f * (1.0f - (float)y / Utility::ScreenHeight));
		int b = (int)(30 + 120.0f * (1.0f - (float)y / Utility::ScreenHeight));
		DrawLine(0, y, Utility::ScreenWidth, y, GetColor(r, g, b));
	}

	// 視差（パララックス）効果を適用した都市遠景のループ描画
	int bgWidth, bgHeight;
	GetGraphSize(m_backGroundHandle, &bgWidth, &bgHeight);
	
	if (bgWidth > 0)
	{
		float scrollSpeed = 0.5f;
		int offsetX = (int)(g_cameraX * scrollSpeed) % bgWidth;
		if (offsetX < 0)
		{
			offsetX += bgWidth;
		}

		int bgOffsetY = (int)g_cameraY;
		const int bgYOffset = 120;

		for (int x = -offsetX; x < Utility::ScreenWidth; x += bgWidth)
		{
			if (m_backGroundHandle != -1)
			{

			}
			DrawGraph(x, -bgOffsetY + bgYOffset, m_backGroundHandle, TRUE);
		}
	}

	Scene::Draw();
}

void GameScene::Finalize()
{
	if (m_backGroundHandle != -1)
	{
		DeleteGraph(m_backGroundHandle);
		m_backGroundHandle = -1;
	}

	Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2D();
	m_player = nullptr;

	if (m_stage != nullptr)
	{
		delete m_stage;
		m_stage = nullptr;
	}
}