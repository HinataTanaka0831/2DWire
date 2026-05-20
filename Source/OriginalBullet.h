#pragma once
#include "Bullet.h"


class OriginalBullet : public Bullet
{
public:
	// コンストラクタ
	OriginalBullet(VECTOR initPos, std::string filename);

	// デストラクタ
	~OriginalBullet();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 移動処理
	void Move();

	// 画面外に出たかどうか
	bool IsScreenOut();

};
