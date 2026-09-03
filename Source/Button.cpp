#include "Button.h"
#include "MouseManager.h"

Button::Button(int x1, int y1, int x2, int y2, std::string name, int color, int changeColor, int fontHandle)
	: m_x1(x1)
	, m_y1(y1)
	, m_x2(x2)
	, m_y2(y2)
	, m_name(name)
	, m_color(color)
	, m_changeColor(changeColor)
	, m_fontHandle(fontHandle)
	, m_stringColor(GetColor(255, 255, 255))
	, m_scale(1.0f)
{
}

// マウスカーソルの包含判定とホバー状態の更新
// 入力: なし / 出力: なし / 副作用: mbIsHoverフラグの更新
void Button::Update()
{
	if (!m_isEnabled)
	{
		m_isHover = false;
		return;
	}

	int mouseX = MouseManager::GetMouseX();
	int mouseY = MouseManager::GetMouseY();

	if (mouseX >= m_x1 && mouseX <= m_x2 && mouseY >= m_y1 && mouseY <= m_y2)
	{
		m_isHover = true;
	}
	else
	{
		m_isHover = false;
	}
}

// 状態に応じたボタン矩形および文字列の描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Button::Draw()
{
	unsigned int drawColor{};

	if (!m_isEnabled)
	{
		drawColor = GetColor(10, 10, 10);
		m_scale = 1.0f;
		m_stringColor = GetColor(80, 80, 80);
	}
	else if (m_isHover)
	{
		m_scale = 1.1f;

		// 左クリック押下時にクリック状態へ遷移し背景色を変更
		if (MouseManager::CheckPressMouseClick(MOUSE_INPUT_LEFT))
		{
			m_isClicked = true;
			drawColor = m_changeColor;
			m_scale = 1.0f;
		}
		else
		{
			m_scale = 1.1f;
			drawColor = m_color;
		}
	}
	else
	{
		m_scale = 1.0f;
		drawColor = GetColor(0, 0, 0);
		m_stringColor = GetColor(255, 255, 255);
	}

	// 拡縮中心がボタン中心となるよう矩形を再計算
	float centerX = (m_x1 + m_x2) / 2.0f;
	float centerY = (m_y1 + m_y2) / 2.0f;
	float width = (float)m_x2 - m_x1;
	float height = (float)m_y2 - m_y1;

	float drawX1 = centerX - (width * m_scale) / 2.0f;
	float drawY1 = centerY - (height * m_scale) / 2.0f;
	float drawX2 = centerX + (width * m_scale) / 2.0f;
	float drawY2 = centerY + (height * m_scale) / 2.0f;

	int stringWidth = GetDrawStringWidth(m_name.c_str(), (int)m_name.length());

	DrawBox((int)drawX1, (int)drawY1, (int)drawX2, (int)drawY2, drawColor, TRUE);
	DrawFormatStringToHandle((int)(centerX - stringWidth / 2), (int)(centerY - 10), m_stringColor, m_fontHandle, "%s", m_name.c_str());
}