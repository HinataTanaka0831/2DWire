#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

class Button;
class TitleDemo;

// タイトル画面のメニューUI選択、ワイヤー操作プレビューデモ、シーン遷移を制御するクラス
class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();

	// タイトル背景、ボタンUI、自動プレビューデモの初期化
	// 入力: なし / 出力: なし / 副作用: 画像ロード、Button/TitleDemoの生成
	void Initialize() override;

	// ボタン入力検知とゲーム本編/ルール画面への遷移制御
	// 入力: なし / 出力: なし / 副作用: シーン遷移要求、デモの進行
	void Update() override;

	// 背景、プレビューデモ、UIボタンの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// デモおよびリソースの解放
	// 入力: なし / 出力: なし / 副作用: mpTitleDemoの破棄
	void Finalize() override;

private:
	std::unique_ptr<Button> mpPlayButton = nullptr;
	std::unique_ptr<Button> mpPlayRuleButton = nullptr;
	std::unique_ptr<Button> mpQuitButton = nullptr;
	std::unique_ptr<TitleDemo> mpTitleDemo = nullptr;
	const int StringX = Utility::SCREEN_WIDTH / 2 - 150;
	const int PlayY = 650;
	const int PlayRuleY = 750;
	const int QuitY = 850;
	int mnBackGroundHandle = -1;
};