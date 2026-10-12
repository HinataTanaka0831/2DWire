#pragma once
#include "DxLib.h"
#include <memory>

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

	// デモステートマシンの進行と座標・速度・角度の更新
	// 入力: なし / 出力: なし / 副作用: m_state, m_playerPosition, m_enemyPosition, m_pendulumAngleの更新
	void UpdateState();

	// デモステートに応じたスプライトアニメーションの更新
	// 入力: なし / 出力: なし / 副作用: m_playerAnim, m_enemyAnimの更新
	void UpdateAnimation();
	
	// デモステートごとの座標・速度の更新、プレイヤーと敵の走行
	// 入力: なし / 出力: なし / 副作用: m_playerPosition, m_enemyPositionの更新
	void UpdateRun();

	// デモステートごとの座標・速度・角度の更新、ワイヤー射出伸張の進行、敵の走行
	// 入力: なし / 出力: なし / 副作用: m_playerPosition, m_enemyPosition, m_pendulumAngleの更新
	void UpdateFireWire();

	// デモステートごとの座標・速度・角度の更新、振り子運動による谷渡り、敵の走行
	// 入力: なし / 出力: なし / 副作用: m_playerPosition, m_enemyPosition, m_pendulumAngleの更新
	void UpdateSwing();

	// デモステートごとの座標・速度の更新、右岸への着地と走行、敵の走行
	// 入力: なし / 出力: なし / 副作用: m_playerPosition, m_enemyPositionの更新
	void UpdateLandRun();

	// デモステートごとの待機タイマー更新、ループ再開のトリガー
	// 入力: なし / 出力: なし / 副作用: m_waitTimerの更新、m_stateのStateRunへの遷移
	void UpdateWaitReset();

	std::unique_ptr<TextureAnimation> m_playerAnim;       // プレイヤーのスプライトアニメーション
	std::unique_ptr<TextureAnimation> m_enemyAnim;        // 敵キャラクターのスプライトアニメーション

	DemoState m_state;                                    // デモステートマシンの現在ステート
	int m_waitTimer;                                      // デモループ待機用タイマー
	int m_buildingHandle;                                 // 背景ビル画像のグラフィックハンドル

	VECTOR m_playerPosition;                              // プレイヤーの座標
	VECTOR m_enemyPosition;                               // 敵キャラクターの座標
	VECTOR m_hookPosition;                                // ワイヤーの係留先アンカー座標

	float m_playerVelocityY;                              // プレイヤーの鉛直方向速度
	float m_enemyVelocityY;                               // 敵キャラクターの鉛直方向速度
	float m_wireLength;                                   // ワイヤーの固定長さ（係留時）
	float m_pendulumAngle;                                // 振り子の現在角度(rad)
	float m_pendulumAngularVelocity;                      // 振り子の角速度
	float m_wireExtendRatio;                              // ワイヤー射出伸張の進行比率（0.0～1.0）
	bool m_isWireVisible;                                 // ワイヤーの可視状態

	const float m_enemyGroundY;                           // 敵キャラクターの地面Y座標
	const float m_playerGroundY;                          // プレイヤーの地面Y座標
	const float m_platformTopY;                           // 崖の上面Y座標
	const float m_leftCliffEndX;                          // 左岸の崖端X座標
	const float m_rightCliffStartX;                       // 右岸の崖端X座標
	const float m_playerRunSpeed;                         // プレイヤーの走行速度
	const float m_enemyRunSpeed;                          // 敵キャラクターの走行速度
	const float m_gravity;                                // 重力加速度定数
};