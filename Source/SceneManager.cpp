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
	, mShakeOffsetX(0)
	, mShakeOffsetY(0)
{
}

SceneManager::~SceneManager()
{
}

// 一時描画スクリーンバッファの生成と初期シーン（タイトル）の設定
// 入力: なし / 出力: なし / 副作用: mWorkScreenHandleの生成
void SceneManager::Initialize()
{
	// 画面シェイクや全画面ポストエフェクト合成用の中間スクリーンバッファを生成
	mWorkScreenHandle = MakeScreen(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, TRUE);
	mnNextSceneType = SCENE_TYPE::SCENE_TITLE;
	ChangeSceneIfNeeded();
}

// 現在シーンの更新および遷移演出タイマーの進行
// 入力: なし / 出力: なし / 副作用: シーンUpdateおよび画面揺れ計算
void SceneManager::Update()
{
	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Update();
	}

	if (mIsTransition)
	{
		UpdateTransition();
	}
}

// オフスクリーンバッファ経由でのシーン描画とポストエフェクト（シェイク・フェード）合成
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void SceneManager::Draw()
{
	if (mpCurrentScene == nullptr) return;

	// シェイク演出を適用するため一旦ワークスクリーンへ全描画
	SetDrawScreen(mWorkScreenHandle);
	ClearDrawScreen();

	mpCurrentScene->Draw();

	SetDrawScreen(DX_SCREEN_BACK);

	// シェイクオフセットを加算して本来のバックバッファへ転送
	DrawGraph(mShakeOffsetX, mShakeOffsetY, mWorkScreenHandle, FALSE);

	if (mIsTransition)
	{
		DrawTransitionEffects();
	}
}

// 一時バッファの解放
// 入力: なし / 出力: なし / 副作用: DxLibグラフィックハンドルの削除
void SceneManager::Finalize()
{
	if (mWorkScreenHandle != -1)
	{
		DeleteGraph(mWorkScreenHandle);
		mWorkScreenHandle = -1;
	}
}

// ループ安全なタイミングで現在シーンを破棄し次シーンを初期化
// 入力: なし / 出力: なし / 副作用: mpCurrentSceneの破棄と再生成
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
	case SCENE_TYPE::SCENE_GAME_STAGE2:
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

// 次に遷移すべきシーンを設定しトランジション演出を開始
// 入力: next(遷移先シーン種別) / 出力: なし / 副作用: 遷移演出フラグとタイマーの開始
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

	mIsTransition = true;
	mPhase = TransitionPhase::TRANS_FADEOUT;
	mTransitionTimer = 0;
	mShakeOffsetX = 0;
	mShakeOffsetY = 0;
}

// 遷移演出のフェード・シェイク進行計算
// 入力: なし / 出力: なし / 副作用: mShakeOffsetX/Yおよびアルファ値の更新
void SceneManager::UpdateTransition()
{
	mTransitionTimer++;

	if (mTransitionType == TransitionType::RED_FLASH_SHAKE)
	{
		if (mPhase == TransitionPhase::TRANS_FADEOUT)
		{
			// 被弾・死亡の衝撃を表現するため時間経過とともに減衰するランダム振動を生成
			int currentTime = mTransitionTimer;
			int limitTime = TRANSITION_TIME;
			int maxShakeAmount = 15;

			int shakeRange = maxShakeAmount * (limitTime - currentTime) / limitTime;
			if (shakeRange > 0)
			{
				mShakeOffsetX = (rand() % (shakeRange * 2 + 1)) - shakeRange;
				mShakeOffsetY = (rand() % (shakeRange * 2 + 1)) - shakeRange;
			}
			else
			{
				mShakeOffsetX = 0;
				mShakeOffsetY = 0;
			}
		}
		else
		{
			mShakeOffsetX = 0;
			mShakeOffsetY = 0;
		}
	}

	switch (mPhase)
	{
	case TransitionPhase::TRANS_FADEOUT:
		if (mTransitionTimer >= TRANSITION_TIME)
		{
			ChangeSceneIfNeeded();
			mPhase = TransitionPhase::TRANS_FADEIN;
			mTransitionTimer = 0;
		}
		break;

	case TransitionPhase::TRANS_FADEIN:
		if (mTransitionTimer >= TRANSITION_TIME)
		{
			mIsTransition = false;
			mPhase = TransitionPhase::TRANS_NONE;
			mShakeOffsetX = 0;
			mShakeOffsetY = 0;
		}
		break;
	}
}

// トランジション用マスク・フラッシュの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへのアルファ描画
void SceneManager::DrawTransitionEffects()
{
	int alpha = 0;
	if (mPhase == TransitionPhase::TRANS_FADEOUT)
	{
		alpha = (mTransitionTimer * 255) / TRANSITION_TIME;
	}
	else if (mPhase == TransitionPhase::TRANS_FADEIN)
	{
		alpha = ((TRANSITION_TIME - mTransitionTimer) * 255) / TRANSITION_TIME;
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