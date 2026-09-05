#include "ResultScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "MouseManager.h"
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
	if (mpTitleButton == nullptr)
	{
		mpTitleButton = std::make_unique<Button>(StringX, TitleY - 10, StringX + 250, TitleY + 60, "タイトルへ", GetColor(70, 70, 90), GetColor(80, 130, 255), fontSize20);
	}

	if (mpQuitButton == nullptr)
	{
		mpQuitButton = std::make_unique<Button>(StringX, QuitY - 10, StringX + 250, QuitY + 60, "終了", GetColor(70, 70, 90), GetColor(80, 130, 255), fontSize20);
	}
}

// 決定キー入力によるタイトル画面への復帰
// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
void ResultScene::Update()
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

// クリア謝辞メッセージの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void ResultScene::Draw()
{
	DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 150, Utility::SCREEN_HEIGHT / 2 - 140, "Game Clear", GetColor(255, 255, 255), fontSize90);

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

void ResultScene::Finalize()
{
}