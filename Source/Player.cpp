#include "Player.h"
#include "Collision.h"
#include "HomingBullet.h"
#include "Bullet.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "Utility.h"
#include <cmath>
#include "InputManager.h"
#include "Enemy.h"
#include "MouseManager.h"

Player::Player(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type)
	: Object2D(filename, initPos, allNum, numX, numY, interval, scale, type)
	, mnDisplayDamage(mnMaxHP)
	, mvWireTargetPos(VGet(0.0f, 0.0f, 0.0f))
{
	SetTag(Object2D::Player2D);

	mAnimController.RegisterAnimation(CharacterState::Idle,
		std::make_unique<TextureAnimation>(filename, initPos, allNum, numX, numY, interval, scale, type));
	mAnimController.RegisterAnimation(CharacterState::Moving,
		std::make_unique<TextureAnimation>("Resource/Player/anim_walk.png", initPos, 4, 4, 1, 7, 1.0f, type));
	mAnimController.RegisterAnimation(CharacterState::Attacking,
		std::make_unique<TextureAnimation>("Resource/Player/anim_attack.png", initPos, 6, 3, 2, 5, 1.0f, type));

	mAnimController.ChangeState(CharacterState::Idle);
}

Player::~Player()
{
}

// 毎フレームの物理挙動、入力検知、攻撃・HPゲージ更新
// 入力: なし / 出力: なし / 副作用: 座標・速度・ステート・HPの更新
void Player::Update()
{
	HPGaugeUpdate();
	Move();
	Attack();

	// 攻撃アニメーションの持続時間管理
	if (mnAttackAnimationTimer > 0)
	{
		mnAttackAnimationTimer--;
		if (mnAttackAnimationTimer <= 0)
		{
			mbIsAttack = false;
		}
	}

	// 状態優先度（死亡 ＞ 攻撃 ＞ 移動/ワイヤー ＞ 待機）に従ってアニメーションを決定
	if (mnHP <= 0) {
		mAnimController.ChangeState(CharacterState::Dead);
	}
	else if (mbIsAttack)
	{
		mAnimController.ChangeState(CharacterState::Attacking);
	}
	else if (std::abs(mfVelocityX) > 0.5f || mbIsWireActive) {
		mAnimController.ChangeState(CharacterState::Moving);
	}
	else {
		mAnimController.ChangeState(CharacterState::Idle);
	}

	mAnimController.Update(mvPosition.x, mvPosition.y);
}

// ワイヤーライン、プレイヤースプライト、HPゲージの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Player::Draw()
{
	// ワイヤー係留中はプレイヤーとターゲット間にラインを描画
	if (mbIsWireActive)
	{
		DrawLine((int)(mvPosition.x - gCameraX), (int)(mvPosition.y - gCameraY),
		         (int)(mvWireTargetPos.x - gCameraX), (int)(mvWireTargetPos.y - gCameraY),
		         GetColor(200, 255, 255), 3);
	}

	mAnimController.Draw(gCameraX, gCameraY);
	HPGaugeDraw();
}

// キー/マウス入力に基づく歩行・ジャンプ・ワイヤー振り子運動物理演算
// 入力: なし / 出力: なし / 副作用: 座標、速度、振り子角度、ワイヤー状態の更新
void Player::Move()
{
	//bool isLeftTrigger = MouseManager::IsLeftTrigger();
	//bool isLeftRelease = MouseManager::IsLeftRelease();
	bool isLeftTrigger = MouseManager::CheckTriggerMouseClick(MOUSE_INPUT_LEFT);
	bool isLeftRelease = MouseManager::CheckReleaseMouseClick(MOUSE_INPUT_LEFT);
	int mouseX = MouseManager::GetMouseX();
	int mouseY = MouseManager::GetMouseY();
	
	// クリックしたターゲットへのワイヤー射出判定
	if (isLeftTrigger)
	{
		std::vector<Object2D*> targets = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::WireTarget2D);
		
		// スクロールに依存せず判定するためスクリーン座標をワールド座標へ変換
		float worldClickX = mouseX + gCameraX;
		float worldClickY = mouseY + gCameraY; 
		
		for (auto* obj : targets)
		{
			VECTOR tPos = obj->GetPosition();
			int sizeX = obj->GetSizeX();
			int sizeY = obj->GetSizeY();

			VECTOR objDis = VSub(tPos, mvPosition);
			float objDistance = VSize(objDis);

			// 無制限な長距離移動を防ぐための射程距離制限(950px)
			if (objDistance >= 950.0f)
			{
				mbIsWireActive = false;
				continue; 
			}
			if (worldClickX >= tPos.x - sizeX / 2.0f && worldClickX <= tPos.x + sizeX / 2.0f &&
				worldClickY >= tPos.y - sizeY / 2.0f && worldClickY <= tPos.y + sizeY / 2.0f)
			{
				mbIsWireActive = true;
				mvWireTargetPos = VGet(worldClickX, worldClickY, 0.0f);
				
				float diffX = mvPosition.x - mvWireTargetPos.x;
				float diffY = mvPosition.y - mvWireTargetPos.y;
				mfWireLength = std::sqrt(diffX * diffX + diffY * diffY);
				// 真下方向を基準(0rad)とする振り子角度を算出
				mfPendulumAngle = std::atan2(diffX, diffY); 
				
				// ワイヤー接続前の直線速度の慣性を初期角速度へ変換
				if (mfWireLength > 0.0f) {
					mfPendulumAngularVelocity = mfVelocityX / mfWireLength; 
				}
				break;
			}
		}
	}
	else if (isLeftRelease)
	{
		// ワイヤー解放時に振り子の接線速度を直線速度ベクトルに射影
		if (mbIsWireActive)
		{
			mbIsWireActive = false;
			float tangentVel = mfWireLength * mfPendulumAngularVelocity;
			mfVelocityX = tangentVel * std::cos(mfPendulumAngle);
			mfVelocityY = -tangentVel * std::sin(mfPendulumAngle);
		}
	}

	VECTOR prevPosition = mvPosition;

	if (mbIsWireActive)
	{
		if (mfWireLength > 0.0f)
		{
			// 重力の接線成分から単振り子の角加速度を計算
			mfAngularAcceleration = -(Gravity / mfWireLength) * std::sin(mfPendulumAngle);

			// 左右キー入力による空中ブランコ風の漕ぎ増し加速
			if (InputManager::CheckPressKey(KEY_INPUT_D)) mfAngularAcceleration += 0.0013f;
			if (InputManager::CheckPressKey(KEY_INPUT_A)) mfAngularAcceleration -= 0.0013f;

			// 無限振動防止のための空気抵抗減衰
			mfPendulumAngularVelocity *= 0.995f;
			mfPendulumAngularVelocity += mfAngularAcceleration;
			mfPendulumAngle += mfPendulumAngularVelocity;

			mvPosition.x = mvWireTargetPos.x + std::sin(mfPendulumAngle) * mfWireLength;
			mvPosition.y = mvWireTargetPos.y + std::cos(mfPendulumAngle) * mfWireLength;
		}
	}
	else
	{
		mfVelocityY += Gravity;

		if (InputManager::CheckPressKey(KEY_INPUT_D))
		{
			mfVelocityX += 0.5f;
		}
		else if (InputManager::CheckPressKey(KEY_INPUT_A))
		{
			mfVelocityX -= 0.5f;
		}
		else
		{
			// 操作性向上のためキー非入力時は滑りを減衰
			mfVelocityX *= 0.85f;
		}

		if (!mbIsJump)
		{
			if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
			{
				mbIsJump = true;
				mfVelocityY -= 15.0f;
			}
		}

		if (InputManager::CheckDownKey(KEY_INPUT_G) && mnAttackCooldown == 0)
		{
			mbIsAttack = true;
			mbHasHitThisAttack = false;
			mnAttackAnimationTimer = 30;
			mnAttackCooldown = AttackInterval;
		}

		if (mfVelocityX < -0.1f) mbFacingLeft = true;
		else if (mfVelocityX > 0.1f) mbFacingLeft = false;

		// 壁抜けや描画破綻を防止するための最高水平速度制限
		if (mfVelocityX > (float)MoveSpeed * 1.5f) mfVelocityX = (float)MoveSpeed * 1.5f;
		if (mfVelocityX < -(float)MoveSpeed * 1.5f) mfVelocityX = -(float)MoveSpeed * 1.5f;
		mvPosition.x += mfVelocityX;
		mvPosition.y += mfVelocityY;
	}

	// 敵との接触衝突判定（頭上スライドおよび移動ブロック）
	{
		float moveX = mvPosition.x - prevPosition.x;
		float moveY = mvPosition.y - prevPosition.y;

		auto enemyList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);
		for (int i = 0; i < (int)enemyList.size(); i++)
		{
			Enemy* pEnemy = dynamic_cast<Enemy*>(enemyList[i]);
			if (pEnemy == nullptr) continue;

			if (Collision::CheckCircleToCircle(
				mvPosition,
				GetRadius(),
				pEnemy->GetPosition(),
				pEnemy->GetRadius()))
			{
				float dirToEnemy = pEnemy->GetPosition().x - mvPosition.x;
				float dirToEnemyY = pEnemy->GetPosition().y - mvPosition.y;

				// ワイヤー移動中に敵上部へ接触した際は頭上を滑走させる演出
				bool isOnTopOfEnemy = mvPosition.y < pEnemy->GetPosition().y;
				if (mbIsWireActive && isOnTopOfEnemy)
				{
					const float slideSpeed = 5.0f;
					mvPosition.y = pEnemy->GetPosition().y - pEnemy->GetRadius() - GetRadius();

					float slideDir = (mvPosition.x <= pEnemy->GetPosition().x) ? -1.0f : 1.0f;
					if (std::abs(moveX) > 0.05f)
					{
						slideDir = (moveX > 0.0f) ? 1.0f : -1.0f;
					}
					mvPosition.x += slideDir * slideSpeed;

					mbFacingLeft = (slideDir < 0.0f);

					// 座標補正に合わせて振り子長と角度を再同期
					if (mfWireLength > 0.0f)
					{
						float diffX = mvPosition.x - mvWireTargetPos.x;
						float diffY = mvPosition.y - mvWireTargetPos.y;
						mfWireLength = std::sqrt(diffX * diffX + diffY * diffY);
						mfPendulumAngle = std::atan2(diffX, diffY);
					}
					break;
				}

				bool blocked = false;

				if ((dirToEnemy > 0.0f && moveX > 0.0f) ||
					(dirToEnemy < 0.0f && moveX < 0.0f))
				{
					mvPosition.x = prevPosition.x;
					if (!mbIsWireActive)
					{
						mfVelocityX = 0.0f;
					}
					blocked = true;
				}

				if ((dirToEnemyY > 0.0f && moveY > 0.0f) ||
					(dirToEnemyY < 0.0f && moveY < 0.0f))
				{
					mvPosition.y = prevPosition.y;
					mbIsJump = false;
					if (!mbIsWireActive)
					{
						mfVelocityY = 0.0f;
					}
					blocked = true;
				}

				if (blocked && mbIsWireActive && mfWireLength > 0.0f)
				{
					float diffX = mvPosition.x - mvWireTargetPos.x;
					float diffY = mvPosition.y - mvWireTargetPos.y;
					mfWireLength = std::sqrt(diffX * diffX + diffY * diffY);
					mfPendulumAngle = std::atan2(diffX, diffY);
					mfPendulumAngularVelocity *= 0.3f;
				}

				if (mnDamageCooldown <= 0)
				{
					PDamage(1);
					mnDamageCooldown = DamageInterval;
				}

				break;
			}
		}
	}

	// 画面外（落下死領域）への脱落防止および地面境界での物理クランプ
	if (mvPosition.y > 1000.0f)
	{
		mvPosition.y = 1000.0f;
		mbIsJump = false;

		if (mbIsWireActive)
		{
			// 接地時のワイヤーたわみ破綻を防ぐため固定長と角度を再同期
			float diffX = mvPosition.x - mvWireTargetPos.x;
			float diffY = mvPosition.y - mvWireTargetPos.y;
			mfWireLength = std::sqrt(diffX * diffX + diffY * diffY);
			mfPendulumAngle = std::atan2(diffX, diffY);
		}
		else
		{
			mfVelocityY = 0.0f;
		}
	}
}

// 近接攻撃の当たり判定生成と敵へのダメージ適用
// 入力: なし / 出力: なし / 副作用: 攻撃判定Rectの計算と敵へのEDamage呼び出し
void Player::Attack()
{
	if (mnAttackCooldown > 0)
	{
		mnAttackCooldown--;
	}

	// 攻撃アクション中かつ未ヒット時のみ判定（多重ヒット防止）
	if (!mbIsAttack || mbHasHitThisAttack)
	{
		return;
	}

	auto enemyList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);
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
		float attackCenterX = mvPosition.x;
		if (mbFacingLeft)
		{
			attackCenterX -= playerHalfWidth * 0.5f + attackWidth * 0.05f;
		}
		else
		{
			attackCenterX += playerHalfWidth * 0.5f + attackWidth * 0.05f;
		}

		VECTOR attackLeftTop = VGet(
			attackCenterX - attackWidth * 0.5f,
			mvPosition.y - attackHeight * 0.5f,
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
			mbHasHitThisAttack = true;
			break;
		}
	}
}

// プレイヤー頭上にHPゲージを描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Player::HPGaugeDraw()
{
	int gaugeX = (int)(mvPosition.x - gCameraX) - 110;
	int gaugeY = (int)(mvPosition.y - gCameraY) - 160;

	DrawBox(gaugeX, gaugeY, gaugeX + mnWidth, gaugeY + mnGaugeHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + mnDamageWidth, gaugeY + mnGaugeHeight, GetColor(255, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + mnGaugeWidth, gaugeY + mnGaugeHeight, GetColor(51, 204, 51), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + mnWidth, gaugeY + mnGaugeHeight, GetColor(255, 255, 255), FALSE);
}

// 被弾時にダメージ減少分を滑らかに追従させる描画幅計算
// 入力: なし / 出力: なし / 副作用: mnDisplayDamage, mnDamageWidth, mnGaugeWidthの更新
void Player::HPGaugeUpdate()
{
	mnHPGaugeTimer++;

	if (mnHPGaugeTimer >= GaugeFrame)
	{
		if (mnDisplayDamage > mnHP)
		{
			// 被弾演出として赤ゲージを徐々に減少させて追従
			mnDisplayDamage--;
			if (mnDisplayDamage < mnHP)
			{
				mnDisplayDamage = mnHP;
			}
		}
		mnHPGaugeTimer = 0;
	}

	int display = mnDisplayDamage;
	if (display < 0 && display > mnMinWidth)
	{
		display = mnMinWidth;
	}

	int displayHp = mnHP;
	if (displayHp < 0 && displayHp > mnMinWidth)
	{
		displayHp = mnMinWidth;
	}

	mnDamageWidth = (int)((float)display / mnMaxHP * mnWidth); 
	mnGaugeWidth = (int)((float)displayHp / mnMaxHP * mnWidth);
}

// 被弾によるHP減算とゲームオーバー遷移トリガー
// 入力: damage(ダメージ量) / 出力: なし / 副作用: HP減算、HP<=0時のSCENE_GAMEOVER遷移要求
void Player::PDamage(int damage)
{
	mnHP -= damage;
	if (mnHP <= 0)
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAMEOVER);
	}
}