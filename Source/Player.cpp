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
{
	SetTag(Object2D::Player2D);

	mAnimController.RegisterAnimation(CharacterState::Idle,
		new TextureAnimation(filename, initPos, allNum, numX, numY, interval, scale, type));
	mAnimController.RegisterAnimation(CharacterState::Moving,
		new TextureAnimation("Resource/Player/anim_walk.png", initPos, 4, 4, 1, 7, 1.0f,type));
	mAnimController.RegisterAnimation(CharacterState::Attacking,
		new TextureAnimation("Resource/Player/anim_attack.png", initPos, 6, 3, 2, 5, 1.0f, type));

	mAnimController.ChangeState(CharacterState::Idle);
}

Player::~Player()
{

}

void Player::Update()
{
	HPGaugeUpdate();
	Move();
	Attack();

	if (mnAttackTimer > 0)
	{
		mnAttackTimer--;
		if (mnAttackTimer <= 0)
		{
			mbIsAttack = false;
		}
	}

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

	mAnimController.Update();

}

void Player::Draw()
{
	if (mbIsWireActive)
	{
		DrawLine((int)(mvPosition.x - gCameraX), (int)(mvPosition.y - gCameraY),
		         (int)(mvWireTargetPos.x - gCameraX), (int)(mvWireTargetPos.y - gCameraY),
		         GetColor(200, 255, 255), 3);
	}

	mAnimController.Draw(mvPosition.x, mvPosition.y, gCameraX, gCameraY);
	HPGaugeDraw();
}

void Player::Move()
{
	bool isLeftTrigger = MouseManager::IsLeftTrigger();
	bool isLeftRelease = MouseManager::IsLeftRelease();
	int mouseX = MouseManager::GetMouseX();
	int mouseY = MouseManager::GetMouseY();
	
	if (isLeftTrigger)
	{
		std::vector<Object2D*> targets = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::WireTarget2D);
		
		// 画面の上下スクロールに依存せず一貫したワールド座標でクリック判定を行うためカメラオフセットを加算
		float worldClickX = mouseX + gCameraX;
		float worldClickY = mouseY + gCameraY; 
		
		for (auto* obj : targets)
		{
			VECTOR tPos = obj->GetPosition();
			int sizeX = obj->GetSizeX();
			int sizeY = obj->GetSizeY();

			VECTOR objDis = VSub(tPos, mvPosition);
			float objdistance = VSize(objDis);

			// ゲームバランス上のワイヤー限界射程（950px）を超えている場合は接続処理をスルー
			if (objdistance >= 950.0f)
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
				// atan2(X, Y)により真下方向を基準角（0度）とするラジアン角を算出
				mfPendulumAngle = std::atan2(diffX, diffY); 
				
				// 接続前の直線速度の慣性をスイング運動へ引き継ぐため、接線速度から初期角速度を算出
				if (mfWireLength > 0.0f) {
					mfPendulumAngularVelocity = mfVelocityX / mfWireLength; 
				}
				break;
			}
		}
	}
	else if (isLeftRelease)
	{
		if (mbIsWireActive)
		{
			mbIsWireActive = false;
			// リリースの慣性を維持するため、振り子の接線方向速度を直線運動のXY速度ベクトルに逆変換
			float tangentVel = mfWireLength * mfPendulumAngularVelocity;
			mfVelocityX = tangentVel * std::cos(mfPendulumAngle);
			mfVelocityY = -tangentVel * std::sin(mfPendulumAngle);
		}
	}


	// このフレームの移動前の位置を覚えておく（ワイヤー中の衝突判定でも使う）
	VECTOR prevPosition = mvPosition;

if (mbIsWireActive)
{
	if (mfWireLength > 0.0f)
	{
		// 振り子の物理挙動を再現するため重力の接線成分から角加速度を算出
		mfAngularAcceleration = -(Gravity / mfWireLength) * std::sin(mfPendulumAngle);

		if (InputManager::CheckPressKey(KEY_INPUT_D)) mfAngularAcceleration += 0.0013f;
		if (InputManager::CheckPressKey(KEY_INPUT_A)) mfAngularAcceleration -= 0.0013f;

		// エネルギー保存による無限振動を防ぎ自然に減衰させるため、摩擦減衰率を適用
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
		// 滑り続ける操作感を防ぎ適度なコントロール性を保つため、非入力時は速度を減衰
		mfVelocityX *= 0.85f;
	}

	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		mfVelocityY = -15.0f;
	}

	if (InputManager::CheckDownKey(KEY_INPUT_G) && mnAttackCooldown == 0)
	{
		mbIsAttack = true;
		mHasHitThisAttack = false;
		mnAttackTimer = 30; // 6frame * 5interval = 30フレームの攻撃持続時間
		mnAttackCooldown = AttackInterval;
	}

	// 速度がゼロでないとき、向いている方向を記録する
	if (mfVelocityX < -0.1f) mbFacingLeft = true;
	else if (mfVelocityX > 0.1f) mbFacingLeft = false;

	// 高速移動によるマップ外への突き抜けや描画の破綻を防ぐための最高速度制限
	if (mfVelocityX > (float)MoveSpeed * 1.5f) mfVelocityX = (float)MoveSpeed * 1.5f;
	if (mfVelocityX < -(float)MoveSpeed * 1.5f) mfVelocityX = -(float)MoveSpeed * 1.5f;
	mvPosition.x += mfVelocityX;
	mvPosition.y += mfVelocityY;
}


// --- 敵との衝突判定：ぶつかっていたらその方向の移動だけブロックする ---
{
	// 通常移動は mVelocity、ワイヤー中は「前の位置との差」で動き方向を求める
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
			// 敵がプレイヤーのどちら側にいるか調べる
			float dirToEnemy = pEnemy->GetPosition().x - mvPosition.x;
			float dirToEnemyY = pEnemy->GetPosition().y - mvPosition.y;

			// ワイヤー中に敵の「上」から触れたときは、頭の上に乗せて横へ滑らせる
			bool isOnTopOfEnemy = mvPosition.y < pEnemy->GetPosition().y;
			if (mbIsWireActive && isOnTopOfEnemy)
			{
				const float slideSpeed = 5.0f;

				// 敵の頭の上にプレイヤーの足元が来る高さへ合わせる
				mvPosition.y = pEnemy->GetPosition().y - pEnemy->GetRadius() - GetRadius();

				// 振り子の動き方向を優先し、止まっているときは左右どちらにいるかで決める
				float slideDir = (mvPosition.x <= pEnemy->GetPosition().x) ? -1.0f : 1.0f;
				if (std::abs(moveX) > 0.05f)
				{
					slideDir = (moveX > 0.0f) ? 1.0f : -1.0f;
				}
				mvPosition.x += slideDir * slideSpeed;

				if (slideDir < 0.0f) mbFacingLeft = true;
				else mbFacingLeft = false;

				// ずらした位置に合わせて振り子の角度と長さを更新する
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

			// 敵のほうへ向かって動いていたら、その方向だけ止める
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
				if (!mbIsWireActive)
				{
					mfVelocityY = 0.0f;
				}
				blocked = true;
			}

			// ワイヤー中に止まった場合は、振り子の角度と速さを位置に合わせ直す
			if (blocked && mbIsWireActive && mfWireLength > 0.0f)
			{
				float diffX = mvPosition.x - mvWireTargetPos.x;
				float diffY = mvPosition.y - mvWireTargetPos.y;
				mfWireLength = std::sqrt(diffX * diffX + diffY * diffY);
				mfPendulumAngle = std::atan2(diffX, diffY);
				mfPendulumAngularVelocity *= 0.3f;
			}

				if (mnAttackCooldown <= 0)
				{
					PDamage(1); // 円衝突時は1ダメージ与える
					mnAttackCooldown = AttackInterval;
				}


			break;
		}
	}
}

	// 画面外（底面）への落下によるゲーム進行停止バグを防止するための境界クランプ
	if (mvPosition.y > 1000.0f)
	{
		mvPosition.y = 1000.0f;

		if (mbIsWireActive)
		{
			// 地面接地時のワイヤーのたわみによる物理挙動の破綻を防ぐため、強制的にワイヤー長をクランプ
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

void Player::Attack()
{
	// クールダウンは「次の攻撃を始める間隔」だけ減らす
	// ここで return すると、攻撃中の当たり判定まで止まってしまうので分けて扱う
	if (mnAttackCooldown > 0)
	{
		mnAttackCooldown--;
	}

	// 攻撃中で、まだこの一振りで当たっていないときだけ判定する
	if (!mbIsAttack || mHasHitThisAttack)
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

		// 攻撃判定の箱の大きさ（プレイヤーのすぐ前＋少し自分側にもかぶせる）
		const float attackWidth = 100.0f;
		const float attackHeight = (float)GetSizeY() * 0.7f;
		const float playerHalfWidth = (float)GetSizeX() * 0.5f;

		// 向いている方向のすぐ前に箱の中心を置く
		float attackCenterX = mvPosition.x;
		if (mbFacingLeft)
		{
			attackCenterX -= playerHalfWidth * 0.5f + attackWidth * 0.05f;
		}
		else
		{
			attackCenterX += playerHalfWidth * 0.5f + attackWidth * 0.05f;
		}

		// 中心 → 左上へ直すときは必ず「半分（0.5）」を使う
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
			mHasHitThisAttack = true;
			break;
		}
	}
}

void Player::HPGaugeDraw()
{
	int gaugeX = (int)(mvPosition.x - gCameraX) - 110;
	int gaugeY = (int)(mvPosition.y - gCameraY) - 160;

	DrawBox(gaugeX, gaugeY, gaugeX + mnWidth, gaugeY + mnGaugeHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + mnDamageWidth, gaugeY + mnGaugeHeight, GetColor(255, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + mnGaugeWidth, gaugeY + mnGaugeHeight, GetColor(51, 204, 51), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + mnWidth, gaugeY + mnGaugeHeight, GetColor(255, 255, 255), FALSE);
}

void Player::HPGaugeUpdate()
{
	mnHPGaugeTimer++;

	if (mnHPGaugeTimer >= GaugeFrame)
	{
		
		if (mnDisplayDamage > mnHP)
		{

			// 被弾演出としてダメージ分の赤ゲージを滑らかに追従させ、被弾の実感を与える
			mnDisplayDamage--;
			if (mnDisplayDamage < mnHP)
			{
				mnDisplayDamage = mnHP;

			}

		}

		mnHPGaugeTimer = 0;

	}


	int display = mnDisplayDamage;
	// 描画サイズがマイナス値になることでゲージの描画が反転・破綻するのを防ぐ防御処理
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

void Player::PDamage(int damage)
{
	mnHP -= damage;
	if (mnHP <= 0)
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAMEOVER);
	}
}


	
