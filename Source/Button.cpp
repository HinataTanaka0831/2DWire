#include "Button.h"
#include "MouseManager.h"

Button::Button(int x1, int y1, int x2, int y2, std::string name, int color, int changeColor, int fontHandle)
	: mnX1(x1)
	, mnY1(y1)
	, mnX2(x2)
	, mnY2(y2)
	, mName(name)
	, mnColor(color)
	, mnChangeColor(changeColor)
	, mnFontHandle(fontHandle)
	, mnStringColor(GetColor(255, 255, 255))
	, mfScale(1.0f)
{
}

// マウスカーソルの包含判定とホバー状態の更新
// 入力: なし / 出力: なし / 副作用: mbIsHoverフラグの更新
void Button::Update()
{
	if (!mbIsEnabled)
	{
		mbIsHover = false;
		return;
	}

	int mouseX = (int)MouseManager::GetMouseX();
	int mouseY = (int)MouseManager::GetMouseY();

	if (mouseX >= mnX1 && mouseX <= mnX2 && mouseY >= mnY1 && mouseY <= mnY2)
	{
		mbIsHover = true;
	}
	else
	{
		mbIsHover = false;
	}
}

// 状態に応じたボタン矩形および文字列の描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Button::Draw()
{
	unsigned int drawColor{};

	if (!mbIsEnabled)
	{
		drawColor = GetColor(10, 10, 10);
		mfScale = 1.0f;
		mnStringColor = GetColor(80, 80, 80);
	}
	else if (mbIsHover)
	{
		mfScale = 1.1f;

		// 左クリック押下時にクリック状態へ遷移し背景色を変更
		if (MouseManager::CheckTriggerMouseClick(MOUSE_INPUT_LEFT))
		{
			mbIsClicked = true;
			drawColor = mnChangeColor;
			mfScale = 1.0f;
		}
		else
		{
			mfScale = 1.1f;
			drawColor = mnColor;
		}
	}
	else
	{
		mfScale = 1.0f;
		drawColor = GetColor(30, 30, 30);
		mnStringColor = GetColor(255, 255, 255);
	}

	// 拡縮中心がボタン中心となるよう矩形を再計算
	float centerX = (mnX1 + mnX2) / 2.0f;
	float centerY = (mnY1 + mnY2) / 2.0f;
	float width = (float)mnX2 - mnX1;
	float height = (float)mnY2 - mnY1;

	float drawX1 = centerX - (width * mfScale) / 2.0f;
	float drawY1 = centerY - (height * mfScale) / 2.0f;
	float drawX2 = centerX + (width * mfScale) / 2.0f;
	float drawY2 = centerY + (height * mfScale) / 2.0f;

	int stringWidth = GetDrawStringWidth(mName.c_str(), (int)mName.length());

	DrawBox((int)drawX1, (int)drawY1, (int)drawX2, (int)drawY2, drawColor, TRUE);
	DrawFormatStringToHandle((int)(centerX - stringWidth / 2), (int)(centerY - 10), mnStringColor, mnFontHandle, "%s", mName.c_str());
}