#include "DxLib.h"
#include "GameRuleScene.h"
#include "Utility.h"
#include "InputManager.h"
#include "Master.h"

GameRuleScene::GameRuleScene()
	:Scene()
{

}

GameRuleScene::~GameRuleScene()
{

}

void GameRuleScene::Initialize()
{

}

void GameRuleScene::Update()
{
	// エンターキーが押されたらタイトル画面に戻る
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
	}
	// 基底クラスの更新処理を呼び出す
	Scene::Update();
}

void GameRuleScene::Draw()
{
	// 四角形の表示
	DrawBox(Utility::SCREEN_WIDTH / 2 - 200, 50, Utility::SCREEN_WIDTH / 2 + 200, 350, GetColor(255, 255, 255), false);
	// 文字列の表示
	DrawString(Utility::SCREEN_WIDTH / 2 - 80, Utility::SCREEN_HEIGHT / 2 - 170, "～～ 遊び方 ～～", GetColor(255, 255, 255));

	DrawString(Utility::SCREEN_WIDTH / 2 - 25, Utility::SCREEN_HEIGHT / 2 - 130, "W : 上", GetColor(255, 255, 255));

	DrawString(Utility::SCREEN_WIDTH / 2 - 130, Utility::SCREEN_HEIGHT / 2 - 90, "A : 左　　S: 下　　D: 右", GetColor(255, 255, 255));

	DrawString(Utility::SCREEN_WIDTH / 2 - 140, Utility::SCREEN_HEIGHT / 2 - 20, "SPEACE : 弾を発射", GetColor(255, 255, 255));

	DrawString(Utility::SCREEN_WIDTH / 2 - 100, 300, "ENTERでタイトルへ戻る", GetColor(255, 0, 255));


	// 基底クラスの描画処理を呼び出す
	Scene::Draw();
}

void GameRuleScene::Finalize()
{

}

