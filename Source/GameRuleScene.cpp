#include "DxLib.h"
#include "GameRuleScene.h"
#include "Utility.h"
#include "InputManager.h"
#include "Master.h"

GameRuleScene::GameRuleScene()
	: Scene()
{
}

GameRuleScene::~GameRuleScene()
{
}

void GameRuleScene::Initialize()
{
}

// 決定キー入力検知によるタイトル画面への復帰
// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
void GameRuleScene::Update()
{
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
	}
	Scene::Update();
}

// 操作キー説明ダイアログの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void GameRuleScene::Draw()
{
	DrawBox(Utility::SCREEN_WIDTH / 2 - 200, 50, Utility::SCREEN_WIDTH / 2 + 200, 350, GetColor(255, 255, 255), false);
	DrawString(Utility::SCREEN_WIDTH / 2 - 80, Utility::SCREEN_HEIGHT / 2 - 170, "～～ 遊び方 ～～", GetColor(255, 255, 255));
	DrawString(Utility::SCREEN_WIDTH / 2 - 25, Utility::SCREEN_HEIGHT / 2 - 130, "W : 上", GetColor(255, 255, 255));
	DrawString(Utility::SCREEN_WIDTH / 2 - 130, Utility::SCREEN_HEIGHT / 2 - 90, "A : 左　　S: 下　　D: 右", GetColor(255, 255, 255));
	DrawString(Utility::SCREEN_WIDTH / 2 - 140, Utility::SCREEN_HEIGHT / 2 - 20, "SPACE : ジャンプ", GetColor(255, 255, 255));
	DrawString(Utility::SCREEN_WIDTH / 2 - 100, 300, "ENTERでタイトルへ戻る", GetColor(255, 0, 255));

	Scene::Draw();
}

void GameRuleScene::Finalize()
{
}