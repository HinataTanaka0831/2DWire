#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "GameRuleScene.h"
#include "GameOverScene.h"
#include "Utility.h"
#include <cstdlib>

SceneManager::SceneManager()
	: mnSceneType(SCENE_TYPE::SCENE_NONE)
	, mnNextSceneType(SCENE_TYPE::SCENE_NONE)
	, mpCurrentScene(nullptr)
	, mTransitionType(TransitionType::NORMAL_BLACK)
	, mnShakeOffsetX(0)
	, mnShakeOffsetY(0)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	// 画面シェイクや全画面ポストエフェクト合成用の中間スクリーンバッファを生成
	mnWorkScreenHandle = MakeScreen(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, TRUE);
	mnNextSceneType = SCENE_TYPE::SCENE_TITLE;
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Update();
	}

	if (mbIsTransition)
	{
		UpdateTransition();
	}
}

void SceneManager::Draw()
{
	if (mpCurrentScene == nullptr) return;

	// シェイク演出を適用するため一旦ワークスクリーンへ全描画
	SetDrawScreen(mnWorkScreenHandle);
	ClearDrawScreen();

	mpCurrentScene->Draw();

	SetDrawScreen(DX_SCREEN_BACK);

	// シェイクオフセットを加算して本来のバックバッファへ転送
	DrawGraph(mnShakeOffsetX, mnShakeOffsetY, mnWorkScreenHandle, FALSE);

	if (mbIsTransition)
	{
		DrawTransitionEffects();
	}
}

void SceneManager::Finalize()
{
	if (mnWorkScreenHandle != -1)
	{
		DeleteGraph(mnWorkScreenHandle);
		mnWorkScreenHandle = -1;
	}
}

void SceneManager::ChangeSceneIfNeeded()
{
	if (mnSceneType == mnNextSceneType)
	{
		return;
	}

	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Finalize();
		delete mpCurrentScene;
		mpCurrentScene = nullptr;
	}

	mnSceneType = mnNextSceneType;

	switch (mnSceneType)
	{
	case SCENE_TYPE::SCENE_TITLE:
		mpCurrentScene = new TitleScene();
		break;
	case SCENE_TYPE::SCENE_GAME:
		mpCurrentScene = new GameScene();
		break;
	case SCENE_TYPE::SCENE_RESULT:
		mpCurrentScene = new ResultScene();
		break;
	case SCENE_TYPE::SCENE_GAME_RULE:
		mpCurrentScene = new GameRuleScene();
		break;
	case SCENE_TYPE::SCENE_GAMEOVER:
		mpCurrentScene = new GameOverScene();
		break;
	default:
		break;
	}

	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Initialize();
	}
}

void SceneManager::SetNextScene(SCENE_TYPE next)
{
	if (mnSceneType == next) return;

	mnNextSceneType = next;

	// シーンの演出意図に応じてトランジション種別を分岐
	if (next == SCENE_TYPE::SCENE_GAME)
	{
		mTransitionType = TransitionType::WHITE_FLASH;
	}
	else if (next == SCENE_TYPE::SCENE_GAMEOVER)
	{
		mTransitionType = TransitionType::RED_FLASH_SHAKE;
	}
	else
	{
		mTransitionType = TransitionType::NORMAL_BLACK;
	}

	mbIsTransition = true;
	mPhase = TransitionPhase::TRANS_FADEOUT;
	mnTransitionTimer = 0;
	mnShakeOffsetX = 0;
	mnShakeOffsetY = 0;
}

void SceneManager::UpdateTransition()
{
	mnTransitionTimer++;

	if (mTransitionType == TransitionType::RED_FLASH_SHAKE)
	{
		if (mPhase == TransitionPhase::TRANS_FADEOUT)
		{
			// 被弾・死亡の衝撃を表現するため時間経過とともに減衰するランダム振動を生成
			int currentTime = mnTransitionTimer;
			int limitTime = TRANSITION_TIME;
			int maxShakeAmount = 15;

			int shakeRange = maxShakeAmount * (limitTime - currentTime) / limitTime;
			if (shakeRange > 0)
			{
				mnShakeOffsetX = (rand() % (shakeRange * 2 + 1)) - shakeRange;
				mnShakeOffsetY = (rand() % (shakeRange * 2 + 1)) - shakeRange;
			}
			else
			{
				mnShakeOffsetX = 0;
				mnShakeOffsetY = 0;
			}
		}
		else
		{
			mnShakeOffsetX = 0;
			mnShakeOffsetY = 0;
		}
	}

	switch (mPhase)
	{
	case TransitionPhase::TRANS_FADEOUT:
		if (mnTransitionTimer >= TRANSITION_TIME)
		{
			ChangeSceneIfNeeded();
			mPhase = TransitionPhase::TRANS_FADEIN;
			mnTransitionTimer = 0;
		}
		break;

	case TransitionPhase::TRANS_FADEIN:
		if (mnTransitionTimer >= TRANSITION_TIME)
		{
			mbIsTransition = false;
			mPhase = TransitionPhase::TRANS_NONE;
			mnShakeOffsetX = 0;
			mnShakeOffsetY = 0;
		}
		break;
	}
}

void SceneManager::DrawTransitionEffects()
{
	int alpha = 0;
	if (mPhase == TransitionPhase::TRANS_FADEOUT)
	{
		alpha = (mnTransitionTimer * 255) / TRANSITION_TIME;
	}
	else if (mPhase == TransitionPhase::TRANS_FADEIN)
	{
		alpha = ((TRANSITION_TIME - mnTransitionTimer) * 255) / TRANSITION_TIME;
	}

	unsigned int color = GetColor(0, 0, 0);

	switch (mTransitionType)
	{
	case TransitionType::WHITE_FLASH:
		color = GetColor(255, 255, 255);
		break;
	case TransitionType::RED_FLASH_SHAKE:
		color = GetColor(255, 0, 0);
		break;
	case TransitionType::NORMAL_BLACK:
	default:
		color = GetColor(0, 0, 0);
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneManager::RequestQuit()
{
	mbQuitRequest = true;
}
