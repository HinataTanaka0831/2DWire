#include "Texture.h"
#include "DxLib.h"

Texture::Texture(std::string filename, VECTOR centerPosition, int transFlag)
    : mnHandle(-1)
    , mvPosition(centerPosition)
    , mnSizeX(0)
    , mnSizeY(0)
    , mnTransFlag(transFlag)
{
    mnHandle = LoadGraph(filename.c_str());
    GetGraphSize(mnHandle, &mnSizeX, &mnSizeY);

    // 矩形の中央基準で円形判定を行うための簡易半径（横幅の半分）
    mfRadius = (float)mnSizeX / 2.0f;
}

Texture::~Texture()
{
    // メモリリーク防止のため保持ハンドルを確実に解放
    DeleteGraph(mnHandle);
}

void Texture::Update()
{
}

// カメラオフセットを考慮した中心基準描画
// 入力: offsetX, offsetY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
void Texture::Draw(float offsetX, float offsetY)
{
    // オブジェクトの中心座標が指定位置に来るよう左上オフセットを引いて描画
    DrawGraph((int)(mvPosition.x - offsetX) - (mnSizeX / 2), (int)(mvPosition.y - offsetY) - (mnSizeY / 2), mnHandle, mnTransFlag);
}