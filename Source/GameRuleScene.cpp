#include "DxLib.h"
#include "GameRuleScene.h"
#include "Utility.h"
#include "MouseManager.h"
#include "Master.h"
#include "Button.h"

GameRuleScene::GameRuleScene()
	: Scene()
{
}

GameRuleScene::~GameRuleScene()
{
}

void GameRuleScene::Initialize()
{
	if (mnMoveLeftHandle == -1)
	{
		mnMoveLeftHandle = LoadGraph("Resource/UI/ui_ruleMoveLeft.png");
	}

	if (mnMoveRightHandle == -1)
	{
		mnMoveRightHandle = LoadGraph("Resource/UI/ui_ruleMoveRight.png");
	}

	if (mnJumpHandle == -1)
	{
		mnJumpHandle = LoadGraph("Resource/UI/ui_ruleJump.png");
	}

	if (mpBackButton == nullptr)
	{
		mpBackButton = std::make_unique<Button>(StringX, BackY - 10, StringX + 250, BackY + 60, "戻る", GetColor(70, 70, 90), GetColor(80, 130, 255), fontSize20);
	}
}

void GameRuleScene::Update()
{
	if (mpBackButton)
	{
		mpBackButton->Update();

		if (mpBackButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
		}
	}
	Scene::Update();
}

void GameRuleScene::Draw()
{
	DrawBox(Utility::SCREEN_WIDTH / 2 - 850, 50, Utility::SCREEN_WIDTH / 2 + 850, 1000, GetColor(255, 255, 255), false);

	DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 400, 100, "～～ 操作説明 ～～", GetColor(255, 255, 255), fontSize90);

	DrawExtendGraph(480, 350, 480 + 210 / 2, 350 + 214 / 2, mnMoveLeftHandle, true);
	DrawExtendGraph(730, 350, 730 + 209 / 2, 350 + 214 / 2, mnMoveRightHandle, true);
	DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 500, 470, "左移動　　右移動", GetColor(255, 255, 255), fontSize50);

	DrawExtendGraph(510, 620, 510 + 614 / 2, 620 + 186 / 2, mnJumpHandle, true);
	DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 391, Utility::SCREEN_HEIGHT / 2 + 190, "ジャンプ", GetColor(255, 255, 255), fontSize50);

	if (mpBackButton)
	{
		mpBackButton->Draw();
	}

	Scene::Draw();
}

void GameRuleScene::Finalize()
{
	if (mnMoveLeftHandle != -1)
	{
		DeleteGraph(mnMoveLeftHandle);
		mnMoveLeftHandle = -1;
	}

	if (mnMoveRightHandle != -1)
	{
		DeleteGraph(mnMoveRightHandle);
		mnMoveRightHandle = -1;
	}

	if (mnJumpHandle != -1)
	{
		DeleteGraph(mnJumpHandle);
		mnJumpHandle = -1;
	}
}