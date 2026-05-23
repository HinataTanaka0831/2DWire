#pragma once

#include "Scene.h"

class Player;

class GameScene : public Scene
{
public:
	// コンストラクタ
	GameScene();
	// デストラクタ
	virtual~GameScene();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() override;
	// 終了処理
	virtual void Finalize() override;


private:
	Player* mpPlayer;
	char font[32];
	
	// 背景画像用ハンドル
	int mBgNightHandle;
	int mBgNight_SkyHandle;
};
