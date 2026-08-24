#pragma once

#include "Scene.h"

// 全ステージクリア時のリザルト画面を制御するクラス
class ResultScene : public Scene
{
public:
	ResultScene();
	~ResultScene();

	void Initialize() override;

	// 決定キー入力によるタイトル画面への復帰
	// 入力: なし / 出力: なし / 副作用: SCENE_TITLEへの遷移要求
	void Update() override;

	// クリア謝辞メッセージの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	void Finalize() override;
};