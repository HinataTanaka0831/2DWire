#pragma once

// ゲーム全体で共有される画面解像度定数およびグローバルステートの定義
class Utility
{
public:
	static const int SCREEN_WIDTH = 1920;   // ゲーム全体の基本描画解像度（横幅）
	static const int SCREEN_HEIGHT = 1080;  // ゲーム全体の基本描画解像度（縦幅）
};

// プレイヤー追従および描画オフセット計算に使用するグローバルカメラ座標
extern float gCameraX;
extern float gCameraY;

// シーン間をまたいで進行度を保持するための現在ステージ番号
extern int gCurrentStage;
// ステージ進行の上限値（これを超えると全ステージクリア判定）
static const int MaxStage = 2;