#pragma once
#include "DxLib.h"
#include "TextureAnimation.h"
#include "Object2D.h"
#include "AnimationController.h"


class Player : public Object2D
{
private:
	static const int MOVE_SPEED = 5;
	static const int BULLET_MAX = 10;
	static const int BULLET_SHOT_INTERVAL = 10;


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

	// キャラクターの頭上に現在HPを示すUIゲージを描画
	void HPGaugeDraw();

	// 被弾時にダメージ量を赤色ゲージとして滑らかに追従させる描画用の幅計算
	// 副作用：演出用HP幅パラメータの更新
	void HPGaugeUpdate();

	// 被弾によるHPの減算、およびHPゼロ時のゲームオーバー遷移トリガー
	// 入力：damage（被弾ダメージ量） / 副作用：HP減算、およびゲームオーバーへのシーン遷移
	void PDamage(int damage);

	float GetVelocityX() const { return mVelocityX; } 
	float GetAngularAcceleration() const { return angularAcceleration; } 
	bool IsWireActive() const { return mbIsWireActive; } 

private:      
	int mnBulletShotCounter;

	bool mbIsWireActive = false;           
	VECTOR mvWireTargetPos;                
	float mPendulumAngle = 0.0f;           
	float mPendulumAngularVelocity = 0.0f; 
	float mWireLength = 0.0f;              
	
	float mVelocityY = 0.0f;               
	float mVelocityX = 0.0f;               
	float angularAcceleration = 0.0f;		       
	static constexpr float GRAVITY = 0.5f; 

	int Hp = 100;  
	int maxHp = 100; 
	int width = 250; 
	int gaugeWidth = 0; 
	int gaugeHeight = 20; 
	int damageWidth = 0; 
	int minWidth = 5; 
	int displayDamage; 
	int Timer = 0;
	const float Gauge_Frame = 3.0f; 

	bool mbIsAttack = false;
	int mnAttackTimer = 0;
};

