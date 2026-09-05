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
{

}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	if (mnBackGroundHandle == -1)
	{
		mnBackGroundHandle = LoadGraph("Resource/BackGround/bg_night.png");
	}

	if (mpPlayButton == nullptr)
	{
		mpPlayButton = std::make_unique<Button>(StringX, PlayY - 10, StringX + 250, PlayY + 60, " プレイ ", GetColor(70, 70, 90), GetColor(80, 130, 255), fontSize20);
	}

	if (mpPlayRuleButton == nullptr)
	{
		mpPlayRuleButton = std::make_unique<Button>(StringX, PlayRuleY - 10, StringX + 250, PlayRuleY + 60, "操作方法", GetColor(70, 70, 90), GetColor(80, 130, 255), fontSize20);
	}

	if (mpQuitButton == nullptr)
	{
		mpQuitButton = std::make_unique<Button>(StringX, QuitY - 10, StringX + 250, QuitY + 60, "終了", GetColor(70, 70, 90), GetColor(80, 130, 255), fontSize20);
	}

	// 前シーンのカメラオフセットがタイトル画面の描画に影響しないよう初期化
	gCameraX = 0.0f;
	gCameraY = 0.0f;

	if (mpTitleDemo == nullptr)
	{
		mpTitleDemo = std::make_unique<TitleDemo>();
	}
}

void TitleScene::Update()
{
	if (mpPlayButton)
	{
		mpPlayButton->Update();

		if (mpPlayButton->IsClick())
		{
			gCurrentStage = 1;
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME);
		}
	}

	if (mpPlayRuleButton)
	{
		mpPlayRuleButton->Update();

		if (mpPlayRuleButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME_RULE);
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

	if (mpTitleDemo)
	{
		mpTitleDemo->Update();
	}

	Scene::Update();
}

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

		for (int x = -offsetX; x < Utility::SCREEN_WIDTH; x += bgWidth)
		{
			DrawGraph(x, -bgOffsetY, mnBackGroundHandle, TRUE);
		}
	}

	// 背景とUIの間にデモを描画し、ボタンの視認性を最優先にする
	if (mpTitleDemo)
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

	if (mpQuitButton)
	{
		mpQuitButton->Draw();
	}

	Scene::Draw();
}

void TitleScene::Finalize()
{
	if (mnBackGroundHandle != -1)
	{
		DeleteGraph(mnBackGroundHandle);
		mnBackGroundHandle = -1;
	}
}