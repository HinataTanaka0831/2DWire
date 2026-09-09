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

	std::unique_ptr<TextureAnimation> m_playerAnim = nullptr;
	std::unique_ptr<TextureAnimation> m_enemyAnim = nullptr;

	DemoState m_state = DemoState::StateRun;
	int m_waitTimer = 0;
	int m_buildingHandle = -1;

	VECTOR m_playerPosition = VGet(0.0f, 0.0f, 0.0f);
	VECTOR m_enemyPosition = VGet(0.0f, 0.0f, 0.0f);
	VECTOR m_hookPosition = VGet(0.0f, 0.0f, 0.0f);

	float m_playerVelocityY = 0.0f;
	float m_enemyVelocityY = 0.0f;
	float m_wireLength = 0.0f;
	float m_pendulumAngle = 0.0f;
	float m_pendulumAngularVelocity = 0.0f;
	float m_wireExtendRatio = 0.0f;
	bool m_isWireVisible = false;

	static constexpr float m_enemyGroundY = 850.0f;
	static constexpr float m_playerGroundY = 900.0f;
	static constexpr float m_platformTopY = 980.0f;
	static constexpr float m_leftCliffEndX = 780.0f;
	static constexpr float m_rightCliffStartX = 1180.0f;
	static constexpr float m_playerRunSpeed = 6.0f;
	static constexpr float m_enemyRunSpeed = 4.2f;
	static constexpr float m_gravity = 0.5f;
};