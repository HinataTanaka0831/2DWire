#pragma once
#include "DxLib.h"
#include "TextureAnimation.h"
#include "Object2D.h"
#include "AnimationController.h"


class Player : public Object2D
{
private:  // Playerクラスで使用する定数の定義
	static const int MOVE_SPEED = 5;    // 移動速度
	static const int BULLET_MAX = 10;   // 弾の個数
	static const int BULLET_SHOT_INTERVAL = 10;   // 弾を打てる間隔（単位：フレーム）


private:
	
	// プレイヤーの状態を管理するアニメーションコントローラー
	AnimationController mAnimController;

public:
	// コンストラクタ
	Player(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale);

	// デストラクタ
	virtual ~Player();


	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 移動処理
	void Move();

	// HPゲージの描画
	void HPGaugeDraw();

	// HPゲージの更新
	void HPGaugeUpdate();

	// ダメージ処理
	void PDamage(int damage);

	float GetVelocityX() const { return mVelocityX; } // 横方向の速度を取得
	float GetAngularAcceleration() const { return angularAcceleration; } // 振り子の角加速度を取得
	bool IsWireActive() const { return mbIsWireActive; } // ワイヤー使用中かを取得

private:      // メンバ変数
	// 弾撃ち間隔カウンター
	int mnBulletShotCounter;

	// ワイヤーアクション用変数
	bool mbIsWireActive = false;           // ワイヤー使用中か
	VECTOR mvWireTargetPos;                // ワイヤーの支点座標
	float mPendulumAngle = 0.0f;           // 振り子の角度
	float mPendulumAngularVelocity = 0.0f; // 振り子の角速度
	float mWireLength = 0.0f;              // ワイヤーの長さ
	
	// 物理演算用変数
	float mVelocityY = 0.0f;               // 縦方向の速度
	float mVelocityX = 0.0f;               // 横方向の速度
	float angularAcceleration;		       // 振り子の角加速度
	static constexpr float GRAVITY = 0.5f; // 重力加速度

	// HP関連の変数
	int Hp = 100;  // HP
	int maxHp = 100; // 最大HP
	int width = 250; // プレイヤーの幅
	int gaugeWidth = 0; // HPゲージの幅
	int gaugeHeight = 20; // HPゲージの高さ
	int damageWidth = 0; // ダメージ表示の幅
	int minWidth = 5; // HPゲージの最小幅
	int displayDamage; // HPゲージでダメージを受けた表示位置
	const float gaugeSpeed = 1.0f; // HPゲージの表示位置が変化する速度
};
