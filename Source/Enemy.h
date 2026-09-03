#pragma once
#include "DxLib.h"
#include "TextureAnimation.h"
#include "Object2D.h"
#include "AnimationController.h"

// 敵キャラクターのAI行動（索敵・追跡・攻撃・重なり解消）およびHP管理を行うクラス
class Enemy : public Object2D
{
private:
	static const int MoveSpeed = 3;

private:
	AnimationController m_animController;

public:
	// 敵キャラクターの初期化とアニメーションの登録
	// 入力: filename, initPos, allNum, numX, numY, interval, scale, type / 出力: なし / 副作用: AnimationControllerへのステート登録
	Enemy(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type);
	~Enemy();

	// 毎フレームのAI思考、移動、被弾点滅、HP更新
	// 入力: なし / 出力: なし / 副作用: 座標・アニメーション・HPの更新
	void Update() override;

	// カメラ座標を加味した敵スプライト（被弾フラッシュ含む）とHPゲージの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// プレイヤーとの距離に応じた追跡・攻撃AIルーチンおよび他敵との重なり分離
	// 入力: なし / 出力: なし / 副作用: mvPosition, mvDirection, mnAttackCooldownの更新
	void Move();

	bool IsScreenOut();

	// プレイヤーからの被弾処理と死亡時削除フラグ設定
	// 入力: damage(ダメージ量) / 出力: なし / 副作用: hp減算、被弾点滅タイマー開始、SetDeleteFlag
	void EDamage(int damage);

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
	int m_time = 0;
	int m_type;

	// AI意思決定用の距離・時間閾値定数
	static constexpr float m_searchRange = 700.0f; // プレイヤーを検知して追跡を開始する距離
	static constexpr float m_attackRange = 230.0f; // 攻撃アニメーションに切り替える距離
	static constexpr int m_attackInterval = 60;    // 毎フレーム多重ヒットによる瞬殺を防ぐクールダウン(60F = 1秒)

	int m_attackCooldown = 0; // 攻撃判定の間隔制御タイマー

	int m_hp = 100;
	int m_maxHP = 100;
	int m_width = 250;
	int m_gaugeWidth = 0;
	int m_gaugeHeight = 20;
	int m_damageWidth = 0;
	int m_minWidth = 5;
	int m_displayDamage = m_maxHP;
	int m_hpGaugeTimer = 0;
	int m_hitFlashTimer = 0;

	static const int HitFlashDuration = 60;
	static const int HitFlashInterval = 6;

	const float GaugeFrame = 3.0f;

	bool m_isCollidingWithPlayer;
};