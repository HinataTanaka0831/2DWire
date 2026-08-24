#pragma once
#include "DxLib.h"
#include <string>

// スプライトシート分割画像の管理とコマ送りアニメーション描画を行うクラス
class TextureAnimation
{
public:
	// スプライトシート画像を分割ロードし、アニメーションリソースを初期化
	// 入力: filename(画像パス), initPos(初期座標), allNum(総コマ数), NumX/NumY(分割数), interval(更新間隔), scale(拡縮率), type(プレイヤー種別フラグ) / 出力: なし / 副作用: 画像配列の動的確保
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

	// 動的確保したグラフィックハンドル配列の破棄
	// 入力: なし / 出力: なし / 副作用: 画像配列メモリの解放
	~TextureAnimation(); 

	// フレーム進行とキャラクター向き（左右反転）状態の更新
	// 入力: なし / 出力: なし / 副作用: アニメーションコマと反転フラグの更新
	void Update();  

	// カメラ座標を加味した拡縮・回転・反転スプライト描画
	// 入力: cameraX, cameraY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
	void Draw(float cameraX = 0.0f, float cameraY = 0.0f);   

	// アニメーション再生位置を先頭コマにリセット
	// 入力: なし / 出力: なし / 副作用: カウンタおよび現在コマの初期化
	void Reset(); 

	void SetPosition(VECTOR pos) { mvPosition = pos; } 
	void SetReverse(bool rev) { mbEnemyReverseX = rev; }
	int GetSizeX() { return mnSizeX; }    
	int GetSizeY() { return mnSizeY; }    
	float GetRadius() { return mfRadius; }    
	void SetScale(float scale) { mnScale = scale; }    
	float GetScale() { return mnScale; }    

private:
	VECTOR mvPosition;        // 中心座標
	int mnCounter;            // フレーム進捗カウンタ
	int mnInterval;           // コマ切り替えフレーム間隔
	int mnCurrentNum;         // 現在再生中のフレームインデックス
	int* mnHandleList;        // 分割ロードされた画像ハンドル配列
	int mnAllNum;             // 総コマ数
	int mnSizeX = 0;          // 1コマあたりの横幅
	int mnSizeY = 0;          // 1コマあたりの縦幅
	float mfRadius = 90.0f;   // 当たり判定用半径
	float mnScale;            // 描画拡縮倍率
	bool mbPlayerReverseX;    // プレイヤー用左右反転フラグ
	bool mbEnemyReverseX;     // 敵キャラクター用左右反転フラグ
	bool mbType;              // true: プレイヤー / false: 敵
};