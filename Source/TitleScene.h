#pragma once

#include "Scene.h"

class TitleScene : public Scene
{
public:
	enum {
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
	static const int Play_Y = 260;
	static const int PlayRule_Y = 310;
	int NowSelect3 = select_Play;
	int y = 0;

};
