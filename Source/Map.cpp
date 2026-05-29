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
		
		// 2. ワイヤーターゲット（マンション・ビル）
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(500.0f, 600.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(1200.0f, 600.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(1800.0f, 600.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building_Nolight.png", VGet(2600.0f, 600.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(3500.0f, 600.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(4000.0f, 600.0f, 0.0f));

		// 3. ゴール
		new Goal(VGet(4800.0f, 1000.0f, 0.0f), 150, 150);
	}
}
