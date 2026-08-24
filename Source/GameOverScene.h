#pragma once
#include "Scene.h"

// プレイヤーHPゼロ時のゲームオーバー画面を制御するクラス
class GameOverScene : public Scene
{
public:
	GameOverScene();
	~GameOverScene();

	void Initialize() override;

	// 決定キー入力によるタイトル画面への復帰
	// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
	void Update() override;

	// GAME OVER表示およびリトライ案内テキストの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	void Finalize() override;
};