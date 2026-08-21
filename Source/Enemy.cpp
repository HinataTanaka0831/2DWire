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
	, displayDamage(maxHP)
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
	if (mnHitFlashTimer > 0)
	{
		mnHitFlashTimer--;
	}

	if (hp <= 0) {
		mAnimController.ChangeState(CharacterState::Dead);
	} 
	else {
		auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
		Player* pPlayer = dynamic_cast<Player*>(pTarget);
		
		if (pPlayer != nullptr) {
			VECTOR diff = VSub(pPlayer->GetPosition(), mvPosition);
			float distance = VSize(diff);

			// プレイヤーとの距離関係に基づき、アニメーションステートを動的に制御
			if (distance <= Attack_Range) {
				mAnimController.ChangeState(CharacterState::Attacking);
			}
			else if (distance <= Search_Range) {
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

}

void Enemy::Draw()
{
	// 被弾直後は赤く点滅させて、「今ダメージを受けた」と見てわかるようにする
	if (mnHitFlashTimer > 0 && ((mnHitFlashTimer / HitFlashInterval) % 2 == 0))
	{
		SetDrawBright(255, 96, 96);
	}

	mAnimController.Draw(mvPosition.x, mvPosition.y, gCameraX, gCameraY);
	SetDrawBright(255, 255, 255);
	HPGaugeDraw();
}

void Enemy::Move()
{
	auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pTarget);

	if (pPlayer != nullptr)
	{
		// プレイヤーと衝突していないときだけ移動する（衝突中はその場にとどまる）
		isCollidingWithPlayer = Collision::CheckCircleToCircle(
			mvPosition, GetRadius(),
			pPlayer->GetPosition(), pPlayer->GetRadius());

		VECTOR diff = VSub(pPlayer->GetPosition(), mvPosition);
		float distance = VSize(diff);

		mnAttackCooldown--;

		if (distance <= Attack_Range)
		{
			// 立ち止まって攻撃動作に専念させるため、移動成分をゼロにリセット
			mvDirection.x = 0.0f;

			if (mnAttackCooldown <= 0)
			{
				pPlayer->PDamage(5); // 1ヒットあたり5ダメージ与える
				mnAttackCooldown = Attack_Interval;
			}

		}
		else if (distance <= Search_Range)
		{
			// プレイヤーの居場所へ向けて追跡するための方向決定
			mvDirection.x = (diff.x > 0.0f) ? 1.0f : -1.0f;
			mvPosition.x += (float)MoveSpeed * mvDirection.x;
			// この敵だけの向きで、この敵のアニメーションだけ反転
			mAnimController.SetEnemyReverse(mvDirection.x > 0.0f);

			// 接近した瞬間の一発目を即時被弾させず、プレイヤーがワイヤーで退避する隙を作る初期化
			if (mnAttackCooldown <= 0)
			{
				mnAttackCooldown = Attack_Interval;
			}
		}
		else
		{
			// 索敵範囲から外れた場合は進行を停止し、Idle状態で待機
			mvDirection.x = 0.0f;
		}
	}

	// 最後に重なりだけをほどくことで、敵が同じ一点に固まり続けるのを防ぐ
	ResolveEnemyOverlap();
}

bool Enemy::IsScreenOut()
{
	return(mvPosition.x + mpTexture->GetSizeX() / 2) < 0.0f;
}

void Enemy::EDamage(int damage)
{
	hp -= damage;
	mnHitFlashTimer = HitFlashDuration;
	if (hp <= 0)
	{
		SetDeleteFlag(true);
	}
}

void Enemy::ResolveEnemyOverlap()
{
	auto enemyList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);

	for (int i = 0; i < (int)enemyList.size(); i++)
	{
		Enemy* pOtherEnemy = dynamic_cast<Enemy*>(enemyList[i]);
		if (pOtherEnemy == nullptr || pOtherEnemy == this)
		{
			continue;
		}

		VECTOR diff = VSub(mvPosition, pOtherEnemy->GetPosition());
		float distance = VSize(diff);
		float minDistance = GetRadius() + pOtherEnemy->GetRadius();

		if (distance >= minDistance)
		{
			continue;
		}

		// 完全に同じ座標だと向きが作れないので、左右どちらかに少しだけ逃がす
		if (distance <= 0.001f)
		{
			mvPosition.x += (this < pOtherEnemy) ? -1.5f : 1.5f;
			continue;
		}

		float overlap = minDistance - distance;
		VECTOR pushDir = VNorm(diff);

		// 半分だけ押し出しておくと、毎フレーム少しずつ自然にばらけやすい
		mvPosition.x += pushDir.x * (overlap * 0.5f);
		mvPosition.y += pushDir.y * (overlap * 0.5f);
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
	hpGaugeTimer++;

	if (hpGaugeTimer >= GaugeFrame)
	{

		if (displayDamage > hp)
		{

			// 被弾演出としてダメージ分の赤ゲージを滑らかに追従させ、被弾の実感を与える
			displayDamage--;

			if (displayDamage < hp)
			{
				displayDamage = hp;

			}

		}

		hpGaugeTimer = 0;

	}


	int display = displayDamage;
	// 描画サイズがマイナス値になることでゲージの描画が反転・破綻するのを防ぐ防御処理
	if (display < 0 && display > minWidth)
	{
		display = minWidth;
	}

	int displayHp = hp;
	if (displayHp < 0 && displayHp > minWidth)
	{
		displayHp = minWidth;
	}

	damageWidth = (int)((float)display / maxHP * width);
	gaugeWidth = (int)((float)displayHp / maxHP * width);
}














