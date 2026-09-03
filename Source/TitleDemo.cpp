#include "TitleDemo.h"
#include "TextureAnimation.h"
#include "Utility.h"
#include <cmath>

TitleDemo::TitleDemo()
	: m_playerAnim(nullptr)
	, m_enemyAnim(nullptr)
	, m_state(DemoState::StateRun)
	, m_waitTimer(0)
	, m_buildingHandle(-1)
	, m_playerPos(VGet(0.0f, 0.0f, 0.0f))
	, m_enemyPos(VGet(0.0f, 0.0f, 0.0f))
	, m_hookPos(VGet(0.0f, 0.0f, 0.0f))
	, m_playerVelY(0.0f)
	, m_enemyVelY(0.0f)
	, m_wireLength(0.0f)
	, m_pendulumAngle(0.0f)
	, m_pendulumAngularVelocity(0.0f)
	, m_wireExtendRatio(0.0f)
	, m_isWireVisible(false)
{
	// デモ用のため本編Player検索を無効化(type=false)して直接アニメーションを制御
	m_playerAnim = std::make_unique<TextureAnimation>(
		"Resource/Player/anim_walk.png",
		VGet(0.0f, 0.0f, 0.0f),
		4, 4, 1, 7, 1.0f, false);

	m_enemyAnim = std::make_unique<TextureAnimation>(
		"Resource/Enemy/anim_monster01walk.png",
		VGet(0.0f, 0.0f, 0.0f),
		6, 6, 1, 8, 1.0f, false);

	m_buildingHandle = LoadGraph("Resource/WireTarget/Building.png");

	Reset();
}

TitleDemo::~TitleDemo()
{
	if (m_buildingHandle != -1)
	{
		DeleteGraph(m_buildingHandle);
		m_buildingHandle = -1;
	}
}

// デモ状態・キャラクター座標の初期化
// 入力: なし / 出力: なし / 副作用: 座標・タイマー・ステートのリセット
void TitleDemo::Reset()
{
	m_playerPos = VGet(-220.0f, m_playerGroundY, 0.0f);
	m_enemyPos = VGet(-420.0f, m_enemyGroundY, 0.0f);
	m_hookPos = VGet(980.0f, 340.0f, 0.0f);
	m_playerVelY = 0.0f;
	m_enemyVelY = 0.0f;
	m_wireLength = 0.0f;
	m_pendulumAngle = 0.0f;
	m_pendulumAngularVelocity = 0.0f;
	m_wireExtendRatio = 0.0f;
	m_isWireVisible = false;
	m_waitTimer = 0;
	m_state = DemoState::StateRun;
}

// デモステートマシンおよびスプライトアニメーションの進行
// 入力: なし / 出力: なし / 副作用: プレイヤー・敵の座標および振り子角度の更新
void TitleDemo::Update()
{
	UpdateState();
	UpdateAnimation();
}

void TitleDemo::UpdateState()
{
	switch (m_state)
	{
	case DemoState::StateRun:
		UpdateRun();
		break;
	case DemoState::StateFireWire:
		UpdateFireWire();
		break;
	case DemoState::StateSwing:
		UpdateSwing();
		break;
	case DemoState::StateLandRun:
		UpdateLandRun();
		break;
	case DemoState::StateWaitReset:
		UpdateWaitReset();
		break;
	}
}

void TitleDemo::UpdateAnimation()
{
	if (m_playerAnim != nullptr)
	{
		m_playerAnim->SetPosition(m_playerPos);
		m_playerAnim->SetReverse(false);
		m_playerAnim->Update();
	}
	if (m_enemyAnim != nullptr)
	{
		m_enemyAnim->SetPosition(m_enemyPos);
		m_enemyAnim->SetReverse(true);
		m_enemyAnim->Update();
	}
}

void TitleDemo::UpdateRun()
{
	m_playerPos.x += m_playerRunSpeed;
	m_enemyPos.x += m_enemyRunSpeed;

	if (m_playerPos.x >= m_leftCliffEndX - 40.0f)
	{
		m_playerPos.x = m_leftCliffEndX - 40.0f;
		m_state = DemoState::StateFireWire;
		m_isWireVisible = true;
		m_wireExtendRatio = 0.0f;
	}
}

void TitleDemo::UpdateFireWire()
{
	m_wireExtendRatio += 0.08f;
	if (m_wireExtendRatio >= 1.0f)
	{
		m_wireExtendRatio = 1.0f;

		float diffX = m_playerPos.x - m_hookPos.x;
		float diffY = m_playerPos.y - m_hookPos.y;
		m_wireLength = std::sqrt(diffX * diffX + diffY * diffY);
		if (m_wireLength < 1.0f)
		{
			m_wireLength = 1.0f;
		}
		m_pendulumAngle = std::atan2(diffX, diffY);
		m_pendulumAngularVelocity = 0.042f;
		m_state = DemoState::StateSwing;
	}

	m_enemyPos.x += m_enemyRunSpeed;
}

void TitleDemo::UpdateSwing()
{
	if (m_wireLength > 0.0f)
	{
		float angularAcceleration = -(m_gravity / m_wireLength) * std::sin(m_pendulumAngle);
		// デモで確実に右岸へ到達させるための補助推進力
		angularAcceleration += 0.0014f;
		m_pendulumAngularVelocity *= 0.997f;
		m_pendulumAngularVelocity += angularAcceleration;
		m_pendulumAngle += m_pendulumAngularVelocity;

		m_playerPos.x = m_hookPos.x + std::sin(m_pendulumAngle) * m_wireLength;
		m_playerPos.y = m_hookPos.y + std::cos(m_pendulumAngle) * m_wireLength;
	}

	m_enemyPos.x += m_enemyRunSpeed;

	bool isOverRightCliff = m_playerPos.x >= m_rightCliffStartX + 30.0f;
	bool isNearGround = m_playerPos.y >= m_playerGroundY - 12.0f;
	if (isOverRightCliff && isNearGround)
	{
		m_playerPos.y = m_playerGroundY;
		m_isWireVisible = false;
		m_playerVelY = 0.0f;
		m_state = DemoState::StateLandRun;
	}

	// 落下等の予期せぬ挙動発生時の自動リカバリ
	if (m_playerPos.y > Utility::SCREEN_HEIGHT + 80.0f)
	{
		Reset();
	}
}

void TitleDemo::UpdateLandRun()
{
	m_playerPos.x += m_playerRunSpeed;
	m_playerPos.y = m_playerGroundY;
	m_enemyPos.x += m_enemyRunSpeed;
	m_enemyPos.y = m_enemyGroundY;

	if (m_playerPos.x > Utility::SCREEN_WIDTH + 140.0f && m_enemyPos.x > Utility::SCREEN_WIDTH + 140.0f)
	{
		m_state = DemoState::StateWaitReset;
		m_waitTimer = 40;
	}
}

void TitleDemo::UpdateWaitReset()
{
	m_waitTimer--;
	if (m_waitTimer <= 0)
	{
		Reset();
	}
}

// ワイヤー・プレイヤー・敵スプライトの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void TitleDemo::Draw()
{
	if (m_buildingHandle != -1)
	{
		DrawRotaGraph((int)m_hookPos.x, (int)m_hookPos.y + 100, 1.0f, 0.0f, m_buildingHandle, TRUE);
	}


	if (m_isWireVisible)
	{
		float tipX = m_hookPos.x;
		float tipY = m_hookPos.y;
		if (m_state == DemoState::StateFireWire)
		{
			tipX = m_playerPos.x + (m_hookPos.x - m_playerPos.x) * m_wireExtendRatio;
			tipY = m_playerPos.y + (m_hookPos.y - m_playerPos.y) * m_wireExtendRatio;
		}

		DrawLine(
			(int)m_playerPos.x, (int)m_playerPos.y,
			(int)tipX, (int)tipY,
			GetColor(200, 255, 255), 4);
	}

	if (m_enemyAnim != nullptr)
	{
		m_enemyAnim->Draw(0.0f, 0.0f);
	}
	if (m_playerAnim != nullptr)
	{
		m_playerAnim->Draw(0.0f, 0.0f);
	}
}