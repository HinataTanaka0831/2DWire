#pragma once
#include <DxLib.h>


class MouseManager
{
public:

	MouseManager();

	static void MouseClick();  // マウスの状態を更新する関数

	// マウスの状態を取得する関数
	static int GetMouseX() { return mouseX; }  // マウスのX座標を取得する関数
	static int GetMouseY() { return mouseY; }  // マウスのY座標を取得する関数
	static bool IsLeftDown() { return (mCurrentMouseInput & MOUSE_INPUT_LEFT); }                                                    // 左クリックが押されているかどうかの状態を判定
	static bool IsLeftTrigger() { return (mCurrentMouseInput & MOUSE_INPUT_LEFT) && !(mPreviousMouseInput & MOUSE_INPUT_LEFT); }   // 左クリックが押された瞬間の状態を判定
	static bool IsLeftRelease() { return  !(mCurrentMouseInput & MOUSE_INPUT_LEFT) && (mPreviousMouseInput & MOUSE_INPUT_LEFT); }   // 左クリックが離された瞬間の状態を判定


private:
	// マウスのカーソル位置を保持する変数
	static int mouseX;                 // マウスのX座標を保持する変数
	static int mouseY;                 // マウスのY座標を保持する変数
	static int mCurrentMouseInput;    	// マウスの現在の状態を保持する変数
	static int mPreviousMouseInput;    // マウスの前回の状態を保持する変数

};