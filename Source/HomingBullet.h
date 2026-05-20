#pragma once
#include "Bullet.h"
#include "Object2D.h"

// クラスの前方宣言
// include せずに存在だけを知らせる宣言
// Enemyクラスの中のメンバ関数などは呼べない
//class Enemy;

class HomingBullet : public Object2D
{
public:
	// コンストラクタ
	HomingBullet(VECTOR initPos, std::string filename);
	// デストラクタ
	virtual ~HomingBullet();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	//方向計算
	void CalcDirection();

private:

};
