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
		int NumX,
		int NumY,
		int interval,
		float scale = 1.0f
	);  
	// デストラクタ
	~TextureAnimation(); 

	void Update();  // 更新
	void Draw(float cameraX = 0.0f, float cameraY = 0.0f);   // 描画

	void Reset(); // アニメーションのリセット
	void SetPosition(VECTOR pos) { mvPosition = pos; } // 位置の設定描画

	// 描画
	void Draw();   

	// 幅サイズ取得
	int GetSizeX() { return mnSizeX; }   

	// 高さサイズ取得
	int GetSizeY() { return mnSizeY; }    

	// 半径取得（とりあえず幅の半分を半径としておく）
	float GetRadius() { return (float)mnSizeX / 2.0f; }   

	// 描画倍率取得
	float GetScale() { return mnScale; }    

	// アニメーションを1度だけ
	int IsEnd() { return mnCurrentNum >= mnAllNum - 1; }

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
