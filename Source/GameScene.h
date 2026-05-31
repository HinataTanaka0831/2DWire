#pragma once

#include "Scene.h"

class Player;

class GameScene : public Scene
{
public:
	// コンストラクタ
	GameScene();
	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;


private:
	Player* mpPlayer;
	
	// 背景画像用ハンドル
	int mBgNightHandle;
};
