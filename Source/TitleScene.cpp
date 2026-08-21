#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"
#include "MouseManager.h"



TitleScene::TitleScene() 
: Scene()     // 基底クラスのコンストラクタを呼び出す
, mnBackGroundHandle(-1)
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
	if (mnBackGroundHandle == -1)
	{
		mnBackGroundHandle = LoadGraph("Resource/BackGround/bg_night.png");
	}

	if (mpPlayButton == nullptr)
	{
		mpPlayButton = new Button(stringX, PlayY - 10, stringX + 250, PlayY + 60, "　プレイ　", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);
	}
	if (mpPlayRuleButton == nullptr)
	{
		mpPlayRuleButton = new Button(stringX, PlayRuleY - 10, stringX + 250, PlayRuleY + 60, "操作説明", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);
	}
	// BGM再生
	//Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

}

void TitleScene::Update()
{
	// SE再生
	Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

	// Sキーが押されたら下に下がる
	if (mpPlayButton)
	{
		mpPlayButton->Update();

		if (mpPlayButton->IsClick())
		{

			NowSelect3 = select_Play;
		}

	}
	// Wキーが押されたら上に上がる
	if (mpPlayRuleButton)
	{
		mpPlayRuleButton->Update();

		if (mpPlayRuleButton->IsClick())
		{
			NowSelect3 = select_PlayRule;
		}
	}

	bool isLeftTrigger = MouseManager::IsLeftTrigger();

	if (isLeftTrigger)
	{
		switch (NowSelect3)
		{
		case select_Play:  // プレイ画面へ
			gCurrentStage = 1;
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
	// 2. 都市背景をgCameraY分下にシフトして描画
	int bgWidth, bgHeight;
	GetGraphSize(mnBackGroundHandle, &bgWidth, &bgHeight);

	if (bgWidth > 0)
	{
		// カメラXに合わせて背景をスクロール
		float scrollSpeed = 0.5f;
		int offsetX = (int)(gCameraX * scrollSpeed) % bgWidth;

		// offsetXが負になる場合の対策
		if (offsetX < 0)
		{
			offsetX += bgWidth;
		}

		// gCameraY分下にシフトして描画（上へ飛ぶとスカイラインが触れます）
		int bgOffsetY = (int)gCameraY;

		const int Bg_Y_Offset = 0;

		// スクロールしたら背景をループさせて描画
		for (int x = -offsetX; x < Utility::SCREEN_WIDTH; x += bgWidth)
		{
			DrawGraph(x, -bgOffsetY + Bg_Y_Offset, mnBackGroundHandle, TRUE);
		}
	}

	// ボタンの表示
	if (mpPlayButton)
	{
		mpPlayButton->Draw();
	}

	if (mpPlayRuleButton)
	{
		mpPlayRuleButton->Draw();
	}


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
