#include "TextureAnimation.h"

// コンストラクタ
TextureAnimation::TextureAnimation(
	VECTOR position,
	std::string filename,
	int allNum,
	int xNum,
	int yNum,
	int interval
)
	: mvPosition()
	, mnCounter(0)
	, mnInterval(interval)
	, mnCurrentNum(0)
	, mnAllNum(allNum)
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

	// テクスチャの分割読み込み
	int success = LoadDivGraph(
		filename.c_str(),
		allNum,
		xNum,
		yNum,
		sizeX / xNum,
		sizeY / yNum,
		mnHandleList
	);
}
// デストラクタ
TextureAnimation::~TextureAnimation()
{

}

void TextureAnimation::Update()
{
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

void TextureAnimation::Draw()
{
	DrawGraph(mvPosition.x, mvPosition.y, mnHandleList[mnCurrentNum], true);
}
