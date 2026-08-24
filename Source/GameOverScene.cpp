#include "GameOverScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"

GameOverScene::GameOverScene()
	: Scene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
}

// 決定キー入力によるタイトル画面への復帰
// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
void GameOverScene::Update()
{
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
	}
	Scene::Update();
}

// GAME OVER表示およびリトライ案内テキストの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void GameOverScene::Draw()
{
	SetFontSize(50);
	DrawString(Utility::SCREEN_WIDTH / 2 - 270, Utility::SCREEN_HEIGHT / 2 - 140, "      GAME OVER", GetColor(255, 255, 255));

	SetFontSize(20);
	DrawString(Utility::SCREEN_WIDTH / 2 - 120, 600, "  ENTER: タイトルへ  ", GetColor(255, 255, 255));

	Scene::Draw();
}

void GameOverScene::Finalize()
{
}