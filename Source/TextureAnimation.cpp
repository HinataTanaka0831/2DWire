#include "TextureAnimation.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player.h"

// コンストラクタ
TextureAnimation::TextureAnimation(
	std::string filename,
	VECTOR initPos,
	int allNum,
	int NumX,
	int NumY,
	int interval,
	float scale
)
	: mvPosition(initPos)
	, mnCounter(0)
	, mnInterval(interval)
	, mnCurrentNum(0)
	, mnAllNum(allNum)
	, mnScale(scale)
{
	mnHandleList = new int[allNum];

	// 画像ファイル読み込み
	int handle = LoadGraph(filename.c_str());
	if (handle == -1)
	{
		return;   // 読み込み失敗していたら以降は処理しない
	}

	// サイズ取得
	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);

	mnSizeX = sizeX / NumX;    // 分割されたテクスチャの幅を保存
	mnSizeY = sizeY / NumY;    // 分割されたテクスチャの高さを保存

	// テクスチャの分割読み込み
	int success = LoadDivGraph(
		filename.c_str(),
		allNum,
		NumX,
		NumY,
		sizeX / NumX,
		sizeY / NumY,
		mnHandleList
	);

}
// デストラクタ
TextureAnimation::~TextureAnimation()
{

}

// 更新
void TextureAnimation::Update()
{

	auto pObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pObj);

	if (pPlayer != nullptr)
	{
		// プレイヤーの向きに合わせて描画反転フラグを設定
		mbReverseX = (pPlayer->GetVelocityX() < 0.0f || pPlayer->GetAngularAcceleration() < 0.0f && pPlayer->IsWireActive()); // 左向きなら反転
	}

	// カウンタをインクリメント
	mnCounter++;
	if (mnCounter % mnInterval == 0)
	{
		mnCounter = 0;   // カウンタを戻す
		mnCurrentNum++;  // テクスチャ番号を進める
		if (mnCurrentNum >= mnAllNum)  // 分割数を超えるならループさせる
		{
			mnCurrentNum = 0;   // ループさせる
		}
	}
}

// 描画
void TextureAnimation::Draw(float cameraX, float cameraY)
{
	DrawRotaGraph((int)(mvPosition.x - cameraX), (int)(mvPosition.y - cameraY), mnScale, 0.0f, mnHandleList[mnCurrentNum], true, mbReverseX, mbReverseY);
}

void TextureAnimation::Reset()
{
	mnCounter = 0;
	mnCurrentNum = 0;
}
