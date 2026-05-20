#include "Object2D.h"
#include "Texture.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Utility.h"


// コンストラクタ
Object2D::Object2D(std::string filename, VECTOR initPos)
	: mvPosition(initPos)
	, mbDeleteFlag(false)
{
	// 現在シーンの ObjectManager に自身（this）を追加する
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

	// 画像生成
	mpTexture = new Texture(filename, initPos, true);



}

// デストラクタ
Object2D::~Object2D()
{
	// 画像破棄
	if (mpTexture != nullptr)
	{
		delete mpTexture;
	}
}

// 更新
void Object2D::Update()
{
	if (mpTexture != nullptr)
	{
		mpTexture->Update();
	}

	// 座標設定
	mpTexture->SetPosition(mvPosition);
}

// 描画
void Object2D::Draw()
{
	if (mpTexture != nullptr)
	{
		mpTexture->Draw(gCameraX, 0.0f);
	}
}



// 半径の取得
float Object2D::GetRadius()
{
      return mpTexture->GetRadius();
}

int Object2D::GetSizeX()
{
	if (mpTexture) return mpTexture->GetSizeX();
	return 0;
}

int Object2D::GetSizeY()
{
	if (mpTexture) return mpTexture->GetSizeY();
	return 0;
}