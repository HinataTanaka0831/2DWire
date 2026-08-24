#pragma once

#include "Scene.h"

// 操作説明およびゲームルールを表示するシーンクラス
class GameRuleScene : public Scene
{
public:
	GameRuleScene();
	~GameRuleScene();

	void Initialize() override;

	// 決定キー入力検知によるタイトル画面への復帰
	// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
	void Update() override;

	// 操作キー説明ダイアログの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	void Finalize() override;
};