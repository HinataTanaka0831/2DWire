#pragma once
#include "DxLib.h"
#include "TextureAnimation.h"
#include "Object2D.h"
#include "AnimationController.h"


class Player : public Object2D
{
private:
	static const int MoveSpeed = 5;

private:
	
	// プレイヤーの状態を管理するアニメーションコントローラー
	AnimationController mAnimController;

public:
	// アニメーション用リソースおよび初期ステータスを初期化
	// 入力：filename（画像パス）, initPos（初期座標）, allNum（全フレーム数）, numX/numY（分割数）, interval（再生間隔）, scale（拡縮率）/ 副作用：初期状態アニメーションの登録
	Player(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scalse, bool type);

	~Player();

	// 物理挙動、入力検知、HP表示制御など毎フレームの更新処理
	// 副作用：位置座標や速度パラメータの更新、アニメーション状態の更新
	void Update() override;

	// ワイヤーのライン描画、アニメーション、およびHPゲージをスクリーンに描画
	void Draw() override;

	// 左右キーおよびマウスによるワイヤー振り子運動などの物理演算と状態遷移制御
	// 副作用：移動速度、座標、ワイヤー状態、アニメーション状態を更新
	void Move();

	// 攻撃アクションの開始と攻撃判定の処理
	// 副作用：攻撃フラグの設定、攻撃判定の実行
	void Attack();

	// キャラクターの頭上に現在HPを示すUIゲージを描画
	void HPGaugeDraw();

	// 被弾時にダメージ量を赤色ゲージとして滑らかに追従させる描画用の幅計算
	// 副作用：演出用HP幅パラメータの更新
	void HPGaugeUpdate();

	// 被弾によるHPの減算、およびHPゼロ時のゲームオーバー遷移トリガー
	// 入力：damage（被弾ダメージ量） / 副作用：HP減算、およびゲームオーバーへのシーン遷移
	void PDamage(int damage);

	float GetVelocityX() const { return mfVelocityX; }
	bool IsFacingLeft() const { return mbFacingLeft; }
	float GetAngularAcceleration() const { return mfAngularAcceleration; } 
	bool IsWireActive() const { return mbIsWireActive; } 

private:      
	bool mbIsWireActive = false;           
	VECTOR mvWireTargetPos;                
	float mfPendulumAngle = 0.0f;           
	float mfPendulumAngularVelocity = 0.0f; 
	float mfWireLength = 0.0f;              
	
	float mfVelocityY = 0.0f;               
	float mfVelocityX = 0.0f;
	// 最後に向いていた方向を保持（true = 左向き）
	bool mbFacingLeft = false;
	float mfAngularAcceleration = 0.0f;		       
	static constexpr float Gravity = 0.5f; 

	int mnHP = 100;  
	int mnMaxHP = 100; 
	int mnWidth = 250; 
	int mnGaugeWidth = 0; 
	int mnGaugeHeight = 20; 
	int mnDamageWidth = 0; 
	int mnMinWidth = 5; 
	int mnDisplayDamage; 
	int mnHPGaugeTimer = 0;
	const float GaugeFrame = 3.0f; 

	bool mbIsAttack = false;
	bool mHasHitThisAttack = false;
	int mnAttackTimer = 0;
	int mnAttackCooldown = 0; // 攻撃できるまでの残り時間
	const int AttackInterval = 16; // 攻撃の間隔（フレーム数）

};

