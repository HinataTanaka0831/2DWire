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

	bool IsClick() const { return mbIsHover && mbIsClicked; }

private:
	int mnX1, mnY1, mnX2, mnY2;  // ボタンの矩形領域
	std::string mName;           // ボタンラベル文字列
	int mnColor;                 // 通常時・ホバー時の背景色
	int mnChangeColor;           // クリック時の背景色
	int mnFontHandle;            // ラベル描画用フォントハンドル
	
	int mnStringColor;           // 文字列カラー
	float mfScale;               // ホバー演出用拡大率
	bool mbIsHover = false;      // カーソルがボタン上にあるか
	bool mbIsEnabled = true;     // ボタンの操作有効フラグ
	bool mbIsClicked = false;    // クリックされたか
};