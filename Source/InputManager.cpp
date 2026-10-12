#include "InputManager.h"
#include "DxLib.h"

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

InputManager::InputManager()
	: m_downBuffer{0}
	, m_upBuffer{0}
    , m_mouseX(0.0f)
	, m_mouseY(0.0f)
	, m_currentMouseInput(0)
	, m_previousMouseInput(0)

{
}

InputManager::~InputManager()
{
}

// 指定キーが押された瞬間かどうかを判定
// 入力: keyCode(キーコード) / 出力: 押下瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
int InputManager::CheckDownKey(int keyCode)
{
	int result = 0;
	int keyState = CheckHitKey(keyCode);

	// 前フレームで非押下かつ現フレームで押下の場合にトリガー成立
	if(m_downBuffer[keyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	m_downBuffer[keyCode] = keyState;
	return result;
}

// 指定キーが離された瞬間かどうかを判定
// 入力: keyCode(キーコード) / 出力: 離された瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
int InputManager::CheckUpKey(int keyCode)
{
	int result = 0;
	int keyState = CheckHitKey(keyCode);

	// 前フレームで押下かつ現フレームで非押下の場合にリリース成立
	if(m_upBuffer[keyCode] == 1 && keyState == 0)
	{
		result = 1;
	}

	m_upBuffer[keyCode] = keyState;
	return result;
}

// 指定キーが押し続けられているかを判定
// 入力: keyCode(キーコード) / 出力: 押下中なら1、それ以外は0 / 副作用: なし
int InputManager::CheckPressKey(int keyCode)
{
	return CheckHitKey(keyCode);
}


//----- マウス入力関連 -----//

// 毎フレームのマウス座標および入力状態のサンプリング
// 入力: なし / 出力: なし / 副作用: 内部の座標・入力ビットフラグを更新
void InputManager::MouseUpdate()
{
	int mx, my;
	GetMousePoint(&mx, &my);

	m_mouseX = (float)mx;
	m_mouseY = (float)my;

	m_previousMouseInput = m_currentMouseInput;
	m_currentMouseInput = GetMouseInput();
}

// 指定されたマウスがが押し続けられているかどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 押下中ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckPressMouseClick(int mouseCode)
{
	bool result = false;

	if (m_currentMouseInput & mouseCode)
	{
		result = true;
	}

	return result;
}

// 指定されたマウスが押された瞬間かどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 押下瞬間ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckTriggerMouseClick(int mouseCode)
{
	bool result = false;

	if ((m_currentMouseInput & mouseCode) && !(m_previousMouseInput & mouseCode))
	{
		result = true;
	}

	return result;
}

// 指定されたマウスが離された瞬間かどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 離された瞬間ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckReleaseMouseClick(int mouseCode)
{
	bool result = false;

	if (!(m_currentMouseInput & mouseCode) && (m_previousMouseInput & mouseCode))
	{
		result = true;
	}

	return result;
}