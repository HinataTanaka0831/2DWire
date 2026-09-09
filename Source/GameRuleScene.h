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
	std::unique_ptr<Button> m_backButton = nullptr;
	const int DrawX = Utility::SCREEN_WIDTH / 2 - 150;
	const int BackY = 900;
	int m_moveLeftHandle = -1;
	int m_moveRightHandle = -1;
	int m_jumpHandle = -1;
};