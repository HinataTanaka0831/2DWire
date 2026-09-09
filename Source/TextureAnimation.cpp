#include "TextureAnimation.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"

TextureAnimation::TextureAnimation(
	std::string fileName,
	VECTOR initPosition,
	int allNum,
	int numX,
	int numY,
	int interval,
	float scale,
	bool type
)
	: m_position(initPosition)
	, m_counter(0)
	, m_interval(interval)
	, m_currentNum(0)
	, m_allNum(allNum)
	, m_scale(scale)
	, m_type(type)
{
	m_handleList = new int[allNum];

	int handle = LoadGraph(fileName.c_str());
	// 不正パスや読み込み失敗時のクラッシュを防止するため早期リターン
	if (handle == -1)
	{
		return;
	}

	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);

	// 衝突判定および中心位置調整のため1コマあたりの幅・高さを算出
	m_sizeX = sizeX / numX;
	m_sizeY = sizeY / numY;

	LoadDivGraph(
		fileName.c_str(),
		allNum,
		numX,
		numY,
		sizeX / numX,
		sizeY / numY,
		m_handleList
	);
}

TextureAnimation::~TextureAnimation()
{
	// メモリリーク防止のため動的確保したハンドル配列を解放
	delete[] m_handleList;
}

// フレーム進行とキャラクター向き（左右反転）状態の更新
// 入力: なし / 出力: なし / 副作用: アニメーションコマと反転フラグの更新
void TextureAnimation::Update()
{
	if (m_type)
	{													
		auto pObj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
		Player* pPlayer = dynamic_cast<Player*>(pObj);

		if (pPlayer != nullptr)
		{
			// 静止時でも直前の向きを維持し、ワイヤーアクション中は振り子角加速度の向きに連動させる
			m_playerReverseX = (pPlayer->IsFacingLeft() || (pPlayer->GetAngularAcceleration() < 0.0f && pPlayer->IsWireActive()));
		}
	}

	// 指定フレーム間隔ごとに次のコマへサイクリックに進行
	m_counter++;
	if (m_counter % m_interval == 0)
	{
		m_counter = 0;
		m_currentNum = (m_currentNum + 1) % m_allNum;
	}
}

// カメラ座標を加味した拡縮・回転・反転スプライト描画
// 入力: cameraX, cameraY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
void TextureAnimation::Draw(float cameraX, float cameraY)
{
	if (m_type)
	{
		DrawRotaGraph((int)(m_position.x - cameraX), (int)(m_position.y - cameraY), m_scale, 0.0f, m_handleList[m_currentNum], true, m_playerReverseX);
	}
	else
	{
		DrawRotaGraph((int)(m_position.x - cameraX), (int)(m_position.y - cameraY), m_scale, 0.0f, m_handleList[m_currentNum], true, m_enemyReverseX);
	}
}

// アニメーション再生位置を先頭コマにリセット
// 入力: なし / 出力: なし / 副作用: カウンタおよび現在コマの初期化
void TextureAnimation::Reset()
{
	m_counter = 0;
	m_currentNum = 0;
}