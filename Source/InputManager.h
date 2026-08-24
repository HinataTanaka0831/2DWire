#pragma once

// キーボード入力の状態（押下瞬間・離脱瞬間・長押し）を管理する補助クラス
class InputManager
{
public:
	InputManager();
	~InputManager();

	// 指定キーが押された瞬間かどうかを判定
	// 入力: KeyCode(キーコード) / 出力: 押下瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
	static int CheckDownKey(int KeyCode);

	// 指定キーが離された瞬間かどうかを判定
	// 入力: KeyCode(キーコード) / 出力: 離された瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
	static int CheckUpKey(int KeyCode);

	// 指定キーが押し続けられているかを判定
	// 入力: KeyCode(キーコード) / 出力: 押下中なら1、それ以外は0 / 副作用: なし
	static int CheckPressKey(int KeyCode);

private:
	static int mDownBuffer[256];	// 押下エッジ検出用の前フレームキー状態保持バッファ
	static int mUpBuffer[256];		// 離脱エッジ検出用の前フレームキー状態保持バッファ
};