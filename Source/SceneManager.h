#pragma once

#include "DxLib.h"

class Scene;

class SceneManager
{
public:
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,
		SCENE_TITLE,
		SCENE_GAME,
		SCENE_RESULT,
		SCENE_SELECT,
		SCENE_GAME_RULE,
		SCENE_GAMEOVER,
	};

	enum class TransitionPhase
	{
		TRANS_NONE,
		TRANS_FADEOUT, // フェードアウト（遷移開始～暗転/明転）
		TRANS_FADEIN   // フェードイン（切り替え後～操作可能まで）
	};

	// --- 【追加】遷移演出のタイプ ---
	enum class TransitionType
	{
		NORMAL_BLACK,   // 通常の黒フェード
		WHITE_FLASH,    // 白フラッシュ（GameSceneへの遷移用）
		RED_FLASH_SHAKE // 赤フラッシュ + シェイク（GameOverSceneへの遷移用）
	};

	TransitionPhase mPhase = TransitionPhase::TRANS_NONE;

public:
	SceneManager();
	~SceneManager();

	void Initialize();
	void Update();
	void Draw(); // ※ Draw内で演出処理を呼び出します
	void Finalize();

	void ChangeSceneIfNeeded();
	void SetNextScene(SCENE_TYPE next);
	void UpdateTransition();

	// --- 【追加】遷移演出の描画処理 ---
	void DrawTransitionEffects();

	Scene* GetCurrentScene() { return mpCurrentScene; }

private:
	SCENE_TYPE mnSceneType;
	SCENE_TYPE mnNextSceneType;
	Scene* mpCurrentScene;

	bool mIsTransition = false;
	int mTransitionTimer = 0;
	const int TRANSITION_TIME = 15; // 遷移にかけるフレーム数（約0.25秒）

	// --- 【追加】演出用の管理変数 ---
	TransitionType mTransitionType = TransitionType::NORMAL_BLACK;
	int mShakeOffsetX = 0;
	int mShakeOffsetY = 0;

	// --- 【追加】シェイク用の一時描画バッファのハンドル ---
	int mWorkScreenHandle = -1;
};