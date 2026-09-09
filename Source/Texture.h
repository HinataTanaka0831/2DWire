#pragma once
#include <string>
#include "DxLib.h"

// 単一画像グラフィックリソースの保持・中心基準描画を担当するクラス
class Texture
{
public:
	// 画像読み込みと中心座標・透過フラグの初期化
	// 入力: fileName(画像パス), centerPosition(中心座標), transFlag(透過有無) / 出力: なし / 副作用: 画像ハンドル読み込み
	Texture(std::string fileName, VECTOR centerPosition, int transFlag);     

	// 保持している画像ハンドルの破棄
	// 入力: なし / 出力: なし / 副作用: DXライブラリ画像リソースの解放
	~Texture();     

	void Update();  

	// カメラオフセットを考慮した中心基準描画
	// 入力: offsetX, offsetY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
	void Draw(float offsetX = 0.0f, float offsetY = 0.0f);

	void SetPosition(VECTOR centerPosition) { m_position = centerPosition; }
	VECTOR GetPosition() { return m_position; }
	int GetSizeX() { return m_sizeX; }
	int GetSizeY() { return m_sizeY; }
	float GetRadius() { return m_radius; }
	
private:
	int m_handle = -1;      // DXライブラリグラフィックハンドル
	VECTOR m_position; // 中心ワールド座標
	int m_sizeX = 0;       // テクスチャ横幅（ピクセル）
	int m_sizeY = 0;       // テクスチャ縦幅（ピクセル）
	int m_transFlag;   // 透過描画フラグ
	float m_radius;    // 衝突判定等で使用する近似半径
};