#pragma once
#include "DxLib.h"
#include <string>

// マウスホバー・クリック検知および視覚演出（拡大・色変化）を持つUIボタンクラス
class Button
{
public:
	// ボタンの矩形領域・ラベル・配色の初期化
	// 入力: x1, y1, x2, y2(矩形座標), name(表示文字列), color, changeColor, fontHandle / 出力: なし / 副作用: 各種プロパティの設定
	Button(int x1, int y1, int x2, int y2, std::string name, int color, int changeColor, int fontHandle);

	// マウスカーソルの包含判定とホバー状態の更新
	// 入力: なし / 出力: なし / 副作用: mbIsHoverフラグの更新
	void Update();

	// 状態（通常・ホバー・クリック中・無効）に応じたボタン矩形および文字列の描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw();

	bool IsClick() const { return m_isHover && m_isClicked; }

private:
	int m_x1, m_y1, m_x2, m_y2;  // ボタンの矩形領域
	std::string m_name;           // ボタンラベル文字列
	int m_color;                 // 通常時・ホバー時の背景色
	int m_changeColor;           // クリック時の背景色
	int m_fontHandle;            // ラベル描画用フォントハンドル
	
	int m_stringColor;           // 文字列カラー
	float m_scale;               // ホバー演出用拡大率
	bool m_isHover = false;      // カーソルがボタン上にあるか
	bool m_isEnabled = true;     // ボタンの操作有効フラグ
	bool m_isClicked = false;    // クリックされたか
};