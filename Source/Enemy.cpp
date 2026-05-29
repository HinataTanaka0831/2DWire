#include "Enemy.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Collision.h"
#include "Player.h"




Enemy::Enemy(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type)
	: Object2D(filename, initPos, allNum, numX, numY, interval, scale, type)
	, mvDirection(VGet(-1.0f, 0.0f, 0.0f))
{
	SetTag(Object2D::Enemy2D);

	mAnimController.RegisterAnimation(CharacterState::Idle,
		new TextureAnimation(filename, initPos, allNum, numX, numY, interval, scale, type));
	
	mAnimController.RegisterAnimation(CharacterState::Moving,
		new TextureAnimation("Resource/Enemy/Monster_Walk.png", initPos, 6, 6, 1, 8, scale, type));

	// 攻撃中はコマ送りを2倍速にし、攻撃の激しさを視覚的に演出
	mAnimController.RegisterAnimation(CharacterState::Attacking,
		new TextureAnimation("Resource/Enemy/Monster_Walk.png", initPos, 6, 6, 1, 4, scale, type));

	mAnimController.ChangeState(CharacterState::Idle);
}


Enemy::~Enemy()
{
	
}

void Enemy::Update()
{
	Move();
	Calcdamage();

	if (mnHp <= 0) {
		mAnimController.ChangeState(CharacterState::Dead);
	} 
	else {
		auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
		Player* pPlayer = dynamic_cast<Player*>(pTarget);
		
		if (pPlayer != nullptr) {
			VECTOR diff = VSub(pPlayer->GetPosition(), mvPosition);
			float distance = VSize(diff);

			// プレイヤーとの距離関係に基づき、アニメーションステートを動的に制御
			if (distance <= ATTACK_RANGE) {
				mAnimController.ChangeState(CharacterState::Attacking);
			}
			else if (distance <= SEARCH_RANGE) {
				mAnimController.ChangeState(CharacterState::Moving);
			}
			else {
				mAnimController.ChangeState(CharacterState::Idle);
			}
		}
		else {
			mAnimController.ChangeState(CharacterState::Idle);
		}
	}
	mAnimController.Update();

	Object2D::Update();
}

void Enemy::Draw()
{
	mAnimController.Draw(mvPosition.x, mvPosition.y, gCameraX, gCameraY);
}

void Enemy::Move()
{
	auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pTarget);

	if (pPlayer != nullptr)
	{
		VECTOR diff = VSub(pPlayer->GetPosition(), mvPosition);
		float distance = VSize(diff);

		if (distance <= ATTACK_RANGE)
		{
			// 立ち止まって攻撃動作に専念させるため、移動成分をゼロにリセット
			mvDirection.x = 0.0f;

			// 被弾間隔を制限し、毎フレーム多重ヒットによる即死バグを防ぐタイマー制御
			if (mnAttackCooldown > 0)
			{
				mnAttackCooldown--;
			}
			else
			{
				pPlayer->PDamage(1);
				mnAttackCooldown = ATTACK_INTERVAL;
			}
		}
		else if (distance <= SEARCH_RANGE)
		{
			// プレイヤーの居場所へ向けて追跡するための方向決定
			mvDirection.x = (diff.x > 0.0f) ? 1.0f : -1.0f;
			mvPosition.x += (float)MOVE_SPEED * mvDirection.x;
			
			// 接近した瞬間の一発目を即時被弾させず、プレイヤーがワイヤーで退避する隙を作る初期化
			mnAttackCooldown = ATTACK_INTERVAL;
		}
		else
		{
			// 索敵範囲から外れた場合は進行を停止し、Idle状態で待機
			mvDirection.x = 0.0f;
		}
	}
}

bool Enemy::IsScreenOut()
{
	return(mvPosition.x + mpTexture->GetSizeX() / 2) < 0.0f;
}

void Enemy::ChangeDamage(int damage)
{
	mnHp -= damage;
	if (mnHp <= 0)
	{
		SetDeleteFlag(true);
	}
}

void Enemy::Calcdamage()
{
	auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pTarget);

	// 動的キャスト失敗時のnullptr参照によるプログラムクラッシュを防止する安全対策
	if (pPlayer != nullptr)
	{
		if (Collision::CheckCircleToCircle(
			mvPosition,
			GetRadius(),
			pPlayer->GetPosition(),
			pPlayer->GetRadius())
			)
		{
			pPlayer->PDamage(1);
		}
	}
}
















