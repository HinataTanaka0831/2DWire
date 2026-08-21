#pragma once
#include "DxLib.h"
#include <string>

class Button
{
public:
	Button(int x1, int y1, int x2, int y2, std::string name, int color, int changeColor, int fontHandle);

	// ボタンの状態を更新する関数
	void Update();
	// ボタンを描画する関数
	void Draw();

	bool IsClick() const { return mbIsHover && mbIsClicked; }

private:
	// クラスが生成された際のボタンの座標、名前、色、フォントハンドルの情報を保持するメンバ変数
	int mnX1;
	int mnY1;
	int mnX2;
	int mnY2;
	std::string mName;
	int mnColor;
	int mnChangeColor;
	int mnFontHandle;
	
	int mnStringColor; // 文字列の色を追加
	float mfScale; // ボタンの拡大率を追加
	bool mbIsHover = false;   // マウスがボタン上にあるかどうかのフラグを追加
	bool mbIsEnabled = true; // ボタンが有効かどうかのフラグを追加
	bool mbIsClicked = false; // ボタンがクリックされたかどうかのフラグを追加
};