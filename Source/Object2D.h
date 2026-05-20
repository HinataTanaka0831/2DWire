#pragma once

#include "DxLib.h"
#include <string>
#include "SelectScene.h"


// クラスの前方宣言
class Texture;



//
// 2Dオブジェクトの基底クラス
// 2Dのオブジェクト（プレイヤー敵など）を何か作る際は、
// 必ずこれを継承して作成する

class Object2D 
{
public:     // enum, struct, 定数の定義
	// オブジェクトを見分けるためのタグ
	enum Tag
	{
		// Titleシーンで使われるタグ（1000～）
		
		
		// Gameシーンで使われるタグ（2000～）
		Player2D = 2000,
		Enemy2D = 2100,
		Enemy22D = 2101,
		BossEnemy2D = 2200,
		Bullet2D = 2200,
		HomingBullet2D = 2201,
		WireTarget2D = 2300,

		// Resultシーンで使われるタグ（3000～）
	};


public:
	// コンストラクタ
	Object2D(std::string filename, VECTOR initPos);
	// デストラクタ
	virtual ~Object2D();



	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();



public:      // ゲッター・セッター
	void SetPosition(VECTOR pos) { mvPosition = pos; }  // 座標設定
	VECTOR GetPosition() { return mvPosition; }         // 座標取得

	void SetDeleteFlag(bool flag) { mbDeleteFlag = flag; }  // 削除フラグ設定
	bool IsDeleteFlag() { return mbDeleteFlag; }         // 削除フラグ取得

	void SetTag(Tag tag) { mnTag = tag; }   // タグ設定
	Tag GetTag() { return mnTag; }          // タグ取得

	float GetRadius();   // 半径の取得
	int GetSizeX();
	int GetSizeY();


protected:
	Texture* mpTexture;     // 画像
	VECTOR mvPosition;      // 座標
	VECTOR mvDirection;  // 移動方向
	float mfAngle;       // 現在の目標角度
	int count;


private:
	bool mbDeleteFlag;     // 削除フラグ（これがtrueになっていると自動的に削除される（ように作る））
	Tag mnTag;             // オブジェクトを見分ける用のタグ
};
