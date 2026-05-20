#include "BossEnemy.h"
#include "Bullet.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Player.h"



BossEnemy::BossEnemy(VECTOR initPos)
	:Object2D("Resource/Player.bmp", initPos)
{
	SetTag(Object2D::BossEnemy2D);
}

BossEnemy::~BossEnemy()
{

}

void BossEnemy::Update()
{
	// 移動処理
	Move();

	// 基底クラスの更新を呼ぶ
	Object2D::Update();

}

void BossEnemy::Draw()
{
	// 基底クラスの描画を呼ぶ
	Object2D::Draw();

}

void BossEnemy::Move()
{

}

void BossEnemy::BDamage()
{

}

void BossEnemy::Shot()
{

    // 弾を撃つ
    mnShotCounter++;
    if (mnShotCounter > Boss_SHOT_INTERVAL)
    {
        mnShotCounter = 0;

    }
}


void BossEnemy::CreateBullet(VECTOR initPos, VECTOR Dire,float speed)
{
    // 真っすぐ飛ぶ弾の生成
    auto pBullet = new Bullet(initPos,"Resource/bullet.png");
    pBullet->SetDirection(Dire);
    pBullet->SetSpeed(speed);

}

void BossEnemy::RandomBullet()
{

    // プレイヤーの情報を取得
    auto pPlayerObject = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);

	// ターゲットの位置と、自身（今回は弾）の位置から、目標となる角度を求める
// -π～π の間の数値が返ってくる（-3.14 ~ 3.14）
	float targetAngle = atan2f(pPlayerObject->GetPosition().y - mvPosition.y, pPlayerObject->GetPosition().x - mvPosition.x);

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


    Player* pPlayer = dynamic_cast<Player*>(pPlayerObject);


    // 初期位置と発射方向を仮設定
    VECTOR initPos = VGet(-1.0f, 0.0f, 0.0f);
    VECTOR dir = VGet(0.0f, 1.0f, 0.0f);

    if (pPlayer != nullptr)
    {
        // プレイヤーが存在しているのであれば、そこへ飛ぶように向きを変える
        float rad = atan2f(pPlayer->GetPosition().y - initPos.y, pPlayer->GetPosition().x - initPos.x);
        dir.x = cosf(rad);
        dir.y = sinf(rad);
    }

    // 弾を生成
    CreateBullet(initPos, dir, 5.0f);

}