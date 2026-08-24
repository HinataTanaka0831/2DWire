#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>
#include "Object2D.h"

class Player;

// プレイヤーまたは敵から直線的に発射される通常弾オブジェクト
class Bullet : public Object2D
{
public:
	// 弾オブジェクトの初期化と発射パラメータの設定
	// 入力: initPos(発射座標), filename(弾画像パス) / 出力: なし / 副作用: Bullet2Dタグの設定
	Bullet(VECTOR initPos, std::string filename);
	virtual ~Bullet();

	// 移動、敵との当たり判定、画面外破棄判定の実行
	// 入力: なし / 出力: なし / 副作用: 座標更新、衝突時の相手へのダメージ適用、削除フラグ設定
	virtual void Update() override;

	// カメラ座標を加味した弾の描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	virtual void Draw() override;

	// 直線移動計算
	// 入力: なし / 出力: なし / 副作用: 座標の更新
	void Move();

	// 画面外（有効描画領域外）へ到達したかの判定
	// 入力: なし / 出力: 画面外ならtrue / 副作用: なし
	bool IsScreenOut();

	// 敵オブジェクト一覧との交差判定およびダメージ適用
	// 入力: なし / 出力: なし / 副作用: 衝突時に弾自身の削除フラグを立てる
	void CalcCollision();

	void SetDirection(VECTOR dir) { mvDirection = dir; }
	void SetSpeed(float speed) { mfspeed = speed; }

protected:
	VECTOR mvDirection;  // 進行方向ベクトル（正規化ベクトル）
	float mfAngle;       // 弾の回転角度
	float mfspeed;       // フレームあたりの移動速度
};