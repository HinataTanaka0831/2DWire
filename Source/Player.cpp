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


Player::Player(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type)
	: Object2D(filename, initPos, allNum, numX, numY, interval, scale, type)
	, displayDamage(maxHp)
{
	SetTag(Object2D::Player2D);

	mAnimController.RegisterAnimation(CharacterState::Idle,
		new TextureAnimation(filename, initPos, allNum, numX, numY, interval, scale));
	mAnimController.RegisterAnimation(CharacterState::Moving,
		new TextureAnimation("Resource/Player/Player_Walk.png", initPos, 4, 4, 1, 8, 1.0f));
	mAnimController.RegisterAnimation(CharacterState::Attacking,
		new TextureAnimation("Resource/Player/Player_Attack.png", initPos, 9, 3, 3, 7, 1.0f));

	mAnimController.ChangeState(CharacterState::Idle);
}

Player::~Player()
{

}

void Player::Update()
{
	HPGaugeUpdate();
	Move();

	if (mnAttackTimer > 0)
	{
		mnAttackTimer--;
		if (mnAttackTimer <= 0)
		{
			mbIsAttack = false;
		}
	}

	if (Hp <= 0) {
		mAnimController.ChangeState(CharacterState::Dead);
	}
	else if (mbIsAttack)
	{
		mAnimController.ChangeState(CharacterState::Attacking);
	}
	else if (std::abs(mVelocityX) > 0.5f || mbIsWireActive) {
		mAnimController.ChangeState(CharacterState::Moving);
	}
	else {
		mAnimController.ChangeState(CharacterState::Idle);
	}
	mAnimController.Update();

	Object2D::Update();
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
	static int oldMouseInput = 0; 
	int mouseInput = GetMouseInput();
	int clickX, clickY;
	GetMousePoint(&clickX, &clickY);

	if ((mouseInput & MOUSE_INPUT_LEFT) && !(oldMouseInput & MOUSE_INPUT_LEFT))
	{
		std::vector<Object2D*> targets = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::WireTarget2D);
		
		// 画面の上下スクロールに依存せず一貫したワールド座標でクリック判定を行うためカメラオフセットを加算
		float worldClickX = clickX + gCameraX;
		float worldClickY = clickY + gCameraY; 
		
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
				mWireLength = std::sqrt(diffX * diffX + diffY * diffY);
				// atan2(X, Y)により真下方向を基準角（0度）とするラジアン角を算出
				mPendulumAngle = std::atan2(diffX, diffY); 
				
				// 接続前の直線速度の慣性をスイング運動へ引き継ぐため、接線速度から初期角速度を算出
				if (mWireLength > 0.0f) {
					mPendulumAngularVelocity = mVelocityX / mWireLength; 
				}
				break;
			}
		}
	}
	else if (!(mouseInput & MOUSE_INPUT_LEFT) && (oldMouseInput & MOUSE_INPUT_LEFT))
	{
		if (mbIsWireActive)
		{
			mbIsWireActive = false;
			// リリースの慣性を維持するため、振り子の接線方向速度を直線運動のXY速度ベクトルに逆変換
			float tangentVel = mWireLength * mPendulumAngularVelocity;
			mVelocityX = tangentVel * std::cos(mPendulumAngle);
			mVelocityY = -tangentVel * std::sin(mPendulumAngle);
		}
	}
	
	oldMouseInput = mouseInput;

	if (mbIsWireActive)
	{
		if (mWireLength > 0.0f)
		{
			// 振り子の物理挙動を再現するため重力の接線成分から角加速度を算出
			angularAcceleration = -(GRAVITY / mWireLength) * std::sin(mPendulumAngle);
			
			if (CheckHitKey(KEY_INPUT_D)) angularAcceleration += 0.0013f;
			if (CheckHitKey(KEY_INPUT_A)) angularAcceleration -= 0.0013f;

			// エネルギー保存による無限振動を防ぎ自然に減衰させるため、摩擦減衰率を適用
			mPendulumAngularVelocity *= 0.995f;
			
			mPendulumAngularVelocity += angularAcceleration;
			mPendulumAngle += mPendulumAngularVelocity;

			mvPosition.x = mvWireTargetPos.x + std::sin(mPendulumAngle) * mWireLength;
			mvPosition.y = mvWireTargetPos.y + std::cos(mPendulumAngle) * mWireLength;
		}
	}
	else
	{
		mVelocityY += GRAVITY; 

		if (CheckHitKey(KEY_INPUT_D))
		{
			mVelocityX += 0.5f; 
		}
		else if (CheckHitKey(KEY_INPUT_A))
		{
			mVelocityX -= 0.5f; 
		}
		else
		{
			// 滑り続ける操作感を防ぎ適度なコントロール性を保つため、非入力時は速度を減衰
			mVelocityX *= 0.85f; 
		}

		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			// 空中での多段ジャンプを防止するため、暫定の設置ライン（Y=1000以上）にいる場合のみジャンプを許容
			if (mvPosition.y >= 1000.0f) 
			{
				mVelocityY = -10.0f; 
			}
		}

		if (InputManager::CheckDownKey(KEY_INPUT_F))
		{
			mbIsAttack = true;
			mnAttackTimer = 63; // 9 frames * 7 interval = 63 frames

			auto pObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Enemy2D);
			Enemy* pEnemy = dynamic_cast<Enemy*>(pObj);

			if (pEnemy != nullptr)
			{
				if (Collision::CheckCircleToCircle(
					mvPosition,
					GetRadius(),
					pEnemy->GetPosition(),
					pEnemy->GetRadius()
				))
				{
					pEnemy->EDamage(10);
				}

			}
		}

		// 高速移動によるマップ外への突き抜けや描画の破綻を防ぐための最高速度制限
		if (mVelocityX > (float)MOVE_SPEED * 1.5f) mVelocityX = (float)MOVE_SPEED * 1.5f;
		if (mVelocityX < -(float)MOVE_SPEED * 1.5f) mVelocityX = -(float)MOVE_SPEED * 1.5f;
		
		mvPosition.x += mVelocityX;
		mvPosition.y += mVelocityY;
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
			mWireLength = std::sqrt(diffX * diffX + diffY * diffY);
			mPendulumAngle = std::atan2(diffX, diffY);
		}
		else
		{
			mVelocityY = 0.0f;
		}
	}

}

void Player::HPGaugeDraw()
{
	int gaugeX = (int)(mvPosition.x - gCameraX) - 110;
	int gaugeY = (int)(mvPosition.y - gCameraY) - 160;

	DrawBox(gaugeX, gaugeY, gaugeX + width, gaugeY + gaugeHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + damageWidth, gaugeY + gaugeHeight, GetColor(255, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(51, 204, 51), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + width, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);
}

void Player::HPGaugeUpdate()
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

void Player::PDamage(int damage)
{
	Hp -= damage;
	if (Hp <= 0)
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAMEOVER);
	}
}


	
