#include "Player.h"
#include "Collision.h"
#include "HomingBullet.h"
#include "Bullet.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Collision.h"
#include "GameScene.h"
#include "Utility.h"
#include <cmath>



Player::Player(std::string filename, VECTOR initPos)
	: Object2D(filename, initPos)
	, displayDamage(maxHp)
{
	SetTag(Object2D::Player2D);

}

Player::~Player()
{

}

void Player::Update()
{
	// HPゲージの更新
	HPGaugeUpdate();

	// 移動処理
	Move();

	// 基底クラスの更新を呼ぶ
	Object2D::Update();

}

void Player::Draw()
{
	// ワイヤー接続中ならワイヤー（線）を描画
	if (mbIsWireActive)
	{
		DrawLine((int)(mvPosition.x - gCameraX), (int)(mvPosition.y - gCameraY),
		         (int)(mvWireTargetPos.x - gCameraX), (int)(mvWireTargetPos.y - gCameraY),
		         GetColor(200, 255, 255), 3);
	}

	// クラスの描画呼ぶ
	Object2D::Draw();
}

void Player::Move()
{
	// ----------------------------------------------------
	// 1. ワイヤーの接続・解除判定
	// ----------------------------------------------------
	static int oldMouseInput = 0; // 前フレームのマウス入力状態
	int mouseInput = GetMouseInput();
	int clickX, clickY;
	GetMousePoint(&clickX, &clickY);

	// 左クリックされた瞬間かチェック
	if ((mouseInput & MOUSE_INPUT_LEFT) && !(oldMouseInput & MOUSE_INPUT_LEFT))
	{
		// ターゲットを探す
		std::vector<Object2D*> targets = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::WireTarget2D);
		
		float worldClickX = clickX + gCameraX;
		float worldClickY = clickY + gCameraY; // gCameraYを加えてワールド座標に変換
		
		for (auto* obj : targets)
		{
			VECTOR tPos = obj->GetPosition();
			int sizeX = obj->GetSizeX();
			int sizeY = obj->GetSizeY();

			VECTOR objDis = VSub(tPos, mvPosition);

			float objdistance = VSize(objDis);

			// ターゲットが遠すぎる場合はスルー
			if (objdistance >= 950.0f)
			{
				mbIsWireActive = false;
				continue; 
			}
			// オブジェクトの矩形範囲内か判定（クリックした「好きなところ」にワイヤーを付ける）
			if (worldClickX >= tPos.x - sizeX / 2.0f && worldClickX <= tPos.x + sizeX / 2.0f &&
				worldClickY >= tPos.y - sizeY / 2.0f && worldClickY <= tPos.y + sizeY / 2.0f)
			{
				// ワイヤー接続！
				mbIsWireActive = true;
				// クリックしたワールド座標をターゲット位置にする
				mvWireTargetPos = VGet(worldClickX, worldClickY, 0.0f);
				
				// 振り子の初期計算（プレイヤーと支点の距離と角度）
				float diffX = mvPosition.x - mvWireTargetPos.x;
				float diffY = mvPosition.y - mvWireTargetPos.y;
				mWireLength = std::sqrt(diffX * diffX + diffY * diffY);
				// 真下を0度とするため、atan2(X, Y)を利用
				mPendulumAngle = std::atan2(diffX, diffY); 
				
				// 角速度に現在の速度を反映（簡易的にX速度から変換）
				if (mWireLength > 0.0f) {
					mPendulumAngularVelocity = mVelocityX / mWireLength; 
				}
				break;
			}
		}
	}
	// 左クリックを離した瞬間
	else if (!(mouseInput & MOUSE_INPUT_LEFT) && (oldMouseInput & MOUSE_INPUT_LEFT))
	{
		if (mbIsWireActive)
		{
			mbIsWireActive = false;
			// ワイヤー解除時に振り子速度を直線速度に変換
			// 振り子の接線方向速度 = r * ω
			float tangentVel = mWireLength * mPendulumAngularVelocity;
			// 角度に応じてX, Y速度に分解
			mVelocityX = tangentVel * std::cos(mPendulumAngle);
			mVelocityY = -tangentVel * std::sin(mPendulumAngle);
		}
	}
	
	oldMouseInput = mouseInput;

	// ----------------------------------------------------
	// 2. 移動処理（ワイヤー状態 vs 通常状態）
	// ----------------------------------------------------
	if (mbIsWireActive)
	{
		// 【ワイヤー接続中：ターザン（振り子）移動】
		if (mWireLength > 0.0f)
		{
			// 重力による角加速度（-g/L * sin(θ)）
			float angularAcceleration = -(GRAVITY / mWireLength) * std::sin(mPendulumAngle);
			
			// プレイヤーの左右キー入力でスイングを加速・減速
			if (CheckHitKey(KEY_INPUT_D)) angularAcceleration += 0.0013f;
			if (CheckHitKey(KEY_INPUT_A)) angularAcceleration -= 0.0013f;

			// 減衰（空気抵抗）
			mPendulumAngularVelocity *= 0.995f;
			
			// 積分して速度と角度を更新
			mPendulumAngularVelocity += angularAcceleration;
			mPendulumAngle += mPendulumAngularVelocity;

			// 角度と距離から新しい座標を計算
			mvPosition.x = mvWireTargetPos.x + std::sin(mPendulumAngle) * mWireLength;
			mvPosition.y = mvWireTargetPos.y + std::cos(mPendulumAngle) * mWireLength;
		}
	}
	else
	{
		// 【通常状態：重力落下とキー移動】
		mVelocityY += GRAVITY; // 重力落下

		if (CheckHitKey(KEY_INPUT_D))
		{
			mVelocityX += 0.5f; // 右へ加速
		}
		else if (CheckHitKey(KEY_INPUT_A))
		{
			mVelocityX -= 0.5f; // 左へ加速
		}
		else
		{
			mVelocityX *= 0.85f; // 摩擦で減速
		}

		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			// ジャンプ（地面にいるときのみ）
			if (mvPosition.y >= 850.0f) // 地面にいるかの簡易判定
			{
				mVelocityY = -10.0f; // 上向きの速度を与える
			}
		}


		// 最高速度制限
		if (mVelocityX > (float)MOVE_SPEED * 1.5f) mVelocityX = (float)MOVE_SPEED * 1.5f;
		if (mVelocityX < -(float)MOVE_SPEED * 1.5f) mVelocityX = -(float)MOVE_SPEED * 1.5f;
		
		// 座標更新
		mvPosition.x += mVelocityX;
		mvPosition.y += mVelocityY;
	}

	// 共通の地面判定（画面下部に行かないようにする）
	if (mvPosition.y > 950.0f)
	{
		mvPosition.y = 950.0f;

		if (mbIsWireActive)
		{
			mvPosition.y = 850.0f;
			// ワイヤー使用中に地面に触れた場合、たるまないようにワイヤーを自動で縮める
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


	// テクスチャ座標に反映
	mpTexture->SetPosition(mvPosition);
}

// HPゲージの描画
void Player::HPGaugeDraw()
{
	int gaugeX = (int)(mvPosition.x - gCameraX) - 120; // HPゲージの表示位置X
	int gaugeY = (int)mvPosition.y - 80; // HPゲージの表示位置Y

	// HPゲージの枠を描画
	DrawBox(gaugeX, gaugeY, gaugeX + width, gaugeY + gaugeHeight, GetColor(0, 0, 0), TRUE);

	// HPゲージのダメージ表示を描画（赤）
	DrawBox(gaugeX, gaugeY, gaugeX + damageWidth, gaugeY + gaugeHeight, GetColor(255, 0, 0), TRUE);

	// HPゲージの中身を描画（緑）
	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(51, 204, 51), TRUE);

	// HPゲージの外枠を描画
	DrawBox(gaugeX, gaugeY, gaugeX + width, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);
}

// HPゲージの更新
void Player::HPGaugeUpdate()
{
	//HPゲージのダメージを受けたときの処理
	if (displayDamage > Hp)
	{
		displayDamage -= gaugeSpeed; // HPゲージの表示位置を徐々に元に戻す

		if (displayDamage < Hp)
        {
 		    displayDamage = Hp; // HPゲージの表示位置を徐々に元に戻す
		}

	}

	int display = displayDamage;

	if (display < 0 && display > minWidth)
	{
		display = minWidth; // HPゲージの最小幅を設定
	}

	int displayHp = Hp;

	if (displayHp < 0 && displayHp > minWidth)
	{
		displayHp = minWidth; // HPゲージの最小幅を設定
	}

	// ダメージ表示の幅を更新
	damageWidth = (int)((float)display / maxHp * width); 

	// HPゲージの幅を更新
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

	
