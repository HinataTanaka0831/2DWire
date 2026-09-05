#pragma once

#include "DxLib.h"
#include <string>
#include "SelectScene.h"

class Texture;
class TextureAnimation;

// ゲーム内の全2Dエンティティ（プレイヤー、敵、弾、ギミック等）の基底クラス
class Object2D 
{
public:
	// 衝突判定やシーン横断検索でオブジェクト種別を識別するためのタグ
	enum Tag
	{
		Player2D = 2000,
		Enemy2D = 2100,
		BossEnemy2D = 2200,
		Bullet2D = 2200,
		HomingBullet2D = 2201,
		WireTarget2D = 2300,
		Goal2D = 2400,
		Ground2D = 2500,
	};

public:
	// 単一静止画テクスチャを持つオブジェクトの生成
	// 入力: filename(画像パス), initPos(初期座標) / 出力: なし / 副作用: ObjectManagerへの自動登録
	Object2D(std::string filename, VECTOR initPos);

	// スプライトシートアニメーションを持つオブジェクトの生成
	// 入力: filename, initPos, allNum, numX, numY, interval, scale, type / 出力: なし / 副作用: ObjectManagerへの自動登録
	Object2D(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale = 1.0f, bool type = true);

	// 保持する画像リソースの破棄
	// 入力: なし / 出力: なし / 副作用: テクスチャメモリの解放
	virtual ~Object2D();

	// 毎フレームの座標更新や内部状態の進行
	// 入力: なし / 出力: なし / 副作用: 座標やテクスチャの更新
	virtual void Update();

	// カメラ座標を加味した画面描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	virtual void Draw();

	void Reset();

public:
	void SetPosition(VECTOR pos) { mvPosition = pos; }
	VECTOR GetPosition() { return mvPosition; }
	void SetDeleteFlag(bool flag) { mbDeleteFlag = flag; }
	bool IsDeleteFlag() { return mbDeleteFlag; }
	void SetTag(Tag tag) { mnTag = tag; }
	Tag GetTag() { return mnTag; }

	float GetRadius();
	int GetSizeX();
	int GetSizeY();

protected:
	Texture* mpTexture;                    // 静止画描画用テクスチャ
	TextureAnimation* mpTextureAnimation;  // 連番アニメーション描画用テクスチャ
	VECTOR mvPosition;                     // ワールド座標
	VECTOR mvDirection;                    // 移動方向ベクトル

private:
	bool mbDeleteFlag;                     // フレーム終了時の安全削除対象フラグ
	Tag mnTag;                             // 種別識別用タグ
};