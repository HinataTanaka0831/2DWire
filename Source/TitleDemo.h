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

	void UpdateState();
	void UpdateAnimation();
	void UpdateRun();
	void UpdateFireWire();
	void UpdateSwing();
	void UpdateLandRun();
	void UpdateWaitReset();

	std::unique_ptr<TextureAnimation> m_playerAnim;
	std::unique_ptr<TextureAnimation> m_enemyAnim;

	DemoState m_state;
	int m_waitTimer;
	int m_buildingHandle;

	VECTOR m_playerPos;
	VECTOR m_enemyPos;
	VECTOR m_hookPos;

	float m_playerVelY;
	float m_enemyVelY;
	float m_wireLength;
	float m_pendulumAngle;
	float m_pendulumAngularVelocity;
	float m_wireExtendRatio;
	bool m_isWireVisible;

	static constexpr float m_enemyGroundY = 850.0f;
	static constexpr float m_playerGroundY = 900.0f;
	static constexpr float m_platformTopY = 980.0f;
	static constexpr float m_leftCliffEndX = 780.0f;
	static constexpr float m_rightCliffStartX = 1180.0f;
	static constexpr float m_playerRunSpeed = 6.0f;
	static constexpr float m_enemyRunSpeed = 4.2f;
	static constexpr float m_gravity = 0.5f;
};