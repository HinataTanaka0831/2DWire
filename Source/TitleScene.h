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
	virtual ~TitleScene();


	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() override;
	// 終了処理
	virtual void Finalize() override;


private:
	static const int Play_Y = 260;
	static const int PlayRule_Y = 310;
	int NowSelect3 = select_Play;
	int y = 0;

};
