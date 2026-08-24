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
	, displayDamage(maxHP)
{
	mvDirection = VGet(-1.0f, 0.0f, 0.0f);
	SetTag(Object2D::Enemy2D);

	mAnimController.RegisterAnimation(CharacterState::Idle,
		std::make_unique<TextureAnimation>(filename, initPos, allNum, numX, numY, interval, scale, type));
	
	mAnimController.RegisterAnimation(CharacterState::Moving,
		std::make_unique<TextureAnimation>("Resource/Enemy/anim_monster01walk.png", initPos, 6, 6, 1, 8, scale, type));

	// 攻撃モーションの俊敏さを演出するため再生速度を調整して登録
	mAnimController.RegisterAnimation(CharacterState::Attacking,
		std::make_unique<TextureAnimation>("Resource/Enemy/anim_monster01attack.png", initPos, 4, 4, 1, 10, scale, type));

	mAnimController.ChangeState(CharacterState::Idle);
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
	if (mnHitFlashTimer > 0)
	{
		mnHitFlashTimer--;
	}

	if (hp <= 0) {
		mAnimController.ChangeState(CharacterState::Dead);
	} 
	else {
		auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
		Player* pPlayer = dynamic_cast<Player*>(pTarget);
		
		if (pPlayer != nullptr) {
			VECTOR diff = VSub(pPlayer->GetPosition(), mvPosition);
			float distance = VSize(diff);

			// プレイヤーとの距離閾値に応じてアニメーション状態を自動遷移
			if (distance <= Attack_Range) {
				mAnimController.ChangeState(CharacterState::Attacking);
			}
			else if (distance <= Search_Range) {
				mAnimController.ChangeState(CharacterState::Moving);
			}
			else {
				mAnimController.ChangeState(CharacterState::Idle);
			}
		}
		else {
			mAnimController.ChangeState(CharacterState::Idle);
		}
	}

	mAnimController.Update(mvPosition.x, mvPosition.y);
}

// カメラ座標を加味した敵スプライト（被弾フラッシュ含む）とHPゲージの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Enemy::Draw()
{
	// 被弾フィードバックとして一定周期で赤色点滅表示
	if (mnHitFlashTimer > 0 && ((mnHitFlashTimer / HitFlashInterval) % 2 == 0))
	{
		SetDrawBright(255, 96, 96);
	}

	mAnimController.Draw(gCameraX, gCameraY);
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
		isCollidingWithPlayer = Collision::CheckCircleToCircle(
			mvPosition, GetRadius(),
			pPlayer->GetPosition(), pPlayer->GetRadius());

		VECTOR diff = VSub(pPlayer->GetPosition(), mvPosition);
		float distance = VSize(diff);

		mnAttackCooldown--;

		if (distance <= Attack_Range)
		{
			mvDirection.x = 0.0f;

			// クールダウン満了時にプレイヤーへダメージ適用
			if (mnAttackCooldown <= 0)
			{
				pPlayer->PDamage(5);
				mnAttackCooldown = Attack_Interval;
			}
		}
		else if (distance <= Search_Range)
		{
			// プレイヤーの方向へ追従移動
			mvDirection.x = (diff.x > 0.0f) ? 1.0f : -1.0f;
			mvPosition.x += (float)MoveSpeed * mvDirection.x;
			mAnimController.SetEnemyReverse(mvDirection.x > 0.0f);

			// 接近直後の初撃猶予時間を与え、プレイヤーの回避猶予を確保
			if (mnAttackCooldown <= 0)
			{
				mnAttackCooldown = Attack_Interval;
			}
		}
		else
		{
			mvDirection.x = 0.0f;
		}
	}

	// 敵同士の物理的なめり込み・重なりを解消
	ResolveEnemyOverlap();
}

bool Enemy::IsScreenOut()
{
	return (mvPosition.x + mpTexture->GetSizeX() / 2) < 0.0f;
}

// プレイヤーからの被弾処理と死亡時削除フラグ設定
// 入力: damage(ダメージ量) / 出力: なし / 副作用: hp減算、被弾点滅タイマー開始、SetDeleteFlag
void Enemy::EDamage(int damage)
{
	hp -= damage;
	mnHitFlashTimer = HitFlashDuration;
	if (hp <= 0)
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

		VECTOR diff = VSub(mvPosition, pOtherEnemy->GetPosition());
		float distance = VSize(diff);
		float minDistance = GetRadius() + pOtherEnemy->GetRadius();

		if (distance >= minDistance)
		{
			continue;
		}

		// 同一座標時のゼロ除算を回避するため微小オフセットを付与
		if (distance <= 0.001f)
		{
			mvPosition.x += (this < pOtherEnemy) ? -1.5f : 1.5f;
			continue;
		}

		float overlap = minDistance - distance;
		VECTOR pushDir = VNorm(diff);

		// 重なり量の半分を互いに押し出し自然に分離
		mvPosition.x += pushDir.x * (overlap * 0.5f);
		mvPosition.y += pushDir.y * (overlap * 0.5f);
	}
}

// 敵頭上にHPゲージ（現在値＋ダメージ演出バー）を描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Enemy::HPGaugeDraw()
{
	int gaugeX = (int)(mvPosition.x - gCameraX) - 110;
	int gaugeY = (int)(mvPosition.y - gCameraY) - 190;

	DrawBox(gaugeX, gaugeY, gaugeX + width, gaugeY + gaugeHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + damageWidth, gaugeY + gaugeHeight, GetColor(255, 0, 0), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(51, 204, 51), TRUE);
	DrawBox(gaugeX, gaugeY, gaugeX + width, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);
}

// 被弾時に減少分を滑らかに追従させるHP演出ゲージの幅計算
// 入力: なし / 出力: なし / 副作用: displayDamage, damageWidth, gaugeWidthの更新
void Enemy::HPGaugeUpdate()
{
	hpGaugeTimer++;

	if (hpGaugeTimer >= GaugeFrame)
	{
		if (displayDamage > hp)
		{
			// 被弾演出としてダメージ減少幅を段階的に追従
			displayDamage--;
			if (displayDamage < hp)
			{
				displayDamage = hp;
			}
		}
		hpGaugeTimer = 0;
	}

	int display = displayDamage;
	// 負値による反転描画バグを防ぐための境界ガード
	if (display < 0 && display > minWidth)
	{
		display = minWidth;
	}

	int displayHp = hp;
	if (displayHp < 0 && displayHp > minWidth)
	{
		displayHp = minWidth;
	}

	damageWidth = (int)((float)display / maxHP * width);
	gaugeWidth = (int)((float)displayHp / maxHP * width);
}