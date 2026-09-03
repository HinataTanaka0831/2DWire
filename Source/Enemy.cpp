#include "Enemy.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Collision.h"
#include "Player.h"
#include "Texture.h"

static bool gCurrentEnemyFlip = false;
extern bool gEnemyReverseX;

Enemy::Enemy(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type)
	: Object2D(filename, initPos, allNum, numX, numY, interval, scale, type)
{
	m_direction = VGet(-1.0f, 0.0f, 0.0f);
	SetTag(Object2D::Enemy2D);

	m_animController.RegisterAnimation(CharacterState::Idle,
		std::make_unique<TextureAnimation>(filename, initPos, allNum, numX, numY, interval, scale, type));
	
	m_animController.RegisterAnimation(CharacterState::Moving,
		std::make_unique<TextureAnimation>("Resource/Enemy/anim_monster01walk.png", initPos, 6, 6, 1, 8, scale, type));

	// 攻撃モーションの俊敏さを演出するため再生速度を調整して登録
	m_animController.RegisterAnimation(CharacterState::Attacking,
		std::make_unique<TextureAnimation>("Resource/Enemy/anim_monster01attack.png", initPos, 4, 4, 1, 10, scale, type));

	m_animController.ChangeState(CharacterState::Idle);
}

Enemy::~Enemy()
{
}

// 毎フレームのAI思考、移動、被弾点滅、HP更新
// 入力: なし / 出力: なし / 副作用: 座標・アニメーション・HPの更新
void Enemy::Update()
{
	HPGaugeUpdate();
	Move();
	if (m_hitFlashTimer > 0)
	{
		m_hitFlashTimer--;
	}

	if (m_hp <= 0) {
		m_animController.ChangeState(CharacterState::Dead);
	} 
	else {
		auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
		Player* pPlayer = dynamic_cast<Player*>(pTarget);
		
		if (pPlayer != nullptr) {
			VECTOR diff = VSub(pPlayer->GetPosition(), m_position);
			float distance = VSize(diff);

			// プレイヤーとの距離閾値に応じてアニメーション状態を自動遷移
			if (distance <= m_attackRange) {
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

// カメラ座標を加味した敵スプライト（被弾フラッシュ含む）とHPゲージの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Enemy::Draw()
{
	// 被弾フィードバックとして一定周期で赤色点滅表示
	if (m_hitFlashTimer > 0 && ((m_hitFlashTimer / HitFlashInterval) % 2 == 0))
	{
		SetDrawBright(255, 96, 96);
	}

	m_animController.Draw(gCameraX, gCameraY);
	SetDrawBright(255, 255, 255);
	HPGaugeDraw();
}

// プレイヤーとの距離に応じた追跡・攻撃AIルーチンおよび他敵との重なり分離
// 入力: なし / 出力: なし / 副作用: mvPosition, mvDirection, mnAttackCooldownの更新
void Enemy::Move()
{
	auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
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
			m_direction.x = 0.0f;

			// クールダウン満了時にプレイヤーへダメージ適用
			if (m_attackCooldown <= 0)
			{
				pPlayer->PDamage(5);
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

bool Enemy::IsScreenOut()
{
	return (m_position.x + m_texture->GetSizeX() / 2) < 0.0f;
}

// プレイヤーからの被弾処理と死亡時削除フラグ設定
// 入力: damage(ダメージ量) / 出力: なし / 副作用: hp減算、被弾点滅タイマー開始、SetDeleteFlag
void Enemy::EDamage(int damage)
{
	m_hp -= damage;
	m_hitFlashTimer = HitFlashDuration;
	if (m_hp <= 0)
	{
		SetDeleteFlag(true);
	}
}

// 敵同士が同じ座標に密集して固まるのを防ぐ斥力（位置補正）処理
// 入力: なし / 出力: なし / 副作用: mvPositionの微小補正
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

// 敵頭上にHPゲージ（現在値＋ダメージ演出バー）を描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Enemy::HPGaugeDraw()
{
	int gaugeX = (int)(m_position.x - gCameraX) - 110;
	int gaugeY = (int)(m_position.y - gCameraY) - 190;

	DrawBox(gaugeX, gaugeY, gaugeX + m_width, gaugeY + m_gaugeHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_damageWidth, gaugeY + m_gaugeHeight, GetColor(255, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_gaugeWidth, gaugeY + m_gaugeHeight, GetColor(51, 204, 51), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_width, gaugeY + m_gaugeHeight, GetColor(255, 255, 255), FALSE);
}

// 被弾時に減少分を滑らかに追従させるHP演出ゲージの幅計算
// 入力: なし / 出力: なし / 副作用: displayDamage, damageWidth, gaugeWidthの更新
void Enemy::HPGaugeUpdate()
{
	m_hpGaugeTimer++;

	if (m_hpGaugeTimer >= GaugeFrame)
	{
		if (m_displayDamage > m_hp)
		{
			// 被弾演出としてダメージ減少幅を段階的に追従
			m_displayDamage--;
			if (m_displayDamage < m_hp)
			{
				m_displayDamage = m_hp;
			}
		}
		m_hpGaugeTimer = 0;
	}

	int display = m_displayDamage;
	// 負値による反転描画バグを防ぐための境界ガード
	if (display < 0 && display > m_minWidth)
	{
		display = m_minWidth;
	}

	int displayHp = m_hp;
	if (displayHp < 0 && displayHp > m_minWidth)
	{
		displayHp = m_minWidth;
	}

	m_damageWidth = (int)((float)display / m_maxHP * m_width);
	m_gaugeWidth = (int)((float)displayHp / m_maxHP * m_width);
}