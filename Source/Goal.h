#pragma once
#include "Object2D.h"
#include "DxLib.h"

class Goal : public Object2D
{
public:
	// x, y: 中心座標, width, height: 幅と高さ
	Goal(VECTOR initPos, int width, int height);
	~Goal();

	void Update() override;
	void Draw() override;

	int GetSizeX() override { return mWidth; }
	int GetSizeY() override { return mHeight; }

private:
	int mWidth;
	int mHeight;
	int mColorState;
};
