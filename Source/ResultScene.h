#pragma once

#include "Scene.h"

class ResultScene : public Scene
{
public:
	// コンストラクタ
	ResultScene();
	// デストラクタ
	~ResultScene();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
    void Draw() override;
	// 終了処理
	void Finalize() override;


};
