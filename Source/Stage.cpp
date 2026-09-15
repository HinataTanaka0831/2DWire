#include "Stage.h"
#include "WireTarget.h"
#include "Goal.h"
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

// 指定ステージ番号に応じたオブジェクト群の一括インスタンス化
// 入力: stageNum(ステージ番号) / 出力: なし / 副作用: WireTarget, Enemy, Goalの生成
void Stage::LoadStage(int stageNum)
{
	if (stageNum == 1)
	{
		m_stageInfo.playerStartX = 300.0f;
		m_stageInfo.playerStartY = 1000.0f;
		m_stageInfo.goalX = 5600.0f;
		m_stageInfo.goalY = 1000.0f;
		m_stageInfo.cameraMinX = 0.0f;
		m_stageInfo.cameraMaxX = 5600.0f - Utility::ScreenWidth / 1.0f;
		m_stageInfo.playerMinX = 100.0f;
		m_stageInfo.playerMaxX = 5600.0f;

		// ワイヤーアクションの中継点となる建物を等間隔に配置
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(600.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(1300.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(2000.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building_Nolight.png", VGet(2750.0f, 830.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(3500.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(4200.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(4900.0f, 800.0f, 0.0f));

		// 進行ルートに沿って敵キャラクターを順次配置
		new Enemy("Resource/Enemy/enemy_monster01.png", VGet(1000.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/enemy_monster01.png", VGet(1800.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/enemy_monster01.png", VGet(2500.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/enemy_monster01.png", VGet(3300.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/enemy_monster01.png", VGet(4100.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/enemy_monster01.png", VGet(4800.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/enemy_monster01.png", VGet(5400.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);

		new Goal(VGet(m_stageInfo.goalX, m_stageInfo.goalY, 0.0f), 150, 150);
	}
	else if (stageNum == 2)
	{
		m_stageInfo.playerStartX = 300.0f;
		m_stageInfo.playerStartY = 1000.0f;
		m_stageInfo.goalX = 9000.0f;
		m_stageInfo.goalY = 1000.0f;
		m_stageInfo.cameraMinX = 0.0f;
		m_stageInfo.cameraMaxX = 9000.0f - Utility::ScreenWidth / 1.0f;
		m_stageInfo.playerMinX = 100.0f;
		m_stageInfo.playerMaxX = 9000.0f;

		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(600.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(1300.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(2000.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building_Nolight.png", VGet(2750.0f, 830.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building.png", VGet(3500.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Building.png", VGet(4200.0f, 800.0f, 0.0f));
		new WireTarget("Resource/WireTarget/Apartment building_Nolight.png", VGet(4900.0f, 800.0f, 0.0f));

		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(1000.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(1800.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(2500.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(3300.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(4100.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(4800.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);				
		new Enemy("Resource/Enemy/Character_Monster01_Idle.png", VGet(5400.0f, 1000.0f, 0.0f), 1, 1, 1, 10, 1.0f, false);
		
		new Goal(VGet(m_stageInfo.goalX, m_stageInfo.goalY, 0.0f), 150, 150);
	}
}