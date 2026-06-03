#include "Object2D.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Utility.h"


// コンストラクタ
Object2D::Object2D(std::string filename, VECTOR initPos)
	: mvPosition(initPos)
	, mbDeleteFlag(false)
	, mpTextureAnimation(nullptr)
{
	// 現在シーンの ObjectManager に自身（this）を追加する
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

	// 画像生成
	mpTexture = new Texture(filename, initPos, true);

}

// コンストラクタ（アニメーション用）
Object2D::Object2D(std::string filename, VECTOR initPos, int allNum, int numX, int numY, int interval, float scale, bool type)
	: mvPosition(initPos)
	, mbDeleteFlag(false)
	, mpTexture(nullptr)
{
	// 現在シーンの ObjectManager に自身（this）を追加する
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

	// 画像生成
	mpTextureAnimation = new TextureAnimation(filename, initPos, allNum, numX, numY, interval, scale, type);

}

// デストラクタ
Object2D::~Object2D()
{
	// 画像破棄
	if (mpTexture != nullptr)
	{
		delete mpTexture;
	}

	// 画像アニメーション破棄
	if (mpTextureAnimation != nullptr)
	{
		delete mpTextureAnimation;
	}

}

// 更新
void Object2D::Update()
{
	// 画像の更新
	if (mpTexture != nullptr)
	{
		mpTexture->Update();

		// 座標設定
		mpTexture->SetPosition(mvPosition);

	}

	// 画像アニメーションの更新
	if (mpTextureAnimation != nullptr)
	{
		mpTextureAnimation->Update();
		
		// 座標設定
		mpTextureAnimation->SetPosition(mvPosition);	
	}

}

// 描画
void Object2D::Draw()
{
	// 画像の描画
	if (mpTexture != nullptr)
	{
		mpTexture->Draw(gCameraX, gCameraY);
	}

	// 画像アニメーションの描画
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


// 半径の取得
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
	if (mpTexture != nullptr) return mpTexture->GetSizeX();
	return 0;
}

// 画像の横サイズを取得
int Object2D::GetSizeY()
{
	if (mpTexture != nullptr) return mpTexture->GetSizeY();
	return 0;
}