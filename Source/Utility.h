#pragma once

// プログラム内で共通して使用する定数定義などをここに記述していく
// 関数なども記述していく

class Utility
{
public:  // 定数定義
	static const int SCREEN_WIDTH = 1920;   // 画面の幅
	static const int SCREEN_HEIGHT = 1080; // 画面の高さ
};

// グローバルなカメラX座標（スクロール用）とY座標（縦スクロール用）を定義
extern float gCameraX;
extern float gCameraY;