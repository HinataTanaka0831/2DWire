#include "TextureAnimation.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Enemy.h"

TextureAnimation::TextureAnimation(
	std::string filename,
	VECTOR initPos,
	int allNum,
	int NumX,
	int NumY,
	int interval,
	float scale,
	bool type
)
	: mvPosition(initPos)
	, mnCounter(0)
	, mnInterval(interval)
	, mnCurrentNum(0)
	, mnAllNum(allNum)
	, mnScale(scale)
	, mbType(type)
{
	mnHandleList = new int[allNum];

	int handle = LoadGraph(filename.c_str());
	// 不正パスや読み込み失敗時のクラッシュを防止するため早期リターン
	if (handle == -1)
	{
		return;
	}

	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);

	// 衝突判定および中心位置調整のため1コマあたりの幅・高さを算出
	mnSizeX = sizeX / NumX;
	mnSizeY = sizeY / NumY;

	LoadDivGraph(
		filename.c_str(),
		allNum,
		NumX,
		NumY,
		sizeX / NumX,
		sizeY / NumY,
		mnHandleList
	);
}

TextureAnimation::~TextureAnimation()
{
	// メモリリーク防止のため動的確保したハンドル配列を解放
	delete[] mnHandleList;
}

// フレーム進行とキャラクター向き（左右反転）状態の更新
// 入力: なし / 出力: なし / 副作用: アニメーションコマと反転フラグの更新
void TextureAnimation::Update()
{
	if (mbType)
	{
		auto pObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
		Player* pPlayer = dynamic_cast<Player*>(pObj);

		if (pPlayer != nullptr)
		{
			// 静止時でも直前の向きを維持し、ワイヤーアクション中は振り子角加速度の向きに連動させる
			mbPlayerReverseX = (pPlayer->IsFacingLeft() || (pPlayer->GetAngularAcceleration() < 0.0f && pPlayer->IsWireActive()));
		}
	}

	// 指定フレーム間隔ごとに次のコマへサイクリックに進行
	mnCounter++;
	if (mnCounter % mnInterval == 0)
	{
		mnCounter = 0;
		mnCurrentNum = (mnCurrentNum + 1) % mnAllNum;
	}
}

// カメラ座標を加味した拡縮・回転・反転スプライト描画
// 入力: cameraX, cameraY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
void TextureAnimation::Draw(float cameraX, float cameraY)
{
	if (mbType)
	{
		DrawRotaGraph((int)(mvPosition.x - cameraX), (int)(mvPosition.y - cameraY), mnScale, 0.0f, mnHandleList[mnCurrentNum], true, mbPlayerReverseX);
	}
	else
	{
		DrawRotaGraph((int)(mvPosition.x - cameraX), (int)(mvPosition.y - cameraY), mnScale, 0.0f, mnHandleList[mnCurrentNum], true, mbEnemyReverseX);
	}
}

// アニメーション再生位置を先頭コマにリセット
// 入力: なし / 出力: なし / 副作用: カウンタおよび現在コマの初期化
void TextureAnimation::Reset()
{
	mnCounter = 0;
	mnCurrentNum = 0;
}