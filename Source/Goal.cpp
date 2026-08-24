#include "Goal.h"
#include "Utility.h"
#include <cmath>

Goal::Goal(VECTOR initPos, int width, int height)
	: Object2D("", initPos)
	, mWidth(width)
	, mHeight(height)
	, mColorState(0)
{
	SetTag(Object2D::Goal2D);
}

Goal::~Goal()
{
}

// 演出用カラーサイクルの更新
// 入力: なし / 出力: なし / 副作用: mColorStateの加算
void Goal::Update()
{
	Object2D::Update();
	mColorState++;
}

// カメラ座標を加味したゴールゲートおよび「GOAL」テキストの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Goal::Draw()
{
	int drawX = (int)(mvPosition.x - gCameraX);
	int drawY = (int)(mvPosition.y - gCameraY);

	int left = drawX - mWidth / 2;
	int top = drawY - mHeight / 2;
	int right = drawX + mWidth / 2;
	int bottom = drawY + mHeight / 2;

	// 視認性を高めるためサイン波で枠色を周期的に変化させる
	int r = 200 + (int)(55.0 * std::sin(mColorState * 0.1));
	int g = 200 + (int)(55.0 * std::sin(mColorState * 0.1 + 2.0));
	int b = 100;

	DrawBox(left, top, right, bottom, GetColor(r, g, b), TRUE);
	DrawBox(left, top, right, bottom, GetColor(255, 255, 255), FALSE);
	
	DrawString(drawX - 20, drawY - 10, "GOAL", GetColor(0, 0, 0), TRUE);
}