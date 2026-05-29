#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "GameRuleScene.h"
#include "GameOverScene.h"

SceneManager::SceneManager()
: mnSceneType(SCENE_TYPE::SCENE_NONE)
, mnNextSceneType(SCENE_TYPE::SCENE_NONE)
, mpCurrentScene(nullptr)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize()
{
	 // 初期シーンの設定
	mnNextSceneType = SCENE_TYPE::SCENE_TITLE;

	// シーン遷移をさせる
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	// シーンの更新
	mpCurrentScene->Update();
}

void SceneManager::Draw()
{
	// シーンの描画
	mpCurrentScene->Draw();
}

void SceneManager::Finalize()
{

}

void SceneManager::ChangeSceneIfNeeded()
{
	// 現在シーンと次シーンが一緒であれば何もしない
	if (mnSceneType == mnNextSceneType)
	{
		return;
	}

	if (mpCurrentScene != nullptr)
	{
		// 現在シーンの終了処理をする
		mpCurrentScene->Finalize();

		// 一旦シーン自体も破棄しておく
		delete mpCurrentScene;
		mpCurrentScene = nullptr;
	}

	// 次シーンにするためシーンタイプを更新
	mnSceneType = mnNextSceneType;

	// mnSceneType に応じてシーンを生成する
	switch (mnSceneType)
	{
	case SCENE_TYPE::SCENE_TITLE:
		mpCurrentScene = new TitleScene();
		break;

	case SCENE_TYPE::SCENE_GAME:
		mpCurrentScene = new GameScene();
		break;

	case SCENE_TYPE::SCENE_RESULT:
		mpCurrentScene = new ResultScene();
		break;

	case SCENE_TYPE::SCENE_GAME_RULE:
		mpCurrentScene = new GameRuleScene();
		break;

	case SCENE_TYPE::SCENE_GAMEOVER:
		mpCurrentScene = new GameOverScene();
		break;

	default:
		break;
	}

	// シーンの生成がされているはずなので、初期化処理を読んでおく
	mpCurrentScene->Initialize();

}