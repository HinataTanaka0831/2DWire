#pragma once
#include <DxLib.h>

// マウスのカーソル座標およびクリック状態（押下・トリガー・リリース）を管理するクラス
class MouseManager
{
public:
	MouseManager();
	~MouseManager();

	// 毎フレームのマウス座標および入力状態のサンプリング
	// 入力: なし / 出力: なし / 副作用: 内部の座標・入力ビットフラグを更新
	static void MouseUpdate();

	// 指定されたマウスがが押し続けられているかどうかを判定
    // 入力: mouseCode(マウスコード) / 出力: 押下中ならtrue、それ以外はfalse / 副作用: なし
	static bool CheckPressMouseClick(int mouseCode);

	// 指定されたマウスが押された瞬間かどうかを判定
    // 入力: mouseCode(マウスコード) / 出力: 押下瞬間ならtrue、それ以外はfalse / 副作用: なし
	static bool CheckTriggerMouseClick(int mouseCode);

	// 指定されたマウスが離された瞬間かどうかを判定
    // 入力: mouseCode(マウスコード) / 出力: 離された瞬間ならtrue、それ以外はfalse / 副作用: なし
	static bool CheckReleaseMouseClick(int mouseCode);

	static float GetMouseX() { return MouseX; }
	static float GetMouseY() { return MouseY; }

private:
	static float MouseX;                 // 現在のマウスX座標
	static float MouseY;                 // 現在のマウスY座標
	static int CurrentMouseInput;     // 現フレームのマウス入力ビットフラグ
	static int PreviousMouseInput;    // 前フレームのマウス入力ビットフラグ（エッジ検出用）
};