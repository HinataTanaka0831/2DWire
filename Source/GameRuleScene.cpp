#include "GameRuleScene.h"
#include "Utility.h"
#include "InputManager.h"
#include "Master.h"
#include "Button.h"

GameRuleScene::GameRuleScene()
	: Scene()
{
}

GameRuleScene::~GameRuleScene()
{
}

void GameRuleScene::Initialize()
{
	if (m_moveLeftHandle == -1)
	{
		m_moveLeftHandle = LoadGraph("Resource/UI/ui_ruleMoveLeft.png");
	}

	if (m_moveRightHandle == -1)
	{
		m_moveRightHandle = LoadGraph("Resource/UI/ui_ruleMoveRight.png");
	}

	if (m_jumpHandle == -1)
	{
		m_jumpHandle = LoadGraph("Resource/UI/ui_ruleJump.png");
	}

	if (m_backButton == nullptr)
	{
		m_backButton = std::make_unique<Button>(DrawX, BackY - 10, DrawX + 250, BackY + 60, "戻る", GetColor(70, 70, 90), GetColor(80, 130, 255), m_fontSize20);
	}
}

void GameRuleScene::Update()
{
	if (m_backButton)
	{
		m_backButton->Update();

		if (m_backButton->IsClick())
		{
			Master::m_soundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::m_sceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
		}
	}
	Scene::Update();
}

void GameRuleScene::Draw()
{
	DrawBox(Utility::ScreenWidth / 2 - 850, 50, Utility::ScreenWidth / 2 + 850, 1000, GetColor(255, 255, 255), false);

	DrawStringToHandle(Utility::ScreenWidth / 2 - 400, 100, "～～ 操作説明 ～～", GetColor(255, 255, 255), m_fontSize90);

	DrawExtendGraph(480, 350, 480 + 210 / 2, 350 + 214 / 2, m_moveLeftHandle, true);
	DrawExtendGraph(730, 350, 730 + 209 / 2, 350 + 214 / 2, m_moveRightHandle, true);
	DrawStringToHandle(Utility::ScreenWidth / 2 - 500, 470, "左移動　　右移動", GetColor(255, 255, 255), m_fontSize50);

	DrawExtendGraph(510, 620, 510 + 614 / 2, 620 + 186 / 2, m_jumpHandle, true);
	DrawStringToHandle(Utility::ScreenWidth / 2 - 391, Utility::ScreenWidth / 2 + 190, "ジャンプ", GetColor(255, 255, 255), m_fontSize50);

	if (m_backButton)
	{
		m_backButton->Draw();
	}

	Scene::Draw();
}

void GameRuleScene::Finalize()
{
	if (m_moveLeftHandle != -1)
	{
		DeleteGraph(m_moveLeftHandle);
		m_moveLeftHandle = -1;
	}

	if (m_moveRightHandle != -1)
	{
		DeleteGraph(m_moveRightHandle);
		m_moveRightHandle = -1;
	}

	if (m_jumpHandle != -1)
	{
		DeleteGraph(m_jumpHandle);
		m_jumpHandle = -1;
	}
}