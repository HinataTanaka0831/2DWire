#pragma once
#include "Bullet.h"

// 派生弾挙動（特殊弾）を実装するためのクラス
class OriginalBullet : public Bullet
{
public:
	// 特殊弾オブジェクトの初期化
	// 入力: initPos(発射座標), filename(画像パス) / 出力: なし / 副作用: 基底Bulletの初期化
	OriginalBullet(VECTOR initPos, std::string filename);
	~OriginalBullet();

	// 独自弾道の更新および基底Update処理の実行
	// 入力: なし / 出力: なし / 副作用: 座標・衝突判定の更新
	void Update();

	void Draw();
	void Move();
	bool IsScreenOut();
};