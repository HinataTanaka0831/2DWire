#pragma once
#include "Bullet.h"
#include "Object2D.h"

// 最寄りの敵を検知して角度を自動補正・追従する誘導弾クラス
class HomingBullet : public Object2D
{
public:
	// 誘導弾オブジェクトの生成
	// 入力: initPos(初期座標), filename(画像パス) / 出力: なし / 副作用: HomingBullet2Dタグの設定
	HomingBullet(VECTOR initPos, std::string filename);
	virtual ~HomingBullet();

	// 誘導方向の再計算と移動
	// 入力: なし / 出力: なし / 副作用: 進行方向および座標の更新
	void Update() override;

	void Draw() override;

	// ターゲット（敵）との相対ベクトルから追尾角度を補正
	// 入力: なし / 出力: なし / 副作用: mvDirectionの更新
	void CalcDirection();
};