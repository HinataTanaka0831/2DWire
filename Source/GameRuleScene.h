#pragma once

#include "Scene.h"

class GameRuleScene : public Scene
{
public:
	// コンストラクタ
	GameRuleScene();
	// デストラクタ
	~GameRuleScene();
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;


};