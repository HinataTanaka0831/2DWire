#include "Object2D.h"
#include "Texture.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"


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
		mpTexture->Draw();
	}
}



// 半径の取得
float Object2D::GetRadius()
{
      return mpTexture->GetRadius();
}