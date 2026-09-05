#include "TitleDemo.h"
#include "TextureAnimation.h"
#include "Utility.h"
#include <cmath>

TitleDemo::TitleDemo()
	: mpPlayerAnim(nullptr)
	, mpEnemyAnim(nullptr)
	, mState(DemoState::StateRun)
	, mnWaitTimer(0)
	, mnBuildingHandle(-1)
	, mvPlayerPos(VGet(0.0f, 0.0f, 0.0f))
	, mvEnemyPos(VGet(0.0f, 0.0f, 0.0f))
	, mvHookPos(VGet(0.0f, 0.0f, 0.0f))
	, mfPlayerVelY(0.0f)
	, mfEnemyVelY(0.0f)
	, mfWireLength(0.0f)
	, mfPendulumAngle(0.0f)
	, mfPendulumAngularVelocity(0.0f)
	, mfWireExtendRatio(0.0f)
	, mbIsWireVisible(false)
{
	// デモ用のため本編Player検索を無効化(type=false)して直接アニメーションを制御
	mpPlayerAnim = std::make_unique<TextureAnimation>(
		"Resource/Player/anim_walk.png",
		VGet(0.0f, 0.0f, 0.0f),
		4, 4, 1, 7, 1.0f, false);

	mpEnemyAnim = std::make_unique<TextureAnimation>(
		"Resource/Enemy/anim_monster01walk.png",
		VGet(0.0f, 0.0f, 0.0f),
		6, 6, 1, 8, 1.0f, false);

	mnBuildingHandle = LoadGraph("Resource/WireTarget/Building.png");

	Reset();
}

TitleDemo::~TitleDemo()
{
	if (mnBuildingHandle != -1)
	{
		DeleteGraph(mnBuildingHandle);
		mnBuildingHandle = -1;
	}
}

// デモ状態・キャラクター座標の初期化
// 入力: なし / 出力: なし / 副作用: 座標・タイマー・ステートのリセット
void TitleDemo::Reset()
{
	mvPlayerPos = VGet(-220.0f, PlayerGroundY, 0.0f);
	mvEnemyPos = VGet(-420.0f, EnemyGroundY, 0.0f);
	mvHookPos = VGet(980.0f, 340.0f, 0.0f);
	mfPlayerVelY = 0.0f;
	mfEnemyVelY = 0.0f;
	mfWireLength = 0.0f;
	mfPendulumAngle = 0.0f;
	mfPendulumAngularVelocity = 0.0f;
	mfWireExtendRatio = 0.0f;
	mbIsWireVisible = false;
	mnWaitTimer = 0;
	mState = DemoState::StateRun;
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
	switch (mState)
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
	if (mpPlayerAnim != nullptr)
	{
		mpPlayerAnim->SetPosition(mvPlayerPos);
		mpPlayerAnim->SetReverse(false);
		mpPlayerAnim->Update();
	}
	if (mpEnemyAnim != nullptr)
	{
		mpEnemyAnim->SetPosition(mvEnemyPos);
		mpEnemyAnim->SetReverse(true);
		mpEnemyAnim->Update();
	}
}

void TitleDemo::UpdateRun()
{
	mvPlayerPos.x += PlayerRunSpeed;
	mvEnemyPos.x += EnemyRunSpeed;

	if (mvPlayerPos.x >= LeftCliffEndX - 40.0f)
	{
		mvPlayerPos.x = LeftCliffEndX - 40.0f;
		mState = DemoState::StateFireWire;
		mbIsWireVisible = true;
		mfWireExtendRatio = 0.0f;
	}
}

void TitleDemo::UpdateFireWire()
{
	mfWireExtendRatio += 0.08f;
	if (mfWireExtendRatio >= 1.0f)
	{
		mfWireExtendRatio = 1.0f;

		float diffX = mvPlayerPos.x - mvHookPos.x;
		float diffY = mvPlayerPos.y - mvHookPos.y;
		mfWireLength = std::sqrt(diffX * diffX + diffY * diffY);
		if (mfWireLength < 1.0f)
		{
			mfWireLength = 1.0f;
		}
		mfPendulumAngle = std::atan2(diffX, diffY);
		mfPendulumAngularVelocity = 0.042f;
		mState = DemoState::StateSwing;
	}

	mvEnemyPos.x += EnemyRunSpeed;
}

void TitleDemo::UpdateSwing()
{
	if (mfWireLength > 0.0f)
	{
		float mfAngularAcceleration = -(Gravity / mfWireLength) * std::sin(mfPendulumAngle);
		// デモで確実に右岸へ到達させるための補助推進力
		mfAngularAcceleration += 0.0014f;
		mfPendulumAngularVelocity *= 0.997f;
		mfPendulumAngularVelocity += mfAngularAcceleration;
		mfPendulumAngle += mfPendulumAngularVelocity;

		mvPlayerPos.x = mvHookPos.x + std::sin(mfPendulumAngle) * mfWireLength;
		mvPlayerPos.y = mvHookPos.y + std::cos(mfPendulumAngle) * mfWireLength;
	}

	mvEnemyPos.x += EnemyRunSpeed;

	bool isOverRightCliff = mvPlayerPos.x >= RightCliffStartX + 30.0f;
	bool isNearGround = mvPlayerPos.y >= PlayerGroundY - 12.0f;
	if (isOverRightCliff && isNearGround)
	{
		mvPlayerPos.y = PlayerGroundY;
		mbIsWireVisible = false;
		mfPlayerVelY = 0.0f;
		mState = DemoState::StateLandRun;
	}

	// 落下等の予期せぬ挙動発生時の自動リカバリ
	if (mvPlayerPos.y > Utility::SCREEN_HEIGHT + 80.0f)
	{
		Reset();
	}
}

void TitleDemo::UpdateLandRun()
{
	mvPlayerPos.x += PlayerRunSpeed;
	mvPlayerPos.y = PlayerGroundY;
	mvEnemyPos.x += EnemyRunSpeed;
	mvEnemyPos.y = EnemyGroundY;

	if (mvPlayerPos.x > Utility::SCREEN_WIDTH + 140.0f && mvEnemyPos.x > Utility::SCREEN_WIDTH + 200.0f)
	{
		mState = DemoState::StateWaitReset;
		mnWaitTimer = 40;
	}
}

void TitleDemo::UpdateWaitReset()
{
	mnWaitTimer--;
	if (mnWaitTimer <= 0)
	{
		Reset();
	}
}

// ワイヤー・プレイヤー・敵スプライトの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void TitleDemo::Draw()
{
	if (mnBuildingHandle != -1)
	{
		DrawRotaGraph((int)mvHookPos.x, (int)mvHookPos.y + 100, 1.0f, 0.0f, mnBuildingHandle, TRUE);
	}


	if (mbIsWireVisible)
	{
		float tipX = mvHookPos.x;
		float tipY = mvHookPos.y;
		if (mState == DemoState::StateFireWire)
		{
			tipX = mvPlayerPos.x + (mvHookPos.x - mvPlayerPos.x) * mfWireExtendRatio;
			tipY = mvPlayerPos.y + (mvHookPos.y - mvPlayerPos.y) * mfWireExtendRatio;
		}

		DrawLine(
			(int)mvPlayerPos.x, (int)mvPlayerPos.y,
			(int)tipX, (int)tipY,
			GetColor(200, 255, 255), 4);
	}

	if (mpEnemyAnim != nullptr)
	{
		mpEnemyAnim->Draw(0.0f, 0.0f);
	}
	if (mpPlayerAnim != nullptr)
	{
		mpPlayerAnim->Draw(0.0f, 0.0f);
	}
}