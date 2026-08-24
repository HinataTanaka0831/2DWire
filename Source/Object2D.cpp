#include "Object2D.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Utility.h"

// 単一静止画テクスチャを持つオブジェクトの生成
// 入力: filename(画像パス), initPos(初期座標) / 出力: なし / 副作用: ObjectManagerへの自動登録
Object2D::Object2D(std::string filename, VECTOR initPos)
	: mvPosition(initPos)
	, mbDeleteFlag(false)
	, mpTextureAnimation(nullptr)
{
	// シーン管理下へ自身を登録し、一括更新・描画の対象にする
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
	mpTexture = new Texture(filename, initPos, true);
}

// スプライトシートアニメーションを持つオブジェクトの生成
// 入力: filename, initPos, allNum, numX, numY, interval, scale, type / 出力: なし / 副作用: ObjectManagerへの自動登録
Object2D::Object2D(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type)
	: mvPosition(initPos)
	, mbDeleteFlag(false)
	, mpTexture(nullptr)
{
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
	mpTextureAnimation = new TextureAnimation(filename, initPos, allNum, numX, numY, interval, scale, type);
}

// 保持する画像リソースの破棄
// 入力: なし / 出力: なし / 副作用: テクスチャメモリの解放
Object2D::~Object2D()
{
	if (mpTexture != nullptr)
	{
		delete mpTexture;
	}

	if (mpTextureAnimation != nullptr)
	{
		delete mpTextureAnimation;
	}
}

// 毎フレームの座標更新や内部状態の進行
// 入力: なし / 出力: なし / 副作用: 座標やテクスチャの更新
void Object2D::Update()
{
	if (mpTexture != nullptr)
	{
		mpTexture->Update();
		mpTexture->SetPosition(mvPosition);
	}

	if (mpTextureAnimation != nullptr)
	{
		mpTextureAnimation->Update();
		mpTextureAnimation->SetPosition(mvPosition);	
	}
}

// カメラ座標を加味した画面描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Object2D::Draw()
{
	if (mpTexture != nullptr)
	{
		mpTexture->Draw(gCameraX, gCameraY);
	}

	if (mpTextureAnimation != nullptr)
	{
		mpTextureAnimation->Draw(gCameraX, gCameraY);
	}
}

void Object2D::Reset()
{
	if (mpTextureAnimation != nullptr)
	{
		mpTextureAnimation->Reset();
	}
}

// 当たり判定等で使用する近似半径の取得
// 入力: なし / 出力: 半径(px) / 副作用: なし
float Object2D::GetRadius()
{
	if (mpTexture != nullptr)
	{
		return mpTexture->GetRadius();
	}

	if (mpTextureAnimation != nullptr)
	{
		return mpTextureAnimation->GetRadius();
	}

	return 0.0f;
}

int Object2D::GetSizeX()
{
	if (mpTexture != nullptr)
	{
		return mpTexture->GetSizeX();
	}

	if (mpTextureAnimation != nullptr)
	{
		return mpTextureAnimation->GetSizeX();
	}

	return 0;
}

int Object2D::GetSizeY()
{
	if (mpTexture != nullptr)
	{
		return mpTexture->GetSizeY();
	}

	if (mpTextureAnimation != nullptr)
	{
		return mpTextureAnimation->GetSizeY();
	}

	return 0;
}