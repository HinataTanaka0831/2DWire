#pragma once
#include "Object2D.h"
#include "DxLib.h"

class Ground : public Object2D
{
public:
	// x, y: 中心座標, width, height: 幅と高さ
	Ground(std::string filename, VECTOR initPos, int width, int height);
	~Ground();

	void Update() override;
	void Draw() override;

	int GetSizeX() override { return mWidth; }
	int GetSizeY() override { return mHeight; }

private:
	int mWidth;
	int mHeight;
};
