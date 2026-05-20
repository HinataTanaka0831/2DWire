#include "ResultScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"

ResultScene::ResultScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize()
{

}

void ResultScene::Update()
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

void ResultScene::Draw()
{
	// 背景の表示
	DrawGraph(0, 0, grHandle, false);
	// 文字列の表示
	DrawString(Utility::SCREEN_WIDTH / 2 - 100, Utility::SCREEN_HEIGHT / 2 , "Thank you for Playing", GetColor(255, 255, 255));

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

void ResultScene::Finalize()
{

}
