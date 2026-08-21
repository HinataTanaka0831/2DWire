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
	AnimationController mAnimController;

public:
	// 敵キャラクターのアニメーションおよびステータスを初期化
	// 入力：filename（画像パス）, initPos（初期座標）, allNum/numX/numY（画像分割情報）, interval（アニメーション間隔）, scale（描画倍率）, type（反転有無） / 副作用：Idle/Moving/Attacking アニメーションの登録
	Enemy(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type);

	~Enemy();

	// 毎フレームのAI思考（追跡・攻撃判断）および物理座標とアニメーションの更新処理
	// 副作用：位置座標（mvPosition）、移動方向（mvDirection）、アニメーション状態、HP等の更新
	void Update() override;

	// アニメーションコントローラーを呼び出し、カメラオフセットを考慮した画面上の位置に描画
	void Draw() override;

	// プレイヤーとの距離に応じて、巡回・追跡・攻撃のAI意思決定および移動処理を実行
	// 副作用：mvPosition、mvDirection、mnAttackCooldown の更新
	void Move();

	// 画面の左端から完全にはみ出したかを判定
	// 出力：画面外に出た場合は true
	bool IsScreenOut();

	// キャラクターの頭上に現在HPを示すUIゲージを描画
	void HPGaugeDraw();

	// 被弾時にダメージ量を赤色ゲージとして滑らかに追従させる描画用の幅計算
	// 副作用：演出用HP幅パラメータの更新
	void HPGaugeUpdate();

	// 被弾によるHP減算、およびHPゼロ時のオブジェクト消滅フラグの設定
	// 入力：damage（被弾ダメージ量） / 副作用：mnHpの減算およびdeleteFlagの設定
	void EDamage(int damage);

	// 重なった敵どうしを少しずつ離して、同じ場所に積み重ならないようにする
	void ResolveEnemyOverlap();

	VECTOR GetDirection() const { return mvDirection; }

private:
	VECTOR mvDirection;  
	int startTime = GetNowCount();  
	static const int MoveSpeed = 3; // プレイヤーの移動速度（5）に対して、ゲームバランス上追跡を可能にするため適度に遅い速度（3）に調整
	int time = 0;
	int type;

	// AI意思決定用のパラメータ定数
	static constexpr float Search_Range = 700.0f; // プレイヤーを検知して追跡を開始するワールド座標系での閾値
	static constexpr float Attack_Range = 230.0f;  // 攻撃アニメーションに切り替え、定期ダメージ判定を行う距離
	static constexpr int Attack_Interval = 60;     // 攻撃が毎フレーム多重ヒットしてプレイヤーが瞬殺されるバグを防ぐためのクールダウン時間 (90フレーム = 1.5秒)

	int mnAttackCooldown = 0; // 攻撃の実行周期を制御するクールダウンカウンター

	int hp = 100;
	int maxHP = 100;
	int width = 250;
	int gaugeWidth = 0;
	int gaugeHeight = 20;
	int damageWidth = 0;
	int minWidth = 5;
	int displayDamage;
	int hpGaugeTimer = 0;
	int mnHitFlashTimer = 0;

	static const int HitFlashDuration = 60;
	static const int HitFlashInterval = 6;

	const float GaugeFrame = 3.0f;

	bool isCollidingWithPlayer;

};


