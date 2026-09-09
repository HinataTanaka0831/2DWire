#include "Texture.h"
#include "DxLib.h"

Texture::Texture(std::string fileName, VECTOR centerPosition, int transFlag)
    : m_position(centerPosition)
    , m_transFlag(transFlag)
{
    m_handle = LoadGraph(fileName.c_str());
    GetGraphSize(m_handle, &m_sizeX, &m_sizeY);

    // 矩形の中央基準で円形判定を行うための簡易半径（横幅の半分）
    m_radius = (float)m_sizeX / 2.0f;
}

Texture::~Texture()
{
    // メモリリーク防止のため保持ハンドルを確実に解放
    DeleteGraph(m_handle);
}

void Texture::Update()
{
}

// カメラオフセットを考慮した中心基準描画
// 入力: offsetX, offsetY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
void Texture::Draw(float offsetX, float offsetY)
{
    // オブジェクトの中心座標が指定位置に来るよう左上オフセットを引いて描画
    DrawGraph((int)(m_position.x - offsetX) - (m_sizeX / 2), (int)(m_position.y - offsetY) - (m_sizeY / 2), m_handle, m_transFlag);
}