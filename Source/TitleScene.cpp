#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"



TitleScene::TitleScene() 
: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

TitleScene::~TitleScene()
{
	
}

void TitleScene::Initialize()
{
	// タイトルロゴのクラスの作成
	// プレイヤーの生成
	// などをここで行う
	// ->タイトル画面で必要なオブジェクトをここで生成する

	// BGM再生
	//Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

}

void TitleScene::Update()
{
	// Sキーが押されたら下に下がる
	if (InputManager::CheckDownKey(KEY_INPUT_S))
	{
		// SE再生
		 Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		NowSelect3 = (NowSelect3 + 1) % select_Now3;
	}
	// Wキーが押されたら上に上がる
	if (InputManager::CheckDownKey(KEY_INPUT_W))
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		NowSelect3 = (NowSelect3 + (select_Now3 - 1)) % select_Now3;
	}
	// エンターキーが押されたら画面の切り替え処理
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		switch (NowSelect3)
		{
		case select_Play:  // プレイ画面へ
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME);
			break;

		case select_PlayRule:  // 遊び方へ
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME_RULE);
			break;
		}

	}

	// 基底クラスの更新処理を呼び出す
	Scene::Update();
}

void TitleScene::Draw()
{
	switch (NowSelect3)
	{
	case select_Play:  // 選択肢（プレイ）が選択されている場合はプレイのY座標を設定する
		y = Play_Y;
		break;

	case select_PlayRule:  // 選択肢（遊び方）が選択されている場合には遊び方のY座標を設定する
		y = PlayRule_Y;
		break;
	}


	// 画像の表示
	DrawGraph(Utility::SCREEN_WIDTH / 2 - 110, y, icHandle, true);


	// 基底クラスの描画処理を呼び出す
	Scene::Draw();
}

void TitleScene::Finalize()
{
	// BGM停止
	//Master::mpSoundManager->StopBGM();
}


		// SE再生
//Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
//
//Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_SELECT);
