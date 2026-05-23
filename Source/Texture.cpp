#include "Texture.h"
#include "DxLib.h"

Texture::Texture(std::string filename, VECTOR centerPosition, int transFlag)
    : mnHandle(-1)
    , mvPosition(centerPosition)
    , mnSizeX(0)
    , mnSizeY(0)
    , mnTransFlag(transFlag)
{
    // 画像の読み込み
    mnHandle = LoadGraph(filename.c_str());

    // 画像のサイズを取得する
    GetGraphSize(mnHandle, &mnSizeX, &mnSizeY);

    // 半径を設定（とりあえず共通で幅の半分を半径としておく）
    mfRadius = (float)mnSizeX / 2.0f;
}

Texture::~Texture()
{
    // 読み込んだ画像の破棄
    DeleteGraph(mnHandle);
}

void Texture::Update()
{
    
}

void Texture::Draw(float offsetX, float offsetY)
{
    // 読み込んだ画像を描画
    DrawGraph((int)(mvPosition.x - offsetX) - (mnSizeX / 2), (int)(mvPosition.y - offsetY) - (mnSizeY / 2), mnHandle, mnTransFlag);
}
