#include "MouseManager.h"

int MouseManager::mouseX = 0;
int MouseManager::mouseY = 0;
int MouseManager::mCurrentMouseInput = 0;
int MouseManager::mPreviousMouseInput = 0;

MouseManager::MouseManager()
{

}


void MouseManager::MouseClick()
{
	// マウスの座標を取得して、マウスの状態を更新する
	int mx, my;
	GetMousePoint(&mx, &my);

	mouseX = (float)mx;
	mouseY = (float)my;

	mPreviousMouseInput = mCurrentMouseInput;

	mCurrentMouseInput = GetMouseInput();

}