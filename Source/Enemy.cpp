#include "Enemy.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Collision.h"
#include "Player.h"
#include "TextureAnimation.h"


Enemy::Enemy(std::string fileName, VECTOR initPosition, int allNum, int numX, int numY, int interval, float scale, bool isPlayer)
	: Object2D(fileName, initPosition, allNum, numX, numY, interval, scale, isPlayer)
	, MoveSpeed(3)
	, m_maxHp(100)
	, m_hp(m_maxHp)
	, m_gaugeWidth(250)
	, m_gaugeHeight(20)
	, m_nowHpGaugeWidth(0)
	, m_damageBarWidth(0)
	, m_minWidth(0)
	, m_damage(m_maxHp)
	, m_hpGaugeTimer(0)
	, GaugeFrame(3.0f)
	, m_hitFlashTimer(0)
	, m_attackAnimationTimer(0)
	, m_deadAnimationTimer(0)
	, m_attackCooldown(0)
	, HitFlashDuration(60)
	, HitFlashInterval(6)
	, m_searchRange(700.0f)
	, m_attackRange(230.0f)
	, m_attackInterval(40)
	, m_isAttack(false)
	, m_isDead(false)
	, m_isCollidingWithPlayer(false)

{
	m_direction = VGet(-1.0f, 0.0f, 0.0f);
	SetTag(Object2D::Enemy2D);

	// 敵の各状態アニメーションを初期化
	m_animController.RegisterAnimation(CharacterState::Idle,
		new TextureAnimation(fileName, initPosition, allNum, numX, numY, interval, scale, isPlayer));
	
	m_animController.RegisterAnimation(CharacterState::Moving,
		new TextureAnimation("Resource/Enemy/enemy_monster01_walk.png", initPosition, 6, 6, 1, 8, scale, isPlayer));

	m_animController.RegisterAnimation(CharacterState::Attacking,
		new TextureAnimation("Resource/Enemy/enemy_monster01_attack.png", initPosition, 4, 4, 1, 10, scale, isPlayer));

	m_animController.RegisterAnimation(CharacterState::Dead,
		new TextureAnimation("Resource/Enemy/enemy_monster01_dead.png", VGet(1000.0f, 1000.0f, 0.0f), 5, 5, 1, 8, scale, isPlayer));

	m_animController.ChangeState(CharacterState::Idle);
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	HPGaugeUpdate();
	Move();

	if (m_hitFlashTimer > 0)
	{
		m_hitFlashTimer--;
	}

	if (m_attackAnimationTimer > 0)
	{

		m_attackAnimationTimer--;

		if (m_attackAnimationTimer <= 0)
		{
			m_isAttack = false;
		}
	}


	if (m_isDead) {
		m_animController.ChangeState(CharacterState::Dead);

		if (m_animController.EndAnimation())
		{
			// 死亡アニメーションが完了した場合、削除フラグを立てる
			SetDeleteFlag(true);
		}

	} 
	else {
		auto pTarget = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
		Player* pPlayer = dynamic_cast<Player*>(pTarget);
		
		if (pPlayer != nullptr) {
			VECTOR diff = VSub(pPlayer->GetPosition(), m_position);
			float distance = VSize(diff);

			// プレイヤーとの距離閾値に応じてアニメーション状態を自動遷移
			if (m_isAttack) {
				m_animController.ChangeState(CharacterState::Attacking);
			}
			else if (distance <= m_searchRange) {
				m_animController.ChangeState(CharacterState::Moving);
			}
			else {
				m_animController.ChangeState(CharacterState::Idle);
			}
		}
		else {
			m_animController.ChangeState(CharacterState::Idle);
		}
	}

	m_animController.Update(m_position.x, m_position.y);
}

void Enemy::Draw()
{
	// 被弾フィードバックとして一定周期で赤色点滅表示
	if (m_hitFlashTimer > 0 && ((m_hitFlashTimer / HitFlashInterval) % 2 == 0))
	{
		SetDrawBright(255, 96, 96);
	}

	m_animController.Draw(g_cameraX, g_cameraY);

	SetDrawBright(255, 255, 255);

	HPGaugeDraw();
}

void Enemy::Move()
{
	auto pTarget = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pTarget);

	if (pPlayer != nullptr)
	{
		m_isCollidingWithPlayer = Collision::CheckCircleToCircle(
			m_position, GetRadius(),
			pPlayer->GetPosition(), pPlayer->GetRadius());

		VECTOR diff = VSub(pPlayer->GetPosition(), m_position);
		float distance = VSize(diff);

		m_attackCooldown--;

		if (distance <= m_attackRange)
		{
			m_isAttack = true;
			m_attackAnimationTimer = 40;
			m_direction.x = 0.0f;

			// クールダウン満了時にプレイヤーへダメージ適用
			if (m_attackCooldown <= 0)
			{
				pPlayer->Damage(5);
				m_attackCooldown = m_attackInterval;
			}
		}
		else if (distance <= m_searchRange)
		{
			// プレイヤーの方向へ追従移動
			m_direction.x = (diff.x > 0.0f) ? 1.0f : -1.0f;
			m_position.x += (float)MoveSpeed * m_direction.x;
			m_animController.SetEnemyReverse(m_direction.x > 0.0f);

			// 接近直後の初撃猶予時間を与え、プレイヤーの回避猶予を確保
			if (m_attackCooldown <= 0)
			{
				m_attackCooldown = m_attackInterval;
			}
		}
		else
		{
			m_direction.x = 0.0f;
		}
	}

	// 敵同士の物理的なめり込み・重なりを解消
	ResolveEnemyOverlap();
}


void Enemy::Damage(int damage)
{
	if (m_hp > 0)
	{
		m_hp -= damage;
		m_hitFlashTimer = HitFlashDuration;
	}

	if (m_hp <= 0 && !m_isDead)
	{
		m_isDead = true;
	}

}

void Enemy::ResolveEnemyOverlap()
{
	auto enemyList = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);

	for (int i = 0; i < (int)enemyList.size(); i++)
	{
		Enemy* pOtherEnemy = dynamic_cast<Enemy*>(enemyList[i]);
		if (pOtherEnemy == nullptr || pOtherEnemy == this)
		{
			continue;
		}

		VECTOR diff = VSub(m_position, pOtherEnemy->GetPosition());
		float distance = VSize(diff);
		float minDistance = GetRadius() + pOtherEnemy->GetRadius();

		if (distance >= minDistance)
		{
			continue;
		}

		// 同一座標時のゼロ除算を回避するため微小オフセットを付与
		if (distance <= 0.001f)
		{
			m_position.x += (this < pOtherEnemy) ? -1.5f : 1.5f;
			continue;
		}

		float overlap = minDistance - distance;
		VECTOR pushDir = VNorm(diff);

		// 重なり量の半分を互いに押し出し自然に分離
		m_position.x += pushDir.x * (overlap * 0.5f);
		m_position.y += pushDir.y * (overlap * 0.5f);
	}
}

void Enemy::HPGaugeDraw()
{
	int gaugeX = (int)(m_position.x - g_cameraX) - 110;
	int gaugeY = (int)(m_position.y - g_cameraY) - 190;

	DrawBox(gaugeX, gaugeY, gaugeX + m_gaugeWidth, gaugeY + m_gaugeHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_damageBarWidth, gaugeY + m_gaugeHeight, GetColor(255, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_nowHpGaugeWidth, gaugeY + m_gaugeHeight, GetColor(51, 204, 51), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_gaugeWidth, gaugeY + m_gaugeHeight, GetColor(255, 255, 255), FALSE);
}

void Enemy::HPGaugeUpdate()
{
	m_hpGaugeTimer++;

	if (m_hpGaugeTimer >= GaugeFrame)
	{
		if (m_damage > m_hp)
		{
			// 被弾演出としてダメージ減少幅を段階的に追従
			m_damage--;
			if (m_damage < m_hp)
			{
				m_damage = m_hp;
			}
		}
		m_hpGaugeTimer = 0;
	}

	// 負値による反転描画バグを防ぐための境界ガード
	int displayDamage = m_damage;
	if (displayDamage < 0 && displayDamage > m_minWidth)
	{
		displayDamage = m_minWidth;
	}

	int displayHp = m_hp;
	if (displayHp < 0 && displayHp > m_minWidth)
	{
		displayHp = m_minWidth;
	}

	m_damageBarWidth = (int)((float)displayDamage / m_maxHp * m_gaugeWidth);
	m_nowHpGaugeWidth = (int)((float)displayHp / m_maxHp * m_gaugeWidth);
}