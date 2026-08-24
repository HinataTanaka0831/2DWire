#pragma once

class Scene;

// 全体シーン遷移のディスパッチ・遅延切替・画面トランジション演出（フェード/シェイク）を統括するクラス
class SceneManager
{
public:
	// シーン種別定数
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,
		SCENE_TITLE,
		SCENE_GAME,
		SCENE_GAME_STAGE2,
		SCENE_RESULT,
		SCENE_GAME_RULE,
		SCENE_GAMEOVER,
	};

	// 遷移演出の種類
	enum class TransitionType
	{
		NORMAL_BLACK,      // 通常の黒フェードイン/アウト
		WHITE_FLASH,       // 白フラッシュ（ゲーム開始時）
		RED_FLASH_SHAKE,   // 赤フラッシュ＋画面シェイク（ゲームオーバー時）
	};

	// 遷移の進行フェーズ
	enum class TransitionPhase
	{
		TRANS_NONE,
		TRANS_FADEOUT,
		TRANS_FADEIN,
	};

public:
	SceneManager();
	~SceneManager();

	// 一時描画スクリーンバッファの生成と初期シーン（タイトル）の設定
	// 入力: なし / 出力: なし / 副作用: mWorkScreenHandleの生成
	void Initialize();

	// 現在シーンの更新および遷移演出タイマーの進行
	// 入力: なし / 出力: なし / 副作用: シーンUpdateおよび画面揺れ計算
	void Update();

	// オフスクリーンバッファ経由でのシーン描画とポストエフェクト（シェイク・フェード）合成
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw();

	// 一時バッファの解放
	// 入力: なし / 出力: なし / 副作用: DxLibグラフィックハンドルの削除
	void Finalize();

	// ループ安全なタイミングで現在シーンを破棄し次シーンを初期化
	// 入力: なし / 出力: なし / 副作用: mpCurrentSceneの破棄と再生成
	void ChangeSceneIfNeeded();

	// 次に遷移すべきシーンを設定しトランジション演出を開始
	// 入力: next(遷移先シーン種別) / 出力: なし / 副作用: 遷移演出フラグとタイマーの開始
	void SetNextScene(SCENE_TYPE next);

	// 遷移演出のフェード・シェイク進行計算
	// 入力: なし / 出力: なし / 副作用: mShakeOffsetX/Yおよびアルファ値の更新
	void UpdateTransition();

	// トランジション用マスク・フラッシュの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへのアルファ描画
	void DrawTransitionEffects();

	Scene* GetCurrentScene() { return mpCurrentScene; }

private:
	SCENE_TYPE mnSceneType;
	SCENE_TYPE mnNextSceneType;
	Scene* mpCurrentScene;

	bool mIsTransition = false;
	int mTransitionTimer = 0;
	const int TRANSITION_TIME = 15; // 遷移にかけるフレーム数（約0.25秒）
	TransitionPhase mPhase = TransitionPhase::TRANS_NONE;

	TransitionType mTransitionType = TransitionType::NORMAL_BLACK;
	int mShakeOffsetX = 0;
	int mShakeOffsetY = 0;
	int mWorkScreenHandle = -1;
};