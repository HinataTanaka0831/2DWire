#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

class Button;

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

private:
	std::unique_ptr<Button> mpTitleButton = nullptr;
	std::unique_ptr<Button> mpQuitButton = nullptr;
	const int StringX = Utility::SCREEN_WIDTH / 2 - 150;
	const int TitleY = 750;
	const int QuitY = 850;
};