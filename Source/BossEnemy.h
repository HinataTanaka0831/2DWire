#pragma once
#include "DxLib.h"
#include "Texture.h"
#include "Object2D.h"

class Player;

class BossEnemy : public Object2D
{
public:
	BossEnemy(VECTOR initPos);
	~BossEnemy();

	void Update();
	void Draw();
	void Move();
	void BDamage();
	void Shot();

public:

	void CreateBullet(VECTOR initPos,VECTOR Dire, float speed);

	void RandomBullet();

private:      // ?????o???
	// ?e??ｳ・u?J?E???^
	int mnShotCounter;
	static const int BULLET_MAX = 10;   // ?e??・
	static const int Boss_SHOT_INTERVAL = 10;   // ?e??ｳﾃ??u?i?P??F?t???[???j
};