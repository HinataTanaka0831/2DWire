#pragma once
#include "DxLib.h"
#include "TextureAnimation.h"
#include "Object2D.h"
#include "AnimationController.h"

// プレイヤーキャラクターの入力制御・ワイヤー振り子物理演算・近接攻撃・HPUIを統括するクラス
class Player : public Object2D
{
private:
	static const int MoveSpeed = 5;

private:
	AnimationController mAnimController; // プレイヤーの状態別アニメーション管理

public:
	// プレイヤーオブジェクトおよび初期ステートの初期化
	// 入力: filename, initPos, allNum, numX, numY, interval, scale, type / 出力: なし / 副作用: 各種アニメーションの登録
	Player(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type);
	~Player();

	// 毎フレームの物理挙動、入力検知、攻撃・HPゲージ更新
	// 入力: なし / 出力: なし / 副作用: 座標・速度・ステート・HPの更新
	void Update() override;

	// ワイヤーライン、プレイヤースプライト、HPゲージの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// キー/マウス入力に基づく歩行・ジャンプ・ワイヤー振り子運動物理演算
	// 入力: なし / 出力: なし / 副作用: 座標、速度、振り子角度、ワイヤー状態の更新
	void Move();

	// 近接攻撃の当たり判定生成と敵へのダメージ適用
	// 入力: なし / 出力: なし / 副作用: 攻撃判定Rectの計算と敵へのEDamage呼び出し
	void Attack();

	// プレイヤー頭上にHPゲージを描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void HPGaugeDraw();

	// 被弾時にダメージ減少分を滑らかに追従させる描画幅計算
	// 入力: なし / 出力: なし / 副作用: mnDisplayDamage, mnDamageWidth, mnGaugeWidthの更新
	void HPGaugeUpdate();

	// 被弾によるHP減算とゲームオーバー遷移トリガー
	// 入力: damage(ダメージ量) / 出力: なし / 副作用: HP減算、HP<=0時のSCENE_GAMEOVER遷移要求
	void PDamage(int damage);

	float GetVelocityX() const { return mfVelocityX; }
	bool IsFacingLeft() const { return mbFacingLeft; }
	float GetAngularAcceleration() const { return mfAngularAcceleration; } 
	bool IsWireActive() const { return mbIsWireActive; } 
	bool IsDead() const { return mbIsDead; }

private:      
	bool mbIsWireActive = false;            // ワイヤー射出・係留中フラグ
	VECTOR mvWireTargetPos;                 // ワイヤーの係留先アンカー座標
	float mfPendulumAngle = 0.0f;           // 振り子の現在角度(rad)
	float mfPendulumAngularVelocity = 0.0f; // 振り子の角速度
	float mfWireLength = 0.0f;              // 係留時のワイヤー固定長
	float mfAngularAcceleration = 0.0f;     // 振り子運動の角加速度
	
	float mfVelocityY = 0.0f;               // Y軸移動速度
	float mfVelocityX = 0.0f;               // X軸移動速度
	bool mbFacingLeft = false;              // 水平方向の向き保持フラグ(true: 左向き)
	static constexpr float Gravity = 0.5f;  // 重力加速度定数

	int mnHP = 100;                         // 現在HP
	int mnMaxHP = 100;                      // 最大HP
	int mnWidth = 250;                      // HPゲージUIの横幅
	int mnGaugeWidth = 0;                   // 実HPバーの描画幅
	int mnGaugeHeight = 20;                 // HPゲージUIの縦幅
	int mnDamageWidth = 0;                  // 遅延ダメージバーの描画幅
	int mnMinWidth = 5;                     // ゲージ最小描画幅
	int mnDisplayDamage;                    // 遅延追従用HP表示値
	int mnHPGaugeTimer = 0;                 // ゲージ更新間隔カウンタ
	const float GaugeFrame = 3.0f; 

	bool mbIsAttack = false;                // 攻撃アクション中フラグ
	bool mbHasHitThisAttack = false;        // 1回のアクションでの多重ヒット防止フラグ
	int mnAttackAnimationTimer = 0;         // 攻撃アニメーション持続タイマー
	int mnAttackCooldown = 0;               // 次の攻撃実行までのクールダウン
	const int AttackInterval = 16;          // 攻撃間隔フレーム数

	int mnDamageCooldown = 0;              // 次のプレイヤーが受けるダメージ実行までのクールダウン
	const int DamageInterval = 10;          // プレイヤーが受けるダメージ間隔フレーム数

	bool mbIsJump = false;                 // ジャンプアクション中のフラグ

	bool mbIsDead = false;                 // プレイヤーがやられたかどうかのフラグ
};