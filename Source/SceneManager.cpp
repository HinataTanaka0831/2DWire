#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "GameRuleScene.h"
#include "GameOverScene.h"
#include "Utility.h"
#include <cstdlib> // rand()用

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
	// 作成したスクリーンバッファを削除
	if (mWorkScreenHandle != -1)
	{
		DeleteGraph(mWorkScreenHandle);
	}
}

void SceneManager::Initialize()
{
	// --- 【追加】シェイクなどの演出用に、裏で描画するためのスクリーンを作成 (1920 * 1080) ---
	// ※ 第3引数をTRUEにすることでアルファチャンネル付きの画像バッファになります
	mWorkScreenHandle = MakeScreen(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, TRUE);
	mnNextSceneType = SCENE_TYPE::SCENE_TITLE;
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	// シーンの更新（遷移中も通常更新するかはゲームデザインによりますが、既存の構造を維持）
	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Update();
	}

	if (mIsTransition)
	{
		UpdateTransition();
	}
}

void SceneManager::Draw()
{
	if (mpCurrentScene == nullptr) return;
	// 1. 描画先を一時スクリーン(mWorkScreenHandle)に変更する
	SetDrawScreen(mWorkScreenHandle);
	ClearDrawScreen();

	// 2. 通常通りシーンを描画する（各シーンは通常通り(0,0)基準で描画してOK）
	mpCurrentScene->Draw();

	// 3. 描画先を本来の裏画面(DX_SCREEN_BACK)に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 4. 一時スクリーンの内容を、シェイクオフセットを加えて裏画面に描画する
	// ※ 画面外の黒い隙間が見えないように少し拡大して描画するか、そのまま等倍でずらして描画します。
	// ここではシンプルにそのまま描画します。
	DrawGraph(mShakeOffsetX, mShakeOffsetY, mWorkScreenHandle, FALSE);

	// 5. 遷移フェード/フラッシュ演出を描画（フラッシュ自体は画面の揺れに影響されないよう最後に描画）
	if (mIsTransition)
	{
		DrawTransitionEffects();
	}
}

void SceneManager::Finalize()
{
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

// --- 【修正】SetNextScene(): 遷移先シーンに応じて演出タイプを切り替える ---
void SceneManager::SetNextScene(SCENE_TYPE next)
{
	if (mnSceneType == next) return;

	mnNextSceneType = next;

	// 遷移演出タイプの判定
	if (next == SCENE_TYPE::SCENE_GAME)
	{
		mTransitionType = TransitionType::WHITE_FLASH;     // GameSceneへは白フラッシュ
	}
	else if (next == SCENE_TYPE::SCENE_GAMEOVER)
	{
		mTransitionType = TransitionType::RED_FLASH_SHAKE; // GameOverSceneへは赤フラッシュ + シェイク
	}
	else
	{
		mTransitionType = TransitionType::NORMAL_BLACK;    // その他は通常の黒フェード
	}

	mIsTransition = true;
	mPhase = TransitionPhase::TRANS_FADEOUT;
	mTransitionTimer = 0;
	mShakeOffsetX = 0;
	mShakeOffsetY = 0;
}

// --- 【修正】UpdateTransition(): 遷移状態の更新とシェイク処理の追加 ---
void SceneManager::UpdateTransition()
{
	mTransitionTimer++;

	// シェイク処理（赤フラッシュ+シェイク時のみ）
	if (mTransitionType == TransitionType::RED_FLASH_SHAKE)
	{
		if (mPhase == TransitionPhase::TRANS_FADEOUT)
		{
			// フェードアウト（ダメージ・ゲームオーバー発生直後）に激しくシェイク
			// タイマーが進むにつれてシェイクの強さを減衰させる
			int currentTime = mTransitionTimer;
			int limitTime = TRANSITION_TIME;
			int maxShakeAmount = 15; // 最大シェイク幅（ピクセル）

			// 残り時間に応じたシェイク量の減衰計算
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
			// フェードイン時は揺らさない
			mShakeOffsetX = 0;
			mShakeOffsetY = 0;
		}
	}

	switch (mPhase)
	{
	case TransitionPhase::TRANS_FADEOUT:
		if (mTransitionTimer >= TRANSITION_TIME)
		{
			// フェードアウト完了時にシーン切り替えを行う
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

// --- 【新規追加】DrawTransitionEffects(): フラッシュおよびフェード描画 ---
void SceneManager::DrawTransitionEffects()
{
	// 遷移タイマー比率から透明度(アルファ値)を算出 (0～255)
	int alpha = 0;
	if (mPhase == TransitionPhase::TRANS_FADEOUT)
	{
		alpha = (mTransitionTimer * 255) / TRANSITION_TIME;
	}
	else if (mPhase == TransitionPhase::TRANS_FADEIN)
	{
		alpha = ((TRANSITION_TIME - mTransitionTimer) * 255) / TRANSITION_TIME;
	}

	// 演出タイプに応じた色を決定
	unsigned int color = GetColor(0, 0, 0); // デフォルトは黒

	switch (mTransitionType)
	{
	case TransitionType::WHITE_FLASH:
		color = GetColor(255, 255, 255); // 白フラッシュ
		break;

	case TransitionType::RED_FLASH_SHAKE:
		color = GetColor(255, 0, 0);     // 赤フラッシュ
		break;

	case TransitionType::NORMAL_BLACK:
	default:
		color = GetColor(0, 0, 0);       // 黒フェード
		break;
	}

	// アルファブレンドを有効にして画面全体に短形を描画 (解像度: 1280x720)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}