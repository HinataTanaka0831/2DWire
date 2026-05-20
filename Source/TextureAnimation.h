#pragma once
#include "DxLib.h"
#include <string>

class TextureAnimation
{
public:
	// コンストラクタ
	TextureAnimation(
		VECTOR position,
		std::string filename,
		int allNum,
		int xNum,
		int yNum,
		int interval
	);  
	// デストラクタ
	~TextureAnimation(); 

	void Update();  // 更新
	void Draw();   // 描画

private:
	VECTOR mvPosition;  // ポジション
	int mnCounter;      // アニメーションカウンタ
	int mnInterval;     // テクスチャ切り替えのフレーム数
	int mnCurrentNum;   // 何番目のテクスチャを表示するか
	int* mnHandleList;  // 分割されたテクスチャのハンドルリスト
	int mnAllNum;       // テクスチャ分割数
};
