#include "Player.h"
#include "Collision.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "Utility.h"
#include <cmath>
#include "InputManager.h"
#include "Enemy.h"

Player::Player(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type)
	: Object2D(filename, initPos, allNum, numX, numY, interval, scale, type)
{
	SetTag(Object2D::Player2D);

	m_animController.RegisterAnimation(CharacterState::Idle,
		std::make_unique<TextureAnimation>(filename, initPos, allNum, numX, numY, interval, scale, type));
	m_animController.RegisterAnimation(CharacterState::Moving,
		std::make_unique<TextureAnimation>("Resource/Player/player_walk.png", initPos, 4, 4, 1, 7, 1.0f, type));
	m_animController.RegisterAnimation(CharacterState::Attacking,
		std::make_unique<TextureAnimation>("Resource/Player/player_attack.png", initPos, 6, 3, 2, 5, 1.0f, type));

	m_animController.ChangeState(CharacterState::Idle);
}

Player::~Player()
{
}

void Player::Update()
{
	HPGaugeUpdate();
	Move();
	Attack();

	// 攻撃アニメーションの持続時間管理
	if (m_attackAnimationTimer > 0)
	{
		m_attackAnimationTimer--;
		if (m_attackAnimationTimer <= 0)
		{
			m_isAttack = false;
		}
	}

	// 状態優先度（死亡 ＞ 攻撃 ＞ 移動/ワイヤー ＞ 待機）に従ってアニメーションを決定
	if (m_hp <= 0) {
		m_animController.ChangeState(CharacterState::Dead);
	}
	else if (m_isAttack)
	{
		m_animController.ChangeState(CharacterState::Attacking);
	}
	else if (std::abs(m_velocityX) > 0.5f || m_isWireActive) {
		m_animController.ChangeState(CharacterState::Moving);
	}
	else {
		m_animController.ChangeState(CharacterState::Idle);
	}

	m_animController.Update(m_position.x, m_position.y);
}

void Player::Draw()
{
	// ワイヤー係留中はプレイヤーとターゲット間にラインを描画
	if (m_isWireActive)
	{
		DrawLine((int)(m_position.x - g_cameraX), (int)(m_position.y - g_cameraY),
		         (int)(m_wireTargetPos.x - g_cameraX), (int)(m_wireTargetPos.y - g_cameraY),
		         GetColor(200, 255, 255), 3);
	}

	m_animController.Draw(g_cameraX, g_cameraY);
	HPGaugeDraw();
}

void Player::Move()
{
	InputManager& input = InputManager::GetInstance();
	bool isLeftTrigger = input.CheckTriggerMouseClick(MOUSE_INPUT_LEFT);
	bool isLeftRelease = input.CheckReleaseMouseClick(MOUSE_INPUT_LEFT);
	float mouseX = input.GetMouseX();
	float mouseY = input.GetMouseY();
	
	// クリックしたターゲットへのワイヤー射出判定
	if (isLeftTrigger)
	{
		std::vector<Object2D*> targets = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::WireTarget2D);
		
		// スクロールに依存せず判定するためスクリーン座標をワールド座標へ変換
		float worldClickX = mouseX + g_cameraX;
		float worldClickY = mouseY + g_cameraY; 
		
		for (auto* obj : targets)
		{
			VECTOR tPos = obj->GetPosition();
			int sizeX = obj->GetSizeX();
			int sizeY = obj->GetSizeY();

			VECTOR objDis = VSub(tPos, m_position);
			float objDistance = VSize(objDis);

			// 無制限な長距離移動を防ぐための射程距離制限(950px)
			if (objDistance >= 950.0f)
			{
				m_isWireActive = false;
				continue; 
			}
			if (worldClickX >= tPos.x - sizeX / 2.0f && worldClickX <= tPos.x + sizeX / 2.0f &&
				worldClickY >= tPos.y - sizeY / 2.0f && worldClickY <= tPos.y + sizeY / 2.0f)
			{
				m_isWireActive = true;
				m_wireTargetPos = VGet(worldClickX, worldClickY, 0.0f);
				
				float diffX = m_position.x - m_wireTargetPos.x;
				float diffY = m_position.y - m_wireTargetPos.y;
				m_wireLength = std::sqrt(diffX * diffX + diffY * diffY);
				// 真下方向を基準(0rad)とする振り子角度を算出
				m_pendulumAngle = std::atan2(diffX, diffY); 
				
				// ワイヤー接続前の直線速度の慣性を初期角速度へ変換
				if (m_wireLength > 0.0f) {
					m_pendulumAngularVelocity = m_velocityX / m_wireLength; 
				}
				break;
			}
		}
	}
	else if (isLeftRelease)
	{
		// ワイヤー解放時に振り子の接線速度を直線速度ベクトルに射影
		if (m_isWireActive)
		{
			m_isWireActive = false;
			float tangentVelocity = m_wireLength * m_pendulumAngularVelocity;
			m_velocityX = tangentVelocity * std::cos(m_pendulumAngle);
			m_velocityY = -tangentVelocity * std::sin(m_pendulumAngle);
		}
	}

	VECTOR prevPosition = m_position;

	if (m_isWireActive)
	{
		if (m_wireLength > 0.0f)
		{
			// 重力の接線成分から単振り子の角加速度を計算
			m_angularAcceleration = -(m_gravity / m_wireLength) * std::sin(m_pendulumAngle);

			// 左右キー入力による空中ブランコ風の漕ぎ増し加速
			if (input.CheckPressKey(KEY_INPUT_D)) m_angularAcceleration += 0.0013f;
			if (input.CheckPressKey(KEY_INPUT_A)) m_angularAcceleration -= 0.0013f;

			// 無限振動防止のための空気抵抗減衰
			m_pendulumAngularVelocity *= 0.995f;
			m_pendulumAngularVelocity += m_angularAcceleration;
			m_pendulumAngle += m_pendulumAngularVelocity;

			m_position.x = m_wireTargetPos.x + std::sin(m_pendulumAngle) * m_wireLength;
			m_position.y = m_wireTargetPos.y + std::cos(m_pendulumAngle) * m_wireLength;
		}
	}
	else
	{
		m_velocityY += m_gravity;

		if (input.CheckPressKey(KEY_INPUT_D))
		{
			m_velocityX += 0.5f;
		}
		else if (input.CheckPressKey(KEY_INPUT_A))
		{
			m_velocityX -= 0.5f;
		}
		else
		{
			// 操作性向上のためキー非入力時は滑りを減衰
			m_velocityX *= 0.85f;
		}

		if (!m_isJump)
		{
			if (input.CheckDownKey(KEY_INPUT_SPACE))
			{
				m_isJump = true;
				m_velocityY -= 15.0f;
			}
		}

		if (input.CheckDownKey(KEY_INPUT_F) && m_attackCooldown == 0)
		{
			m_isAttack = true;
			m_hasHitThisAttack = false;
			m_attackAnimationTimer = 30;
			m_attackCooldown = AttackInterval;
		}

		if (m_velocityX < -0.1f) m_facingLeft = true;
		else if (m_velocityX > 0.1f) m_facingLeft = false;

		// 壁抜けや描画破綻を防止するための最高水平速度制限
		if (m_velocityX > (float)MoveSpeed * 1.5f) m_velocityX = (float)MoveSpeed * 1.5f;
		if (m_velocityX < -(float)MoveSpeed * 1.5f) m_velocityX = -(float)MoveSpeed * 1.5f;
		m_position.x += m_velocityX;
		m_position.y += m_velocityY;
	}

	// 敵との接触衝突判定（頭上スライドおよび移動ブロック）
	{
		float moveX = m_position.x - prevPosition.x;
		float moveY = m_position.y - prevPosition.y;

		auto enemyList = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);
		for (int i = 0; i < (int)enemyList.size(); i++)
		{
			Enemy* pEnemy = dynamic_cast<Enemy*>(enemyList[i]);
			if (pEnemy == nullptr) continue;

			if (Collision::CheckCircleToCircle(
				m_position,
				GetRadius(),
				pEnemy->GetPosition(),
				pEnemy->GetRadius()))
			{
				float dirToEnemy = pEnemy->GetPosition().x - m_position.x;
				float dirToEnemyY = pEnemy->GetPosition().y - m_position.y;

				// ワイヤー移動中に敵上部へ接触した際は頭上を滑走させる演出
				bool isOnTopOfEnemy = m_position.y < pEnemy->GetPosition().y;
				if (m_isWireActive && isOnTopOfEnemy)
				{
					const float slideSpeed = 5.0f;
					m_position.y = pEnemy->GetPosition().y - pEnemy->GetRadius() - GetRadius();

					float slideDir = (m_position.x <= pEnemy->GetPosition().x) ? -1.0f : 1.0f;
					if (std::abs(moveX) > 0.05f)
					{
						slideDir = (moveX > 0.0f) ? 1.0f : -1.0f;
					}
					m_position.x += slideDir * slideSpeed;

					m_facingLeft = (slideDir < 0.0f);

					// 座標補正に合わせて振り子長と角度を再同期
					if (m_wireLength > 0.0f)
					{
						float diffX = m_position.x - m_wireTargetPos.x;
						float diffY = m_position.y - m_wireTargetPos.y;
						m_wireLength = std::sqrt(diffX * diffX + diffY * diffY);
						m_pendulumAngle = std::atan2(diffX, diffY);
					}
					break;
				}

				bool blocked = false;

				if ((dirToEnemy > 0.0f && moveX > 0.0f) ||
					(dirToEnemy < 0.0f && moveX < 0.0f))
				{
					m_position.x = prevPosition.x;
					if (!m_isWireActive)
					{
						m_velocityX = 0.0f;
					}
					blocked = true;
				}

				if ((dirToEnemyY > 0.0f && moveY > 0.0f) ||
					(dirToEnemyY < 0.0f && moveY < 0.0f))
				{
					m_position.y = prevPosition.y;
					m_isJump = false;
					if (!m_isWireActive)
					{
						m_velocityY = 0.0f;
					}
					blocked = true;
				}

				if (blocked && m_isWireActive && m_wireLength > 0.0f)
				{
					float diffX = m_position.x - m_wireTargetPos.x;
					float diffY = m_position.y - m_wireTargetPos.y;
					m_wireLength = std::sqrt(diffX * diffX + diffY * diffY);
					m_pendulumAngle = std::atan2(diffX, diffY);
					m_pendulumAngularVelocity *= 0.3f;
				}

				if (m_damageCooldown <= 0)
				{
					PDamage(1);
					m_damageCooldown = DamageInterval;
				}

				break;
			}
		}
	}

	// 画面外（落下死領域）への脱落防止および地面境界での物理クランプ
	if (m_position.y > 1000.0f)
	{
		m_position.y = 1000.0f;
		m_isJump = false;

		if (m_isWireActive)
		{
			// 接地時のワイヤーたわみ破綻を防ぐため固定長と角度を再同期
			float diffX = m_position.x - m_wireTargetPos.x;
			float diffY = m_position.y - m_wireTargetPos.y;
			m_wireLength = std::sqrt(diffX * diffX + diffY * diffY);
			m_pendulumAngle = std::atan2(diffX, diffY);
		}
		else
		{
			m_velocityY = 0.0f;
		}
	}
}

void Player::Attack()
{
	if (m_attackCooldown > 0)
	{
		m_attackCooldown--;
	}

	// 攻撃アクション中かつ未ヒット時のみ判定（多重ヒット防止）
	if (!m_isAttack || m_hasHitThisAttack)
	{
		return;
	}

	auto enemyList = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);
	for (int i = 0; i < (int)enemyList.size(); i++)
	{
		Enemy* pEnemy = dynamic_cast<Enemy*>(enemyList[i]);
		if (pEnemy == nullptr)
		{
			continue;
		}

		const float attackWidth = 100.0f;
		const float attackHeight = (float)GetSizeY() * 0.7f;
		const float playerHalfWidth = (float)GetSizeX() * 0.5f;

		// プレイヤーの前方方向に攻撃判定矩形を生成
		float attackCenterX = m_position.x;
		if (m_facingLeft)
		{
			attackCenterX -= playerHalfWidth * 0.5f + attackWidth * 0.05f;
		}
		else
		{
			attackCenterX += playerHalfWidth * 0.5f + attackWidth * 0.05f;
		}

		VECTOR attackLeftTop = VGet(
			attackCenterX - attackWidth * 0.5f,
			m_position.y - attackHeight * 0.5f,
			0.0f
		);

		VECTOR enemyLeftTop = VGet(
			pEnemy->GetPosition().x - pEnemy->GetSizeX() * 0.5f,
			pEnemy->GetPosition().y - pEnemy->GetSizeY() * 0.5f,
			0.0f
		);

		if (Collision::CheckRectToRect(
			attackLeftTop,
			attackWidth,
			attackHeight,
			enemyLeftTop,
			(float)pEnemy->GetSizeX(),
			(float)pEnemy->GetSizeY()
		))
		{
			pEnemy->EDamage(10);
			m_hasHitThisAttack = true;
			break;
		}
	}
}

void Player::HPGaugeDraw()
{
	int gaugeX = (int)(m_position.x - g_cameraX) - 110;
	int gaugeY = (int)(m_position.y - g_cameraY) - 160;

	DrawBox(gaugeX, gaugeY, gaugeX + m_width, gaugeY + m_gaugeHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_damageWidth, gaugeY + m_gaugeHeight, GetColor(255, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_gaugeWidth, gaugeY + m_gaugeHeight, GetColor(51, 204, 51), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + m_width, gaugeY + m_gaugeHeight, GetColor(255, 255, 255), FALSE);
}

void Player::HPGaugeUpdate()
{
	m_hpGaugeTimer++;

	if (m_hpGaugeTimer >= GaugeFrame)
	{
		if (m_displayDamage > m_hp)
		{
			// 被弾演出として赤ゲージを徐々に減少させて追従
			m_displayDamage--;
			if (m_displayDamage < m_hp)
			{
				m_displayDamage = m_hp;
			}
		}
		m_hpGaugeTimer = 0;
	}

	int display = m_displayDamage;
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

void Player::PDamage(int damage)
{
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_isDead = true;
	}
}