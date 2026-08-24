#include "MouseManager.h"

int MouseManager::mouseX = 0;
int MouseManager::mouseY = 0;
int MouseManager::mCurrentMouseInput = 0;
int MouseManager::mPreviousMouseInput = 0;

MouseManager::MouseManager()
{
}

// 毎フレームのマウス座標および入力状態のサンプリング
// 入力: なし / 出力: なし / 副作用: 内部の座標・入力ビットフラグを更新
void MouseManager::MouseClick()
{
	int mx, my;
	GetMousePoint(&mx, &my);

	mouseX = mx;
	mouseY = my;

	// 前フレーム状態を退避し、トリガー/リリース判定を可能にする
	mPreviousMouseInput = mCurrentMouseInput;
	mCurrentMouseInput = GetMouseInput();
}