#pragma once

#include "Scene.h"
#include "Utility.h"

class Button;
class TitleDemo;

// タイトル画面のメニューUI選択、ワイヤー操作プレビューデモ、シーン遷移を制御するクラス
class TitleScene : public Scene
{
public:
	enum {
		select_Play,
		select_PlayRule,

		select_Now3,
	};

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
	Button* mpPlayButton;
	Button* mpPlayRuleButton;
	int NowSelect3 = select_Play;
	const int stringX = Utility::SCREEN_WIDTH / 2 - 150;
	const int PlayY = 600;
	const int PlayRuleY = 750;
	int mnBackGroundHandle;
	TitleDemo* mpTitleDemo;
};