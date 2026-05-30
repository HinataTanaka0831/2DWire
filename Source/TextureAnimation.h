#pragma once
#include "DxLib.h"
#include <string>

class TextureAnimation
{
public:
	// スプライトシート画像を分割ロードし、アニメーション用リソースを初期化
	// 入力：filename（画像パス）, initPos（初期座標）, allNum（全フレーム数）, NumX/NumY（分割コマ数）, interval（更新間隔）, scale（拡縮率） / 副作用：mnHandleListのメモリ確保
	TextureAnimation(
		std::string filename,
		VECTOR initPos,
		int allNum,
		int NumX,
		int NumY,
		int interval,
		float scale = 1.0f,
		bool type = true
	);  
	// 確保した画像ハンドルリストのメモリ領域を破棄
	// 副作用：mnHandleListのメモリ解放
	~TextureAnimation(); 

	// プレイヤーの移動向きを監視し、スプライトの左右反転フラグとアニメーションコマを更新
	// 副作用：反転フラグおよびアニメーションカウンタを更新
	void Update();  

	// カメラのスクロール座標を減算したスクリーン座標で、反転設定を反映して描画
	// 入力：cameraX, cameraY（カメラのワールド座標）
	void Draw(float cameraX = 0.0f, float cameraY = 0.0f);   

	// アニメーションを最初のフレームに戻し、再生をリスタート
	// 副作用：再生フレームの初期化
	void Reset(); 
	void SetPosition(VECTOR pos) { mvPosition = pos; } 

	int GetSizeX() { return mnSizeX; }    
	int GetSizeY() { return mnSizeY; }    

	float GetRadius() { return mfRadius; }    

	void SetScale(float scale) { mnScale = scale; }    
	float GetScale() { return mnScale; }    

	// アニメーションを1度だけ
	int IsEnd() { return mnCurrentNum >= mnAllNum - 1; }

private:
	VECTOR mvPosition;  
	int mnCounter;      
	int mnInterval;     
	int mnCurrentNum;   
	int* mnHandleList;  
	int mnAllNum;       
	int mnSizeX;       
	int mnSizeY;       
	float mfRadius = 60.0f;
	float mnScale;     
	bool mbPlayerReverseX;    
	bool mbEnemyReverseX;
	bool mbType;
  
};
