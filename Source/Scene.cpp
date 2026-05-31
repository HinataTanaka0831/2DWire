#include "Scene.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Bullet.h"


// コンストラクタ
Scene::Scene()
{
	// オブジェクトマネージャーの生成
	mpObjectManager = new ObjectManager();
}

// デストラクタ
Scene::~Scene()
{
	if (mpObjectManager != nullptr)
	{
		delete mpObjectManager;
	}
}

// 更新
void Scene::Update()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Update();
	}

}

// 描画
void Scene::Draw()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Draw();
	}

}

