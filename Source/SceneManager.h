#pragma once

class Scene;

// 全体シーン遷移のディスパッチ・遅延切替・画面トランジション演出（フェード/シェイク）を統括するクラス
class SceneManager
{
public:
	// シーン種別定数
	enum SceneType
	{
		SceneNone = 0,  // 無効値
		SceneTitle,     // タイトル画面
		SceneGameStage1,      // ゲームプレイ本編ステージ1
		SceneGameStage2,      // ゲームプレイ本編ステージ2
		SceneResult,    // リザルト画面（クリア/ゲームオーバー）
		SceneGameRule,  // ゲームルール説明画面
	};

	// リザルト演出の種類
	enum class ResultType
	{
		ResultNone = 0,  // 無効値
		ResultClear,     // 全ステージクリア演出
		ResultGameOver,  // ゲームオーバー演出
	};


	// 遷移演出の種類
	enum class TransitionType
	{
		NormalBlack,      // 通常の黒フェードイン/アウト
		WhiteFlash,       // 白フラッシュ（ゲーム開始時）
		RedFlashShake,   // 赤フラッシュ＋画面シェイク（ゲームオーバー時）
	};

	// 遷移の進行フェーズ
	enum class TransitionPhase
	{
		TransNone,       // 遷移演出なし
		TransFadeOut,    // フェードアウト中
		TransFadeIn,     // フェードイン中
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
	void SetNextScene(SceneType next);

	// リザルト画面の演出種別を設定（クリア/ゲームオーバー）
	// 入力: result(リザルト種別) / 出力: なし / 副作用: mResultTypeの更新
	void SetResultType(ResultType result);

	// 遷移演出のフェード・シェイク進行計算
	// 入力: なし / 出力: なし / 副作用: mShakeOffsetX/Yおよびアルファ値の更新
	void UpdateTransition();

	// トランジション用マスク・フラッシュの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへのアルファ描画
	void DrawTransitionEffects();

	// ゲームループを終了するフラグを設定する
	// 入力: なし / 出力: なし / 副作用: mbQuitRequestでゲームループを続けるかどうかを決める
	void RequestQuit();


	bool IsQuitRequest() const { return m_quitRequest; }

	ResultType GetResultType() const { return m_resultType; }

	Scene* GetCurrentScene() { return m_currentScene; }


private:
	SceneType m_sceneType = SceneType::SceneNone;
	SceneType m_nextSceneType = SceneType::SceneNone;
	ResultType m_resultType = ResultType::ResultNone;
	Scene* m_currentScene = nullptr;

	bool m_isTransition = false;    // トランジション演出を行うかどうかのフラグ
	int m_transitionTimer = 0;      // トランジションを行う時間
	const int TransitionTime = 15; // 遷移にかけるフレーム数（約0.25秒）
	TransitionPhase m_phase = TransitionPhase::TransNone;

	TransitionType m_transitionType = TransitionType::NormalBlack;
	int m_shakeOffsetX = 0;
	int m_shakeOffsetY = 0;
	int m_workScreenHandle = -1;

	bool m_quitRequest = false;
};