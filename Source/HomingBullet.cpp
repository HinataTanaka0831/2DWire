#include "HomingBullet.h"
#include "Player.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"


HomingBullet::HomingBullet(VECTOR initPos, std::string filename)
	:Object2D(filename, initPos)

{
	SetTag(Object2D::HomingBullet2D);

}


HomingBullet::~HomingBullet()
{

}

void HomingBullet::Update()
{
	CalcDirection();

	Object2D::Update();
}

void HomingBullet::Draw()
{
	Object2D::Draw();
}

void HomingBullet::CalcDirection()
{

	// 敵情報を取得
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);


			// ターゲットの位置と、自身（今回は弾）の位置から、目標となる角度を求める
		// -π～π の間の数値が返ってくる（-3.14 ~ 3.14）
			float targetAngle = atan2f(mpPlayer->GetPosition().y - mvPosition.y, mpPlayer->GetPosition().x - mvPosition.x);

			// 新しい角度と現在の角度の差分を求める
			float subAngle = targetAngle - mfAngle;

			// atan2f によって求められた値は -π～π の間になるので、
			// 差分の値がそれ未満、あるいは超えた場合、-π～π の間に収まるように調整する
			// DX_PI_F ... DXライブラリに用意されている円周率（3.141592）
			// DX_TWO_PI_F ... DXライブラリに用意されている円周率の2倍（3.14 * 2.0f）
			if (subAngle < -DX_PI_F)
			{
				subAngle += DX_TWO_PI_F;
			}
			else if (subAngle > DX_PI_F)
			{
				subAngle -= DX_TWO_PI_F;
			}

			// 角度の差分を 0 に近づける
			if (subAngle > 0.0f)
			{
				subAngle -= 0.05f;
				if (subAngle < 0.0f)
				{
					subAngle = 0.0f;
				}
			}
			else
			{
				subAngle += 0.05f;
				if (subAngle > 0.0f)
				{
					subAngle = 0.0f;
				}
			}

			// 向かいたい方向を求める
			mfAngle = targetAngle - subAngle;

			// 移動方向を求める
			mvDirection.x = cosf(mfAngle);
			mvDirection.y = sinf(mfAngle);


}