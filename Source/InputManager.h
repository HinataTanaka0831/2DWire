#pragma once

// キーボードやマウス入力の状態（押下瞬間・離脱瞬間・長押し）を管理する補助クラス
class InputManager
{
public:
	// シングルトンインスタンスの取得
	// 入力: なし / 出力: InputManagerのシングルトンインスタンス / 副作用: なし
	static InputManager& GetInstance();

	// 指定キーが押された瞬間かどうかを判定
	// 入力: keyCode(キーコード) / 出力: 押下瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
	int CheckDownKey(int keyCode);

	// 指定キーが離された瞬間かどうかを判定
	// 入力: keyCode(キーコード) / 出力: 離された瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
	int CheckUpKey(int keyCode);

	// 指定キーが押し続けられているかを判定
	// 入力: KeyCode(キーコード) / 出力: 押下中なら1、それ以外は0 / 副作用: なし
	int CheckPressKey(int KeyCode);


	//----- マウス入力関連 -----//
	// 毎フレームのマウス座標および入力状態のサンプリング
    // 入力: なし / 出力: なし / 副作用: 内部の座標・入力ビットフラグを更新
	void MouseUpdate();

	// 指定されたマウスがが押し続けられているかどうかを判定
	// 入力: mouseCode(マウスコード) / 出力: 押下中ならtrue、それ以外はfalse / 副作用: なし
	bool CheckPressMouseClick(int mouseCode);

	// 指定されたマウスが押された瞬間かどうかを判定
	// 入力: mouseCode(マウスコード) / 出力: 押下瞬間ならtrue、それ以外はfalse / 副作用: なし
	bool CheckTriggerMouseClick(int mouseCode);

	// 指定されたマウスが離された瞬間かどうかを判定
	// 入力: mouseCode(マウスコード) / 出力: 離された瞬間ならtrue、それ以外はfalse / 副作用: なし
	bool CheckReleaseMouseClick(int mouseCode);

	float GetMouseX() { return m_mouseX; }
	float GetMouseY() { return m_mouseY; }

private:
	InputManager();
	~InputManager();

	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager&&) = delete;

private:
	int m_downBuffer[256] = { 0 };	    // 押下エッジ検出用の前フレームキー状態保持バッファ
	int m_upBuffer[256] = { 0 };		// 離脱エッジ検出用の前フレームキー状態保持バッファ

	float m_mouseX = 0.0f;                 // 現在のマウスX座標
	float m_mouseY = 0.0f;                 // 現在のマウスY座標
	int m_currentMouseInput = 0;     // 現フレームのマウス入力ビットフラグ
	int m_previousMouseInput = 0;    // 前フレームのマウス入力ビットフラグ（エッジ検出用）

};