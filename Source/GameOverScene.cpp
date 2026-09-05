#include "GameOverScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "MouseManager.h"
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
	if (mpTitleButton == nullptr)
	{
		mpTitleButton = std::make_unique<Button>(StringX, TitleY - 10, StringX + 250, TitleY + 60, "タイトルへ", GetColor(70, 70, 90), GetColor(80, 130, 255), fontSize20);
	}

	if (mpQuitButton == nullptr)
	{
		mpQuitButton = std::make_unique<Button>(StringX, QuitY - 10, StringX + 250, QuitY + 60, "終了", GetColor(70, 70, 90), GetColor(80, 130, 255), fontSize20);
	}
}

void GameOverScene::Update()
{
	if (mpTitleButton)
	{
		mpTitleButton->Update();

		if (mpTitleButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
		}
	}

	if (mpQuitButton)
	{
		mpQuitButton->Update();

		if (mpQuitButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->RequestQuit();
		}
	}
	Scene::Update();
}

void GameOverScene::Draw()
{
	DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 150, Utility::SCREEN_HEIGHT / 2 - 140, "Game Over", GetColor(255, 255, 255), fontSize90);

	if (mpTitleButton)
	{
		mpTitleButton->Draw();
	}

	if (mpQuitButton)
	{
		mpQuitButton->Draw();
	}

	Scene::Draw();
}

void GameOverScene::Finalize()
{
}