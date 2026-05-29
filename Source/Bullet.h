#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>
#include "Object2D.h"

class Player;

class Bullet : public Object2D
{


public:
	// コンストラクタ
	Bullet(VECTOR initPos, std::string filename);

	// デストラクタ
	virtual ~Bullet();

	// 更新
	virtual void Update() override;

	// 描画
	virtual void Draw() override;

	// 移動処理
	void Move();

	// 画面外に出たかどうか
	bool IsScreenOut();

	// 当たり判定
	void CalcCollision();


	// 進行方向
	void SetDirection(VECTOR dir) { mvDirection = dir; }

	// 速度設定
	void SetSpeed(float speed) { mfspeed = speed; }


protected:
	VECTOR mvDirection;  // 移動方向
	float mfAngle;       // 現在の目標角度
	float mfspeed;
};
