#pragma once
#include <DxLib.h>

// マウスのカーソル座標およびクリック状態（押下・トリガー・リリース）を管理するクラス
class MouseManager
{
public:
	MouseManager();

	// 毎フレームのマウス座標および入力状態のサンプリング
	// 入力: なし / 出力: なし / 副作用: 内部の座標・入力ビットフラグを更新
	static void MouseClick();

	static int GetMouseX() { return mouseX; }
	static int GetMouseY() { return mouseY; }
	static bool IsLeftDown() { return (mCurrentMouseInput & MOUSE_INPUT_LEFT); }
	static bool IsLeftTrigger() { return (mCurrentMouseInput & MOUSE_INPUT_LEFT) && !(mPreviousMouseInput & MOUSE_INPUT_LEFT); }
	static bool IsLeftRelease() { return !(mCurrentMouseInput & MOUSE_INPUT_LEFT) && (mPreviousMouseInput & MOUSE_INPUT_LEFT); }

private:
	static int mouseX;                 // 現在のマウスX座標
	static int mouseY;                 // 現在のマウスY座標
	static int mCurrentMouseInput;     // 現フレームのマウス入力ビットフラグ
	static int mPreviousMouseInput;    // 前フレームのマウス入力ビットフラグ（エッジ検出用）
};