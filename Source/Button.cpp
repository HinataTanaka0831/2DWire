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

void Button::Update()
{
	// マウスがボタンの範囲内にあるかどうかを判定する。
	// マウスがボタンの範囲内にある場合は mbIsHover を true に設定し、そうでない場合は false に設定する。
	if (!mbIsEnabled)
	{
		mbIsHover = false;
		return;
	}

	int mouseX = MouseManager::GetMouseX();
	int mouseY = MouseManager::GetMouseY();

	if (mouseX >= mnX1 && mouseX <= mnX2 && mouseY >= mnY1 && mouseY <= mnY2)
	{
		mbIsHover = true;
	}
	else
	{
		mbIsHover = false;
	}

}

void Button::Draw()
{
	// ボタンの描画色を決定する。
	// ボタンが無効な場合は暗い色、ホバー中は明るい色、通常時は標準色を使用する。
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

		if (MouseManager::IsLeftDown)
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
		drawColor = GetColor(0, 0, 0);
		mnStringColor = GetColor(255, 255, 255);
	}

	// ボタンの中心座標とサイズを計算する。
	// ボタンの描画位置を計算する。
	// ボタンの描画位置は、ボタンの中心座標から拡大率を考慮して計算する。
	float centerX = (mnX1 + mnX2) / 2.0f;
	float centerY = (mnY1 + mnY2) / 2.0f;
	float width = (float)mnX2 - mnX1;
	float height = (float)mnY2 - mnY1;

	float drawX1 = centerX - (width * mfScale) / 2.0f;
	float drawY1 = centerY - (height * mfScale) / 2.0f;
	float drawX2 = centerX + (width * mfScale) / 2.0f;
	float drawY2 = centerY + (height * mfScale) / 2.0f;

	int stringWidth = GetDrawStringWidth(mName.c_str(), mName.length());

	DrawBox((int)drawX1, (int)drawY1, (int)drawX2, (int)drawY2, drawColor, TRUE);
	DrawFormatStringToHandle((int)(centerX - stringWidth / 2), (int)(centerY - 10), mnStringColor, mnFontHandle, "%s", mName.c_str());
}