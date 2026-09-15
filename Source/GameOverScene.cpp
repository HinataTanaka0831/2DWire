#include "GameOverScene.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"


GameOverScene::GameOverScene()
	: Scene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	if (m_titleButton == nullptr)
	{
		m_titleButton = std::make_unique<Button>(DrawX, TitleY - 10, DrawX + 250, TitleY + 60, "タイトル", GetColor(70, 70, 90), GetColor(80, 130, 255), m_fontSize20);
	}

	if (m_quitButton == nullptr)
	{
		m_quitButton = std::make_unique<Button>(DrawX, QuitY - 10, DrawX + 250, QuitY + 60, "終了", GetColor(70, 70, 90), GetColor(80, 130, 255), m_fontSize20);
	}
}

void GameOverScene::Update()
{
	if (m_titleButton)
	{
		m_titleButton->Update();

		if (m_titleButton->IsClick())
		{
			Master::m_soundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::m_sceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
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
	Scene::Update();
}

void GameOverScene::Draw()
{
	DrawStringToHandle(Utility::ScreenWidth / 2 - 230, Utility::ScreenHeight / 2 - 140, "Game Over", GetColor(255, 255, 255), m_fontSize90);

	if (m_titleButton)
	{
		m_titleButton->Draw();
	}

	if (m_quitButton)
	{
		m_quitButton->Draw();
	}

	Scene::Draw();
}

void GameOverScene::Finalize()
{
}