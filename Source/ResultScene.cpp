#include "ResultScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"

ResultScene::ResultScene()
	: Scene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	if (m_retryButton == nullptr)
	{
		m_retryButton = std::make_unique<Button>(DrawX, TitleY - 10, DrawX + 250, TitleY + 60, "リトライ", GetColor(70, 70, 90), GetColor(80, 130, 255), m_fontSize20);
	}

	if (m_titleButton == nullptr)
	{
		m_titleButton = std::make_unique<Button>(DrawX, QuitY - 10, DrawX + 250, QuitY + 60, "タイトル", GetColor(70, 70, 90), GetColor(80, 130, 255), m_fontSize20);
	}
}

// 決定キー入力によるタイトル画面への復帰
// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
void ResultScene::Update()
{
	if (m_retryButton)
	{
		m_retryButton->Update();

		if (m_retryButton->IsClick())
		{
			g_currentStage = 1;
			Master::m_soundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::m_sceneManager->SetNextScene(SceneManager::SceneType::SceneGameStage1);
		}
	}

	if (m_titleButton)
	{
		m_titleButton->Update();

		if (m_titleButton->IsClick())
		{
			Master::m_soundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::m_sceneManager->SetNextScene(SceneManager::SceneType::SceneTitle);
		}
	}
	Scene::Update();
}

// クリア謝辞メッセージの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void ResultScene::Draw()
{
	if (Master::m_sceneManager->GetResultType() == SceneManager::ResultType::ResultClear)
	{
		DrawStringToHandle(Utility::ScreenWidth / 2 - 230, Utility::ScreenHeight / 2 - 140, "Game Clear", GetColor(255, 255, 255), m_fontSize90);
	}
	else if (Master::m_sceneManager->GetResultType() == SceneManager::ResultType::ResultGameOver)
	{
		DrawStringToHandle(Utility::ScreenWidth / 2 - 230, Utility::ScreenHeight / 2 - 140, "Game Over", GetColor(255, 255, 255), m_fontSize90);
	}

	if (m_retryButton)
	{
		m_retryButton->Draw();
	}

	if (m_titleButton)
	{
		m_titleButton->Draw();
	}

	Scene::Draw();
}

void ResultScene::Finalize()
{
}