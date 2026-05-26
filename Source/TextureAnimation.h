#pragma once
#include "DxLib.h"
#include <string>

class TextureAnimation
{
public:
	// コンストラクタ
	TextureAnimation(
		std::string filename,
		VECTOR initPos,
		int allNum,
		int xNum,
		int yNum,
		int interval,
		float scale = 1.0f
	);  
	// デストラクタ
	~TextureAnimation(); 

	void Update();  // 更新
	void Draw(float cameraX = 0.0f, float cameraY = 0.0f);   // 描画

	void Reset(); // アニメーションのリセット
	void SetPosition(VECTOR pos) { mvPosition = pos; } // 位置の設定描画

	int GetSizeX() { return mnSizeX; }    // 幅サイズ取得
	int GetSizeY() { return mnSizeY; }    // 高さサイズ取得

	float GetRadius() { return (float)mnSizeX / 2.0f; }    // 半径取得（とりあえず幅の半分を半径としておく）

	void SetScale(float scale) { mnScale = scale; }    // 描画倍率設定
	float GetScale() { return mnScale; }    // 描画倍率取得


private:
	VECTOR mvPosition;  // ポジション
	int mnCounter;      // アニメーションカウンタ
	int mnInterval;     // テクスチャ切り替えのフレーム数
	int mnCurrentNum;   // 何番目のテクスチャを表示するか
	int* mnHandleList;  // 分割されたテクスチャのハンドルリスト
	int mnAllNum;       // テクスチャ分割数
	int mnSizeX;       // 分割されたテクスチャの幅
	int mnSizeY;       // 分割されたテクスチャの高さ
	float mnScale;     // 描画倍率
};
