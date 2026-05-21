#include "Map.h"
#include "WireTarget.h"
#include "Ground.h"
#include "Goal.h"
#include "DxLib.h"
#include "Utility.h"

Map::Map()
{
}

Map::~Map()
{
}

void Map::Initialize()
{
}

void Map::LoadStage(int stageNum)
{
	if (stageNum == 1)
	{
		// ====== ステージ1の配置 ======
		
		// 1. 地面（初期位置周辺と、ところどころに配置）
		// 最初は落ちないように長めの地面
		new Ground("Resource/Road.png", VGet(500.0f, 900.0f, 0.0f), 2000, 200);
		// 中間の足場（落ちたらゲームオーバーのリスク）
		new Ground("Resource/Road.png", VGet(2000.0f, 1000.0f, 0.0f), 600, 200);
		new Ground("Resource/Road.png", VGet(3000.0f, 1050.0f, 0.0f), 500, 200);
		// ゴール前の地面
		new Ground("Resource/Road.png", VGet(4500.0f, 950.0f, 0.0f), 1000, 200);

		// 2. ワイヤーターゲット（マンション・ビル）
		new WireTarget("Resource/target_building_1.png", VGet(500.0f, 300.0f, 0.0f));
		new WireTarget("Resource/target_mansion_1.png", VGet(1200.0f, 200.0f, 0.0f));
		new WireTarget("Resource/target_building_1.png", VGet(1800.0f, 400.0f, 0.0f));
		new WireTarget("Resource/target_mansion_1.png", VGet(2600.0f, 250.0f, 0.0f));
		new WireTarget("Resource/target_building_1.png", VGet(3500.0f, 350.0f, 0.0f));
		new WireTarget("Resource/target_mansion_1.png", VGet(4000.0f, 150.0f, 0.0f));

		// 3. ゴール
		new Goal(VGet(4800.0f, 800.0f, 0.0f), 150, 150);
	}
}
