#include "Stage.h"
#include "WireTarget.h"
#include "Ground.h"
#include "Goal.h"
#include "DxLib.h"
#include "Utility.h"
#include "Enemy.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
}

void Stage::LoadStage(int stageNum)
{
	if (stageNum == 1)
	{
		// ====== ステージ1の配置 ======
		mStageInfo.playerStartX = 300.0f;
		mStageInfo.playerStartY = 1000.0f;
		mStageInfo.goalX = 5600.0f;
		mStageInfo.goalY = 1000.0f;
		mStageInfo.cameraMinX = 0.0f;
		mStageInfo.cameraMaxX = 5600.0f - Utility::SCREEN_WIDTH / 1.0f;
		mStageInfo.playerMinX = 100.0f;
		mStageInfo.playerMaxX = 5600.0f;

		// 2. ワイヤーターゲット（マンション・ビル）
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(600.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(1300.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(2000.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building_Nolight.png", VGet(2750.0f, 830.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(3500.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(4200.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(4900.0f, 800.0f, 0.0f));

	    // 敵の生成 (プレイヤー進行ルートに沿って戦略的かつ段階的に配置)
        new Enemy("Resource/Enemy/character_monster01.png", VGet(1000.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
        new Enemy("Resource/Enemy/character_monster01.png", VGet(1800.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
        new Enemy("Resource/Enemy/character_monster01.png", VGet(2500.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
        new Enemy("Resource/Enemy/character_monster01.png", VGet(3300.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
        new Enemy("Resource/Enemy/character_monster01.png", VGet(4100.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
        new Enemy("Resource/Enemy/character_monster01.png", VGet(4800.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
        new Enemy("Resource/Enemy/character_monster01.png", VGet(5400.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);

		// 3. ゴール
		new Goal(VGet(mStageInfo.goalX, mStageInfo.goalY, 0.0f), 150, 150);
	}
	else if (stageNum == 2)
	{
		// ====== ステージ2の配置 ======
		mStageInfo.playerStartX = 300.0f;
		mStageInfo.playerStartY = 1000.0f;
		mStageInfo.goalX = 9000.0f;
		mStageInfo.goalY = 1000.0f;
		mStageInfo.cameraMinX = 0.0f;
		mStageInfo.cameraMaxX = 9000.0f - Utility::SCREEN_WIDTH / 1.0f;
		mStageInfo.playerMinX = 100.0f;
		mStageInfo.playerMaxX = 9000.0f;

		// 2. ワイヤーターゲット（マンション・ビル）
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(600.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(1300.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(2000.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building_Nolight.png", VGet(2750.0f, 830.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(3500.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(4200.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(4900.0f, 800.0f, 0.0f));

		// 敵の生成 (プレイヤー進行ルートに沿って戦略的かつ段階的に配置)
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(1000.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(1800.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(2500.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(3300.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(4100.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(4800.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);				
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(5400.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		
		// 3. ゴール
		new Goal(VGet(mStageInfo.goalX, mStageInfo.goalY, 0.0f), 150, 150);
	}
}
