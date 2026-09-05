#pragma once
#include "Object2D.h"
#include "DxLib.h"

// ステージクリアを判定するためのゴールゲートオブジェクト
class Goal : public Object2D
{
public:
	// ゴール領域の生成
	// 入力: initPos(中心座標), width(横幅), height(縦幅) / 出力: なし / 副作用: Goal2Dタグの設定
	Goal(VECTOR initPos, int width, int height);
	~Goal();

	// 演出用カラーサイクルの更新
	// 入力: なし / 出力: なし / 副作用: mColorStateの加算
	void Update() override;

	// カメラ座標を加味したゴールゲートおよび「GOAL」テキストの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

private:
	int mWidth;       // ゴール領域の横幅
	int mHeight;      // ゴール領域の縦幅
	int mColorState;  // ゴール枠のアニメーション点滅用カウンタ
};