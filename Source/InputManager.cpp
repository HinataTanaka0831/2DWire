#include "InputManager.h"
#include "DxLib.h"

int InputManager::mDownBuffer[256] = { 0 };
int InputManager::mUpBuffer[256] = { 0 };

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

// 指定キーが押された瞬間かどうかを判定
// 入力: KeyCode(キーコード) / 出力: 押下瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
int InputManager::CheckDownKey(int KeyCode)
{
	int result = 0;
	int keyState = CheckHitKey(KeyCode);

	// 前フレームで非押下かつ現フレームで押下の場合にトリガー成立
	if(mDownBuffer[KeyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	mDownBuffer[KeyCode] = keyState;
	return result;
}

// 指定キーが離された瞬間かどうかを判定
// 入力: KeyCode(キーコード) / 出力: 離された瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
int InputManager::CheckUpKey(int KeyCode)
{
	int result = 0;
	int keyState = CheckHitKey(KeyCode);

	// 前フレームで押下かつ現フレームで非押下の場合にリリース成立
	if(mUpBuffer[KeyCode] == 1 && keyState == 0)
	{
		result = 1;
	}

	mUpBuffer[KeyCode] = keyState;
	return result;
}

// 指定キーが押し続けられているかを判定
// 入力: KeyCode(キーコード) / 出力: 押下中なら1、それ以外は0 / 副作用: なし
int InputManager::CheckPressKey(int KeyCode)
{
	return CheckHitKey(KeyCode);
}