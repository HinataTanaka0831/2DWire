#include "Scene.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Bullet.h"

Scene::Scene()
{
	// 各シーンごとに独立したオブジェクト管理領域を生成
	mpObjectManager = new ObjectManager();
}

Scene::~Scene()
{
	// シーン破棄時に管理下のObjectManagerを確実に破棄
	if (mpObjectManager != nullptr)
	{
		delete mpObjectManager;
		mpObjectManager = nullptr;
	}
}

// シーン内の全オブジェクトの更新
// 入力: なし / 出力: なし / 副作用: mpObjectManager->Update()呼び出し
void Scene::Update()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Update();
	}
}

// シーン内の全オブジェクトの描画
// 入力: なし / 出力: なし / 副作用: mpObjectManager->Draw()呼び出し
void Scene::Draw()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Draw();
	}
}