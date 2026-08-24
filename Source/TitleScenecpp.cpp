#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"
#include "MouseManager.h"
#include "TitleDemo.h"

TitleScene::TitleScene() 
	: Scene()
	, mpPlayButton(nullptr)
	, mpPlayRuleButton(nullptr)
	, mnBackGroundHandle(-1)
	, mpTitleDemo(nullptr)
{
}

TitleScene::~TitleScene()
{
}

// タイトル背景、ボタンUI、自動プレビューデモの初期化
// 入力: なし / 出力: なし / 副作用: 画像ロード、Button/TitleDemoの生成
void TitleScene::Initialize()
{
	if (mnBackGroundHandle == -1)
	{
		mnBackGroundHandle = LoadGraph("Resource/BackGround/bg_night.png");
	}

	if (mpPlayButton == nullptr)
	{
		mpPlayButton = new Button(stringX, PlayY - 10, stringX + 250, PlayY + 60, " プレイ ", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);
	}
	if (mpPlayRuleButton == nullptr)
	{
		mpPlayRuleButton = new Button(stringX, PlayRuleY - 10, stringX + 250, PlayRuleY + 60, "あそびかた", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);
	}

	gCameraX = 0.0f;
	gCameraY = 0.0f;

	if (mpTitleDemo == nullptr)
	{
		mpTitleDemo = new TitleDemo();
	}
}

// ボタン入力検知とゲーム本編/ルール画面への遷移制御
// 入力: なし / 出力: なし / 副作用: シーン遷移要求、デモの進行
void TitleScene::Update()
{
	Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

	if (mpPlayButton)
	{
		mpPlayButton->Update();
		if (mpPlayButton->IsClick())
		{
			NowSelect3 = select_Play;
		}
	}

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
		case select_Play:
			gCurrentStage = 1;
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME);
			break;

		case select_PlayRule:
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME_RULE);
			break;
		}
	}

	if (mpTitleDemo != nullptr)
	{
		mpTitleDemo->Update();
	}

	Scene::Update();
}

// 背景、プレビューデモ、UIボタンの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void TitleScene::Draw()
{
	int bgWidth, bgHeight;
	GetGraphSize(mnBackGroundHandle, &bgWidth, &bgHeight);

	if (bgWidth > 0)
	{
		float scrollSpeed = 0.5f;
		int offsetX = (int)(gCameraX * scrollSpeed) % bgWidth;

		if (offsetX < 0)
		{
			offsetX += bgWidth;
		}

		int bgOffsetY = (int)gCameraY;
		const int Bg_Y_Offset = 0;

		for (int x = -offsetX; x < Utility::SCREEN_WIDTH; x += bgWidth)
		{
			DrawGraph(x, -bgOffsetY + Bg_Y_Offset, mnBackGroundHandle, TRUE);
		}
	}

	if (mpTitleDemo != nullptr)
	{
		mpTitleDemo->Draw();
	}

	if (mpPlayButton)
	{
		mpPlayButton->Draw();
	}

	if (mpPlayRuleButton)
	{
		mpPlayRuleButton->Draw();
	}

	Scene::Draw();
}

// デモおよびリソースの解放
// 入力: なし / 出力: なし / 副作用: mpTitleDemoの破棄
void TitleScene::Finalize()
{
	delete mpTitleDemo;
	mpTitleDemo = nullptr;
}