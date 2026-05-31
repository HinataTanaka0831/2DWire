#include "GameOverScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"

GameOverScene::GameOverScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

GameOverScene::~GameOverScene()
{

}

void GameOverScene::Initialize()
{

}

void GameOverScene::Update()
{

	// エンターキーが押されたらゲーム画面へ移動
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);

	}




	// 基底クラスの更新処理を呼び出す
	Scene::Update();

}

void GameOverScene::Draw()
{
	// 背景の表示
	DrawGraph(0, 0, grHandle, false);
	// 四角形の表示
	DrawBox(Utility::SCREEN_WIDTH / 2 - 150, 50, Utility::SCREEN_WIDTH / 2 + 150, 200, GetColor(255, 255, 255), false);
	// 文字のフォントサイズ変更
	SetFontSize(50);
	// 文字列の表示
	DrawString(Utility::SCREEN_WIDTH / 2 - 270, Utility::SCREEN_HEIGHT / 2 - 140, "　　　GAME OVER", GetColor(255, 255, 255));


	SetFontSize(20);

	DrawString(Utility::SCREEN_WIDTH / 2 - 120, 300, "　ENTER: タイトルへ　", GetColor(255, 255, 255));




	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

void GameOverScene::Finalize()
{

}

