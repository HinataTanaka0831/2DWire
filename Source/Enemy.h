#pragma once
#include "DxLib.h"
#include "TextureAnimation.h"
#include "Object2D.h"
#include "AnimationController.h"

// 敵キャラクターのAI行動（索敵・追跡・攻撃・重なり解消）およびHP管理を行うクラス
class Enemy : public Object2D
{
public:
	// 敵キャラクターの初期化とアニメーションの登録
	// 入力: fileName, initPosition, allNum, numX, numY, interval, scale, isPlayer / 出力: なし / 副作用: AnimationControllerへのステート登録
	Enemy(std::string fileName, VECTOR initPosition, int allNum, int numX, int numY, int interval, float scale, bool isPlayer);
	~Enemy();

	// 毎フレームのAI思考、移動、被弾点滅、HP更新、死亡アニメーション終了時の削除フラグ設定
	// 入力: なし / 出力: なし / 副作用: 座標・アニメーション・HPの更新
	void Update() override;

	// カメラ座標を加味した敵スプライト（被弾フラッシュ含む）とHPゲージの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// プレイヤーとの距離に応じた追跡・攻撃AIルーチンおよび他敵との重なり分離
	// 入力: なし / 出力: なし / 副作用: mvPosition, mvDirection, mnAttackCooldownの更新
	void Move();

	// プレイヤーからの被弾処理と死亡時アニメーション後に削除するためのタイマーを設定
	// 入力: damage(ダメージ量) / 出力: なし / 副作用: hp減算、被弾点滅タイマー開始、死亡時アニメーションタイマー開始
	void Damage(int damage);

	// 敵同士が同じ座標に密集して固まるのを防ぐ斥力（位置補正）処理
	// 入力: なし / 出力: なし / 副作用: mvPositionの微小補正
	void ResolveEnemyOverlap();

	// 敵頭上にHPゲージ（現在値＋ダメージ演出バー）を描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void HPGaugeDraw();

	// 被弾時に減少分を滑らかに追従させるHP演出ゲージの幅計算
	// 入力: なし / 出力: なし / 副作用: displayDamage, damageWidth, gaugeWidthの更新
	void HPGaugeUpdate();

private:
	const int MoveSpeed;                      // 敵の移動速度（ピクセル/フレーム）
	int m_maxHp;                              // 敵の最大HP
	int m_hp;                                 // 敵の現在HP
	int m_gaugeWidth;                         // HPゲージUIの横幅
	int m_gaugeHeight;                        // HPゲージUIの縦幅
	int m_nowHpGaugeWidth;                    // 現在HPゲージの描画幅
	int m_damageBarWidth;                     // 遅延ダメージバーの描画幅
	int m_minWidth;                           // ゲージ最小描画幅
	int m_damage;                             // 遅延追従用HP表示値
	int m_hpGaugeTimer;                       // HPゲージの演出更新タイマー
	const float GaugeFrame;                   // HPゲージの演出更新間隔（フレーム数）
	int m_hitFlashTimer;                      // 被弾フラッシュタイマー
	int m_attackAnimationTimer;               // 攻撃アニメーションタイマー
	int m_deadAnimationTimer;                 // 死亡アニメーションタイマー
	int m_attackCooldown;                     // 攻撃判定の間隔制御タイマー
	const int HitFlashDuration;               // 被弾フラッシュの持続時間（フレーム数）
	const int HitFlashInterval;               // 被弾フラッシュの点滅間隔（フレーム数）

	AnimationController m_animController;     // 敵キャラクターの状態に応じたアニメーション切り替えを管理するコントローラ

	// AI意思決定用の距離・時間閾値定数
	const float m_searchRange;                // プレイヤーを検知して追跡を開始する距離
	const float m_attackRange;                // 攻撃アニメーションに切り替える距離
	const int m_attackInterval;               // 毎フレーム多重ヒットによる瞬殺を防ぐクールダウン(60F = 1秒)

	bool m_isAttack;                          // 攻撃アニメーション中フラグ
	bool m_isDead;                            // 死亡アニメーション中フラグ
	bool m_isCollidingWithPlayer;             // プレイヤーとの接触判定フラグ

};