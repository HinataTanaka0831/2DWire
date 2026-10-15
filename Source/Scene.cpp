#include "Scene.h"
#include "ObjectManager.h"
#include "Master.h"

Scene::Scene()
	: m_fontSize20(CreateFontToHandle(NULL, 20, -1, -1))
	, m_fontSize50(CreateFontToHandle(NULL, 50, -1, -1))
	, m_fontSize90(CreateFontToHandle(NULL, 90, -1, -1))
	, m_objectManager(nullptr)

{
	// 各シーンごとに独立したオブジェクト管理領域を生成
	m_objectManager = new ObjectManager();
}

Scene::~Scene()
{
	// シーン破棄時に管理下のObjectManagerを確実に破棄
	if (m_objectManager != nullptr)
	{
		delete m_objectManager;
		m_objectManager = nullptr;
	}
}

void Scene::Update()
{
	if (m_objectManager != nullptr)
	{
		m_objectManager->Update();
	}
}

void Scene::Draw()
{
	if (m_objectManager != nullptr)
	{
		m_objectManager->Draw();
	}
}