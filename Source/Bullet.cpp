#include "Bullet.h"
#include "Collision.h"
#include "Enemy.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Player.h"

Bullet::Bullet(VECTOR initPos, std::string filename)
	: Object2D(filename, initPos)
	, mvDirection(VGet(1.0f, 0.0f, 0.0f))
	, mfAngle(0.0f)
	, mfspeed(50.0f)
{
	SetTag(Object2D::Bullet2D);
}

Bullet::~Bullet()
{
}

// 移動、敵との当たり判定、画面外破棄判定の実行
// 入力: なし / 出力: なし / 副作用: 座標更新、衝突時の相手へのダメージ適用、削除フラグ設定
void Bullet::Update()
{
	Move();
	CalcCollision();

	// メモリ節約とパフォーマンス維持のため画面外に出た弾を自動消滅させる
	if (IsScreenOut())
	{
		SetDeleteFlag(true);
	}

	Object2D::Update();
}

void Bullet::Draw()
{
	Object2D::Draw();
}

// 直線移動計算
// 入力: なし / 出力: なし / 副作用: 座標の更新
void Bullet::Move()
{
	// 方向ベクトルに速度を乗算してワールド座標を進行
	mvPosition = VAdd(mvPosition, VScale(mvDirection, mfspeed));
	mpTexture->SetPosition(mvPosition);
}

// 画面外（有効描画領域外）へ到達したかの判定
// 入力: なし / 出力: 画面外ならtrue / 副作用: なし
bool Bullet::IsScreenOut()
{
	return (mvPosition.x + mpTexture->GetSizeX() / 2) > 640.0f;
}

// 敵オブジェクト一覧との交差判定およびダメージ適用
// 入力: なし / 出力: なし / 副作用: 衝突時に弾自身の削除フラグを立てる
void Bullet::CalcCollision()
{
	auto pTargetList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);

	for (size_t i = 0; i < pTargetList.size(); i++)
	{
		auto pTarget = pTargetList[i];
		if (pTarget != nullptr)
		{
			if (Collision::CheckCircleToCircle(
				mvPosition,
				GetRadius(),
				pTarget->GetPosition(),
				pTarget->GetRadius()))
			{
				// 多重ヒットを防ぐため着弾と同時に弾の削除フラグを有効化
				this->SetDeleteFlag(true);

				Enemy* pEnemy = dynamic_cast<Enemy*>(pTarget);
				if (pEnemy != nullptr)
				{
				}
			}
		}
	}
}