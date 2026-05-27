#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>
#include "Object2D.h"
#include "AnimationController.h"
#include "TextureAnimation.h"



class Enemy : public Object2D
{
private:
	// 敵の状態を管理するアニメーションコントローラー
	AnimationController mAnimController;

public:
	// コンストラクタ
	Enemy(std::string filename, VECTOR initPos, int t,int s);

	//デストラクタ
	virtual ~Enemy();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 移動処理
	void Move();

	// 画面外に出たら
	bool IsScreenOut();

	//// 初期化
	void Initialize();

	// ダメージ処理
	void ChangeDamage(int damage);
	//void ChangeDamage2(int damage);

	void Calcdamage();


	void Settype(int a) { type = a; }

	void SetEnemytype(int set) { enemytype = set; }
	int GetEnemytype() { return enemytype; }

private:
	VECTOR mvDirection;  // 移動方向
	int StartTime = GetNowCount();  // 時間指定
	static const int MOVE_SPEED = 5;
	int time = 0;
	int type;
	int mnHp = 6;  // HP(Enemyでしか使用しない変数)
	int enemytype;

};


