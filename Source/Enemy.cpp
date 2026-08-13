#include "Enemy.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Collision.h"
#include "Player.h"

static bool gCurrentEnemyFlip = false;
extern bool gEnemyReverseX; // Global flag for enemy flip direction


Enemy::Enemy(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type)
	: Object2D(filename, initPos, allNum, numX, numY, interval, scale, type)
	, mvDirection(VGet(-1.0f, 0.0f, 0.0f))
	, displayDamage(maxHp)
{
	SetTag(Object2D::Enemy2D);

	mAnimController.RegisterAnimation(CharacterState::Idle,
		new TextureAnimation(filename, initPos, allNum, numX, numY, interval, scale, type));
	
	mAnimController.RegisterAnimation(CharacterState::Moving,
		new TextureAnimation("Resource/Enemy/anim_monster01walk.png", initPos, 6, 6, 1, 8, scale, type));

	// 攻撃中はコマ送りを2倍速にし、攻撃の激しさを視覚的に演出
	mAnimController.RegisterAnimation(CharacterState::Attacking,
		new TextureAnimation("Resource/Enemy/anim_monster01attack.png", initPos, 4, 4, 1, 10, scale,type));

	mAnimController.ChangeState(CharacterState::Idle);
}


Enemy::~Enemy()
{
	
}

void Enemy::Update()
{
	HPGaugeUpdate();
	Move();
	Calcdamage();

	if (Hp <= 0) {
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
	HPGaugeDraw();
}

void Enemy::Move()
{
	auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pTarget);

	if (pPlayer != nullptr)
	{
		VECTOR diff = VSub(pPlayer->GetPosition(), mvPosition);
		float distance = VSize(diff);

		mnAttackCooldown--;

		if (distance <= ATTACK_RANGE)
		{
			// 立ち止まって攻撃動作に専念させるため、移動成分をゼロにリセット
			mvDirection.x = 0.0f;

			if (mnAttackCooldown <= 0)
			{
				pPlayer->PDamage(5); // 1ヒットあたり5ダメージ与える (最大10回耐えられる)
				mnAttackCooldown = ATTACK_INTERVAL;
			}
		}
		else if (distance <= SEARCH_RANGE)
		{
			// プレイヤーの居場所へ向けて追跡するための方向決定
			mvDirection.x = (diff.x > 0.0f) ? 1.0f : -1.0f;
			mvPosition.x += (float)MOVE_SPEED * mvDirection.x;
			
			// この敵だけの向きで、この敵のアニメーションだけ反転
			mAnimController.SetEnemyReverse(mvDirection.x > 0.0f);

			// 接近した瞬間の一発目を即時被弾させず、プレイヤーがワイヤーで退避する隙を作る初期化
			if (mnAttackCooldown <= 0)
			{
				mnAttackCooldown = ATTACK_INTERVAL;
			}
		}
		else
		{
			// 索敵範囲から外れた場合は進行を停止し、Idle状態で待機
			mvDirection.x = 0.0f;
			mAnimController.SetEnemyReverse(false);
		}
	}
}

bool Enemy::IsScreenOut()
{
	return(mvPosition.x + mpTexture->GetSizeX() / 2) < 0.0f;
}

void Enemy::EDamage(int damage)
{
	Hp -= damage;
	if (Hp <= 0)
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
			if (mnAttackCooldown <= 0)
			{
				pPlayer->PDamage(1); // 円衝突時は1ダメージ与える
				mnAttackCooldown = ATTACK_INTERVAL;
			}
		}
	}
}


void Enemy::HPGaugeDraw()
{
	int gaugeX = (int)(mvPosition.x - gCameraX) - 110;
	int gaugeY = (int)(mvPosition.y - gCameraY) - 190;

	DrawBox(gaugeX, gaugeY, gaugeX + width, gaugeY + gaugeHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + damageWidth, gaugeY + gaugeHeight, GetColor(255, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(51, 204, 51), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + width, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);
}

void Enemy::HPGaugeUpdate()
{
	Timer++;

	if (Timer >= Gauge_Frame)
	{

		if (displayDamage > Hp)
		{

			// 被弾演出としてダメージ分の赤ゲージを滑らかに追従させ、被弾の実感を与える
			displayDamage--;

			if (displayDamage < Hp)
			{
				displayDamage = Hp;

			}

		}

		Timer = 0;

	}


	int display = displayDamage;
	// 描画サイズがマイナス値になることでゲージの描画が反転・破綻するのを防ぐ防御処理
	if (display < 0 && display > minWidth)
	{
		display = minWidth;
	}

	int displayHp = Hp;
	if (displayHp < 0 && displayHp > minWidth)
	{
		displayHp = minWidth;
	}

	damageWidth = (int)((float)display / maxHp * width);
	gaugeWidth = (int)((float)displayHp / maxHp * width);
}














