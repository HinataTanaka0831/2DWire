#pragma once

#include "DxLib.h"
#include "Scene.h"
#include "Utility.h"
#include <memory>

class Button;

// 操作説明およびゲームルールを表示するシーンクラス
class GameRuleScene : public Scene
{
public:
	GameRuleScene();
	~GameRuleScene();

	// ボタンUI、操作キー画像の初期化
    // 入力: なし / 出力: なし / 副作用: 画像ロード、Buttonの生成
	void Initialize() override;

	// ボタン入力検知によるタイトル画面への復帰
	// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
	void Update() override;

	// 操作キー説明ダイアログ、操作キー画像、UIボタンの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// リソースの解放
    // 入力: なし / 出力: なし / 副作用: 操作キー画像の削除
	void Finalize() override;

private:
	std::unique_ptr<Button> m_backButton;   // 戻るボタンのスマートポインタ
	const int DrawX;                        // 操作キー画像の描画X座標
	const int BackY;                        // 戻るボタンの描画Y座標
	int m_moveLeftHandle;                   // 左移動キー画像のハンドル
	int m_moveRightHandle;                  // 右移動キー画像のハンドル
	int m_jumpHandle;                       // ジャンプキー画像のハンドル
};