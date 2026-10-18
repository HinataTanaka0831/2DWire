#pragma once

#include "Scene.h"
#include "Stage.h"

class Player;
class Stage;

// ゲームプレイ本編（ステージ構築、カメラ追従、プレイヤー・敵の相互作用、ゴール判定）を統括するシーンクラス
class GameScene : public Scene
{
public:
	GameScene();
	~GameScene() override;

	// 背景リソースロードおよびローダーを介したステージ初期化の実行
	// 入力: なし / 出力: なし / 副作用: ローディング画面実行、ステージ構築
	void Initialize() override;

	// ステージ地形・プレイヤー・敵のインスタンス化
	// 入力: なし / 出力: なし / 副作用: Player, Stageの生成
	void LoadData();

	// カメラのスムーズ追従、境界制限、ゴール接触クリア判定、プレイヤー死亡判定
	// 入力: なし / 出力: なし / 副作用: gCameraX/Yの更新、次シーン遷移要求
	void Update() override;

	// スカイライン・視差スクロール背景およびオブジェクト群の描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// ステージ・プレイヤー・背景リソースの解放
	// 入力: なし / 出力: なし / 副作用: 全2Dオブジェクトおよび背景ハンドルの破棄
	void Finalize() override;

private:
	Player* m_player;          // プレイヤーオブジェクトのポインタ
	Stage* m_stage;            // ステージオブジェクトのポインタ
	StageInfo m_stageInfo;     // ステージ情報構造体（プレイヤー初期位置、ゴール位置、カメラ制限座標など）
	int m_backGroundHandle;    // 視差スクロール背景のグラフィックハンドル
	bool m_IsGoalReached;       // ゴール到達フラグ（true: ゴールに接触済み、false: 未接触）
};