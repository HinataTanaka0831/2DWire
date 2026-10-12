#pragma once
#include "DxLib.h"
#include "TextureAnimation.h"
#include "Object2D.h"
#include "AnimationController.h"

// プレイヤーキャラクターの入力制御・ワイヤー振り子物理演算・近接攻撃・HPUIを統括するクラス
class Player : public Object2D
{
public:
	// プレイヤーオブジェクトおよび初期ステートの初期化
	// 入力: fileName, initPosition, allNum, numX, numY, interval, scale, isPlayer / 出力: なし / 副作用: 各種アニメーションの登録
	Player(std::string fileName, VECTOR initPosition, int allNum, int numX, int numY, int interval, float scale, bool isPlayer);
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
	void Damage(int damage);

	float GetVelocityX() const { return m_velocityX; }
	bool IsFacingLeft() const { return m_facingLeft; }
	float GetAngularAcceleration() const { return m_angularAcceleration; } 
	bool IsWireActive() const { return m_isWireActive; } 
	bool IsDead() const { return m_isDead; }

private:      
	AnimationController m_animController; // プレイヤーの状態別アニメーション管理

	const int MoveSpeed;
	int m_maxHp;                          // 最大HP
	int m_hp;                             // 現在HP
	int m_gaugeWidth;                     // HPゲージUIの横幅
	int m_gaugeHeight;                    // HPゲージUIの縦幅
	int m_nowHpGaugeWidth;                // 現在HPゲージの描画幅
	int m_damageBarWidth;                 // 遅延ダメージバーの描画幅
	int m_minWidth;                       // ゲージ最小描画幅
	int m_damage;                         // 遅延追従用HP表示値
	int m_hpGaugeTimer;                   // ゲージ更新間隔カウンタ
	const float GaugeFrame;               // HPゲージ更新間隔フレーム数
	bool m_isAttack;                      // 攻撃アクション中フラグ
	bool m_hasHitThisAttack;              // 1回のアクションでの多重ヒット防止フラグ
	int m_attackAnimationTimer;           // 攻撃アニメーション持続タイマー
	int m_attackCooldown;                 // 次の攻撃実行までのクールダウン
	const int AttackInterval;             // 攻撃間隔フレーム数
	int m_damageCooldown;                 // 次のプレイヤーが受けるダメージ実行までのクールダウン
	const int DamageInterval;             // プレイヤーが受けるダメージ間隔フレーム数

	bool m_isWireActive;                  // ワイヤー射出・係留中フラグ
	VECTOR m_wireTargetPos;               // ワイヤーの係留先アンカー座標
	float m_pendulumAngle;                // 振り子の現在角度(rad)
	float m_pendulumAngularVelocity;      // 振り子の角速度
	float m_wireLength;                   // 係留時のワイヤー固定長
	float m_angularAcceleration;          // 振り子運動の角加速度
	
	float m_velocityY;                    // Y軸移動速度
	float m_velocityX;                    // X軸移動速度
	bool m_facingLeft;                    // 水平方向の向き保持フラグ(true: 左向き)
	const float m_gravity;                // 重力加速度定数

	bool m_isJump;                        // ジャンプアクション中のフラグ
	bool m_isDead;                        // プレイヤーがやられたかどうかのフラグ
};