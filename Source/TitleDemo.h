#pragma once
#include "DxLib.h"

class TextureAnimation;

// タイトル画面でワイヤーアクションのチュートリアル・演出を自動ループ再生するデモクラス
class TitleDemo
{
public:
	TitleDemo();
	~TitleDemo();

	// デモ状態・キャラクター座標の初期化
	// 入力: なし / 出力: なし / 副作用: 座標・タイマー・ステートのリセット
	void Reset();

	// デモステートマシンおよびスプライトアニメーションの進行
	// 入力: なし / 出力: なし / 副作用: プレイヤー・敵の座標および振り子角度の更新
	void Update();

	// ワイヤー・プレイヤー・敵スプライトの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw();

private:
	// デモシーケンスの進行ステート
	enum class DemoState
	{
		StateRun,		// 画面左外から崖手前まで走行
		StateFireWire,	// アンカーへワイヤー射出伸張
		StateSwing,		// 振り子運動で谷を跳躍
		StateLandRun,	// 右岸へ着地し画面外へ離脱
		StateWaitReset	// 再ループ待機
	};

	void UpdateState();
	void UpdateAnimation();
	void UpdateRun();
	void UpdateFireWire();
	void UpdateSwing();
	void UpdateLandRun();
	void UpdateWaitReset();

	TextureAnimation* mpPlayerAnim;
	TextureAnimation* mpEnemyAnim;

	DemoState mState;
	int mnWaitTimer;
	int mnBuildingHandle;

	VECTOR mvPlayerPos;
	VECTOR mvEnemyPos;
	VECTOR mvHookPos;

	float mfPlayerVelY;
	float mfEnemyVelY;
	float mfWireLength;
	float mfPendulumAngle;
	float mfPendulumAngularVelocity;
	float mfWireExtendRatio;
	bool mbIsWireVisible;

	static constexpr float EnemyGroundY = 850.0f;
	static constexpr float PlayerGroundY = 900.0f;
	static constexpr float PlatformTopY = 980.0f;
	static constexpr float LeftCliffEndX = 780.0f;
	static constexpr float RightCliffStartX = 1180.0f;
	static constexpr float PlayerRunSpeed = 6.0f;
	static constexpr float EnemyRunSpeed = 4.2f;
	static constexpr float Gravity = 0.5f;
};