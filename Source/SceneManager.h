#pragma once
#include "TransitionEffectManager.h"

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

	// ゲームループを終了するフラグを設定する
	// 入力: なし / 出力: なし / 副作用: mbQuitRequestでゲームループを続けるかどうかを決める
	void RequestQuit();

public:
	bool IsQuitRequest() const { return m_quitRequest; }
	ResultType GetResultType() const { return m_resultType; }
	Scene* GetCurrentScene() const { return m_currentScene; }


private:
	SceneType m_sceneType;             // 現在のシーン種別
	SceneType m_nextSceneType;         // 次に遷移すべきシーン種別
	ResultType m_resultType;           // リザルト演出の種類（クリア/ゲームオーバー）
	Scene* m_currentScene;             // 現在のシーンインスタンス
	TransitionEffectManager& instance;

	int m_shakeOffsetX;               // シェイクオフセットX
	int m_shakeOffsetY;               // シェイクオフセットY
	int m_workScreenHandle;           // ワークスクリーンハンドル

	bool m_quitRequest;               // ゲームループを終了するかどうかのフラグ
};