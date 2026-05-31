#include "Bullet.h"
#include "Collision.h"
#include "Enemy.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Player.h"



Bullet::Bullet(VECTOR initPos, std::string filename)
	: Object2D(filename, initPos)
	, mvDirection(VGet(1.0f, 0.0f, 0.0f))  // デフォルトでは右方向に進むようにする
	, mfAngle(0.0f)
	, mfspeed(50.0f)
	
{
	SetTag(Object2D::Bullet2D);

}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
	// 移動処理
	Move();

	CalcCollision();

	// 画面外に出たら削除する
	if (IsScreenOut())
	{
		SetDeleteFlag(true);   // 削除フラグを立てて、後で消してもらう
	}


	Object2D::Update();
}

void Bullet::Draw()
{
	Object2D::Draw();
}

void Bullet::Move()
{
	// 座標移動（画面の上のほうに向かって移動）
	// VAdd() ... VECTOR 同士の足し算を行う関数
	// VScale() ... VECTOR に float 値をかける関数
	mvPosition = VAdd(mvPosition, VScale(mvDirection, mfspeed));

	// テクスチャに座標を伝える
	mpTexture->SetPosition(mvPosition);
}

bool Bullet::IsScreenOut()
{
	//return mvPosition.y < 0.0f - mpTexture->GetSizeY() / 2;
	return (mvPosition.x + mpTexture->GetSizeX() / 2) > 640.0f;

}



void Bullet::CalcCollision()
{
	
	// 敵情報を取得
	auto pTargetList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);


	for (int i = 0; i < pTargetList.size(); i++)
	{
		auto pTarget = pTargetList[i];

		// 取得した情報が存在していれば
		if (pTarget != nullptr)
		{

			// 当たり判定
			if (Collision::CheckCircleToCircle(
				mvPosition,
				GetRadius(),
				pTarget->GetPosition(),
				pTarget->GetRadius())
				)
			{

				// 当たった弾を削除
				this->SetDeleteFlag(true);


				Enemy* pEnemy = dynamic_cast<Enemy*>(pTarget);


				if (pEnemy != nullptr)     // dynamic_cast をした後は必ずnullチェックを行う
				{

					
					

				}

				

			}



			

		}

	}

}




