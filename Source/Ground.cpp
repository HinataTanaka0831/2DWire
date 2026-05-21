#include "Ground.h"
#include "Utility.h"

Ground::Ground(std::string filename, VECTOR initPos, int width, int height)
	: Object2D(filename, initPos)
	, mWidth(width)
	, mHeight(height)
{
	SetTag(Object2D::Ground2D);
}

Ground::~Ground()
{
}

void Ground::Update()
{
	Object2D::Update();
}

void Ground::Draw()
{
	Object2D::Draw();
	// カメラのオフセットを適用
	//int drawX = (int)(mvPosition.x - gCameraX);
	//int drawY = (int)mvPosition.y;

	// 中心座標から矩形を描画
	//int left = drawX - mWidth / 2;
	//int top = drawY - mHeight / 2;
	//int right = drawX + mWidth / 2;
	//int bottom = drawY + mHeight / 2;

	// 地面の色
	//DrawBox(left, top, right, bottom, GetColor(100, 100, 100), TRUE);
	// 枠線
	//DrawBox(left, top, right, bottom, GetColor(50, 50, 50), FALSE);
}
