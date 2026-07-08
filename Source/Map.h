#pragma once

// 1ステージ分の設定データ
struct StageInfo
{
	float playerStartX;
	float playerStartY;
	float goalX;
	float goalY;
	float cameraMinX;
	float cameraMaxX;
	float playerMinX;
	float playerMaxX;
};

class Map
{
public:
	// ?C???X?g???N?^
	Map();

	// ?f?X?g???N?^
	~Map();

	// ??????
	void Initialize();

	// ?X?e?[?W???????
	void LoadStage(int stageNum);

	// 読み込んだステージの設定を取得
	StageInfo GetStageInfo() const { return mStageInfo;}

private:
    StageInfo mStageInfo;
};
