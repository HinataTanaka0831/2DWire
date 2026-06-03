#pragma once

class Map
{
public:
	// インストラクタ
	Map();

	// デストラクタ
	~Map();

	// 初期化
	void Initialize();

	// ステージの読み込み
	void LoadStage(int stageNum);
};
