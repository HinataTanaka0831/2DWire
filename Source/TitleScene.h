#pragma once

#include "Scene.h"
#include "Utility.h"

class Button;

class TitleScene : public Scene
{
public:
	enum{
		select_Play,
		select_PlayRule,

		select_Now3,
	};

public:
	// コンストラクタ
	TitleScene();
	// デストラクタ
	~TitleScene();


	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;


private:
	Button* mpPlayButton;
	Button* mpPlayRuleButton;
	int NowSelect3 = select_Play;
	const int stringX = Utility::SCREEN_WIDTH / 2 - 150;   // 文字列のX座標
	const int PlayY = 600;       // プレイボタンの高さ
    const int PlayRuleY = 750;   // 操作方法ボタンの高さ
	int mnBackGroundHandle;
};
