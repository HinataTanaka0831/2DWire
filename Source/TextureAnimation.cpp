#include "TextureAnimation.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Enemy.h"

bool gEnemyReverseX = false; // Global flag indicating if enemies should be drawn flipped horizontally

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
	// 不正ファイル読み込みによるクラッシュを回避するため、失敗時は早期離脱
	if (handle == -1)
	{
		return;
	}

	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);

	// 衝突判定および描画時の矩形計算用として等分割した一コマ分のサイズを保存
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
	// メモリリーク防止のため、コンストラクタでnewしたグラフィックハンドル配列を確実に解放
	delete[] mnHandleList;
}

void TextureAnimation::Update()
{
	auto pObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);
	Player* pPlayer = dynamic_cast<Player*>(pObj);

	if (pPlayer != nullptr)
	{
		// 意図しない反転挙動のバグを防ぐため、演算子の優先順位を括弧で明示
		mbPlayerReverseX = (pPlayer->GetVelocityX() < 0.0f || (pPlayer->GetAngularAcceleration() < 0.0f && pPlayer->IsWireActive()));
	}

	    mbEnemyReverseX = gEnemyReverseX; // set enemy flip based on movement direction
    // 指定されたインターバル時間に基づき、等速でアニメーションをループ再生
	mnCounter++;
	if (mnCounter % mnInterval == 0)
	{
		mnCounter = 0;
		mnCurrentNum = (mnCurrentNum + 1) % mnAllNum;
	}
}

void TextureAnimation::Draw(float cameraX, float cameraY)
{
	if (mbType)
	{
		DrawRotaGraph((int)(mvPosition.x - cameraX), (int)(mvPosition.y - cameraY), mnScale, 0.0f, mnHandleList[mnCurrentNum], true, mbPlayerReverseX);
	}

	if (!mbType)
	{
		DrawRotaGraph((int)(mvPosition.x - cameraX), (int)(mvPosition.y - cameraY), mnScale, 0.0f, mnHandleList[mnCurrentNum], true, mbEnemyReverseX);
	}
}

void TextureAnimation::Reset()
{
	mnCounter = 0;
	mnCurrentNum = 0;
}
