#include "Enemy.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Collision.h"
#include "Player.h"




Enemy::Enemy(std::string filename, VECTOR initPos, int t, int s)
	: Object2D(filename, initPos)
	,mvDirection(VGet(-1.0f, 0.0f, 0.0f))   // 最初はX軸の右方向へ動くようにしておく
	
{
	SetTag(Object2D::Enemy2D);

	Settype(t);

	SetEnemytype(s);

	// Idle: 6 frame monster walk animation
	mAnimController.RegisterAnimation(CharacterState::Idle,
		new TextureAnimation(filename, initPos, 1, 1, 1, 10, 1.0f));
	// Moving: same 6 frame animation
	mAnimController.RegisterAnimation(CharacterState::Moving,
		new TextureAnimation(filename, initPos, 1, 1, 1, 10, 1.0f));

	mAnimController.ChangeState(CharacterState::Moving);
}


Enemy::~Enemy()
{
	
}

void Enemy::Update()
{

	// 移動処理
	Move();

	Calcdamage();

	if (IsScreenOut())
	{
		SetDeleteFlag(true);
		
		//Initialize();
		
	}

	// 状態の判定（例として移動状態の切り替え）
	if (mnHp <= 0) {
		mAnimController.ChangeState(CharacterState::Dead);
	} else if (mvDirection.x != 0.0f || mvDirection.y != 0.0f) {
		mAnimController.ChangeState(CharacterState::Moving);
	} else {
		mAnimController.ChangeState(CharacterState::Idle);
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
	switch (type)
	{
	case 1:time = 400; break;
	case 2:time = 100; break;
	case 3:time = 600; break;
	case 4:time = 800; break;
	}

	if (GetNowCount() - StartTime > time)
	{
		// mvDirection.x の方向へ進むようにする
		mvPosition.x += (float)MOVE_SPEED * mvDirection.x;
	}

	// テクスチャに座標を伝える
	mpTexture->SetPosition(mvPosition);
}

bool Enemy::IsScreenOut()
{
	return(mvPosition.x + mpTexture->GetSizeX() / 2) < 0.0f;
}



//void Enemy::Initialize()
//{
//	int work = rand() % 8;
//
//
//	switch (work)
//	{
//	case 0:
//
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
//			"Resource/kusikatsu_gyu.png", 1,1
//		);
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
//			"Resource/torimomoniku.png", 2, 1
//		);
//
//		break;
//		
//	case 1:
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
//			"Resource/torimomoniku.png", 2,1
//		);
//		break;
//
//	case 2:
//
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
//			"Resource/hamukatsu.png", 3,1
//		);
//		break;
//
//	case 3:
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 140.0f, GetRand(380), 0.0f),
//			"Resource/Rebakatsu.png", 4,1
//		);
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
//			"Resource/hamukatsu.png", 3, 1
//		);
//
//		break;
//
//	case 4:
//		// 串揚げ塩
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
//			"Resource/kusikatsu_ton.png", 1, 2
//		);
//
//		break;
//	case 5:
//		// 串揚げ塩
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
//			"Resource/renkon.png", 2, 2
//		);
//		break;
//	case 6:
//		// 串揚げ塩
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 150.0f, GetRand(380), 0.0f),
//			"Resource/torisasami.png", 3, 2
//		);
//		// 串揚げ塩
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
//			"Resource/kusikatsu_ton.png", 1, 2
//		);
//
//		break;
//	case 7:
//		// 串揚げ塩
//		new Enemy(
//			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
//			"Resource/tamanegi.png", 4, 2
//		);
//		break;
//
//
//	}
//
//}


void Enemy::ChangeDamage(int damage)
{
	mnHp -= damage;

	if (mnHp <= 0)
	{
		SetDeleteFlag(true);


		//auto pPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);

		//Player* cPlayer = dynamic_cast<Player*>(pPlayer);

		//if (cPlayer != nullptr)
		//{

		//	if (enemytype == 1)
		//	{
		//		cPlayer->PDamage(1);
		//	}

	}

}



void Enemy::Calcdamage()
{
	auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);


	Player* pPlayer = dynamic_cast<Player*>(pTarget);


	if (pPlayer != nullptr)     // dynamic_cast をした後は必ずnullチェックを行う
	{
		// 当たり判定
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
















