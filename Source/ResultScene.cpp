#include "ResultScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"

ResultScene::ResultScene()
	: Scene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

// 決定キー入力によるタイトル画面への復帰
// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
void ResultScene::Update()
{
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
	}
	Scene::Update();
}

// クリア謝辞メッセージの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void ResultScene::Draw()
{
	DrawString(Utility::SCREEN_WIDTH / 2 - 100, Utility::SCREEN_HEIGHT / 2, "Thank you for Playing", GetColor(255, 255, 255));
	Scene::Draw();
}

void ResultScene::Finalize()
{
}