#pragma once
#include "DxLib.h"
#include <string>

// スプライトシート分割画像の管理とコマ送りアニメーション描画を行うクラス
class TextureAnimation
{
public:
	// スプライトシート画像を分割ロードし、アニメーションリソースを初期化
	// 入力: fileName(画像パス), initPosition(初期座標), allNum(総コマ数), numX/numY(分割数), interval(更新間隔), scale(拡縮率), type(プレイヤー種別フラグ) / 出力: なし / 副作用: 画像配列の動的確保
	TextureAnimation(
		std::string fileName,
		VECTOR initPosition,
		int allNum,
		int numX,
		int numY,
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

	void SetPosition(VECTOR position) { m_position = position; } 
	void SetReverse(bool reverse) { m_enemyReverseX = reverse; }
	int GetSizeX() { return m_sizeX; }    
	int GetSizeY() { return m_sizeY; }    
	float GetRadius() { return m_radius; }    
	void SetScale(float scale) { m_scale = scale; }    
	float GetScale() { return m_scale; }    

private:
	VECTOR m_position;        // 中心座標
	int m_counter;            // フレーム進捗カウンタ
	int m_interval;           // コマ切り替えフレーム間隔
	int m_currentNum;         // 現在再生中のフレームインデックス
	int* m_handleList;        // 分割ロードされた画像ハンドル配列
	int m_allNum;             // 総コマ数
	int m_sizeX = 0;          // 1コマあたりの横幅
	int m_sizeY = 0;          // 1コマあたりの縦幅
	float m_radius = 90.0f;   // 当たり判定用半径
	float m_scale;            // 描画拡縮倍率
	bool m_playerReverseX;    // プレイヤー用左右反転フラグ
	bool m_enemyReverseX;     // 敵キャラクター用左右反転フラグ
	bool m_type;              // true: プレイヤー / false: 敵
};