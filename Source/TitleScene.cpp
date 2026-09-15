#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"
#include "TitleDemo.h"

TitleScene::TitleScene() 
	: Scene()
{

}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	if (m_backGroundHandle == -1)
	{
		m_backGroundHandle = LoadGraph("Resource/BackGround/bg_night.png");
	}

	if (m_playButton == nullptr)
	{
		m_playButton = std::make_unique<Button>(DrawX, PlayY - 10, DrawX + 250, PlayY + 60, " プレイ ", GetColor(70, 70, 90), GetColor(80, 130, 255), m_fontSize20);
	}

	if (m_playRuleButton == nullptr)
	{
		m_playRuleButton = std::make_unique<Button>(DrawX, PlayRuleY - 10, DrawX + 250, PlayRuleY + 60, "操作方法", GetColor(70, 70, 90), GetColor(80, 130, 255), m_fontSize20);
	}

	if (m_quitButton == nullptr)
	{
		m_quitButton = std::make_unique<Button>(DrawX, QuitY - 10, DrawX + 250, QuitY + 60, "終了", GetColor(70, 70, 90), GetColor(80, 130, 255), m_fontSize20);
	}

	// 前シーンのカメラオフセットがタイトル画面の描画に影響しないよう初期化
	g_cameraX = 0.0f;
	g_cameraY = 0.0f;

	if (m_titleDemo == nullptr)
	{
		m_titleDemo = std::make_unique<TitleDemo>();
	}
}

void TitleScene::Update()
{
	if (m_playButton)
	{
		m_playButton->Update();

		if (m_playButton->IsClick())
		{
			g_currentStage = 1;
			Master::m_soundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::m_sceneManager->SetNextScene(SceneManager::SCENE_GAME);
		}
	}

	if (m_playRuleButton)
	{
		m_playRuleButton->Update();

		if (m_playRuleButton->IsClick())
		{
			Master::m_soundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::m_sceneManager->SetNextScene(SceneManager::SCENE_GAME_RULE);
		}
	}

	if (m_quitButton)
	{
		m_quitButton->Update();

		if (m_quitButton->IsClick())
		{
			Master::m_soundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::m_sceneManager->RequestQuit();
		}
	}

	if (m_titleDemo)
	{
		m_titleDemo->Update();
	}

	Scene::Update();
}

void TitleScene::Draw()
{
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

		for (int x = -offsetX; x < Utility::ScreenWidth; x += bgWidth)
		{
			DrawGraph(x, -bgOffsetY, m_backGroundHandle, TRUE);
		}
	}

	// 背景とUIの間にデモを描画し、ボタンの視認性を最優先にする
	if (m_titleDemo)
	{
		m_titleDemo->Draw();
	}

	if (m_playButton)
	{
		m_playButton->Draw();
	}

	if (m_playRuleButton)
	{
		m_playRuleButton->Draw();
	}

	if (m_quitButton)
	{
		m_quitButton->Draw();
	}

	Scene::Draw();
}

void TitleScene::Finalize()
{
	if (m_backGroundHandle != -1)
	{
		DeleteGraph(m_backGroundHandle);
		m_backGroundHandle = -1;
	}
}