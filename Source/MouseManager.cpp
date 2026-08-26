#include "MouseManager.h"

int MouseManager::MouseX = 0;
int MouseManager::MouseY = 0;
int MouseManager::PreviousMouseInput = 0;
int MouseManager::CurrentMouseInput = 0;


MouseManager::MouseManager()
{
}

// 毎フレームのマウス座標および入力状態のサンプリング
// 入力: なし / 出力: なし / 副作用: 内部の座標・入力ビットフラグを更新
void MouseManager::MouseUpdate()
{
	int mx, my;
	GetMousePoint(&mx, &my);

	MouseX = mx;
	MouseY = my;

	PreviousMouseInput = CurrentMouseInput;
	CurrentMouseInput = GetMouseInput();
}

// 指定されたマウスがが押し続けられているかどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 押下中ならtrue、それ以外はfalse / 副作用: なし
bool MouseManager::CheckPressMouseClick(int mouseCode)
{
	bool result = false;

	if (CurrentMouseInput & mouseCode)
	{
		result = true;
	}

	return result;
}

// 指定されたマウスが押された瞬間かどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 押下瞬間ならtrue、それ以外はfalse / 副作用: なし
bool MouseManager::CheckTriggerMouseClick(int mouseCode)
{
	bool result = false;

	if ((CurrentMouseInput & mouseCode) && !(PreviousMouseInput & mouseCode))
	{
		result = true;
	}

	return result;
}

// 指定されたマウスが離された瞬間かどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 離された瞬間ならtrue、それ以外はfalse / 副作用: なし
bool MouseManager::CheckReleaseMouseClick(int mouseCode)
{
	bool result = false;

	if (!(CurrentMouseInput & mouseCode) && (PreviousMouseInput & mouseCode))
	{
		result = true;
	}

	return result;
}