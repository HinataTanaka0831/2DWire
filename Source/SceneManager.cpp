#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "GameRuleScene.h"
#include "Utility.h"
#include "TransitionEffectManager.h"


SceneManager::SceneManager()
	: m_sceneType(SceneType::SceneNone)
	, m_nextSceneType(SceneType::SceneNone)
	, m_resultType(ResultType::ResultNone)
	, m_currentScene(nullptr)
	, m_isTransition(false)
	, m_quitRequest(false)

{
}

SceneManager::~SceneManager()
{
	if (m_currentScene != nullptr)
	{
		delete m_currentScene;
		m_currentScene = nullptr;
	}
}

void SceneManager::Initialize()
{
	m_nextSceneType = SceneType::SceneTitle;
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->Update();
	}

	if (m_isTransition)
	{
		UpdateTransition();
	}
}

void SceneManager::Draw()
{
	if (m_currentScene == nullptr)
	{
		return;
	}

	// シェイク演出を適用するため一旦ワークスクリーンへ全描画
	SetDrawScreen(m_workScreenHandle);
	ClearDrawScreen();

	m_currentScene->Draw();

	SetDrawScreen(DX_SCREEN_BACK);

	// シェイクオフセットを加算して本来のバックバッファへ転送
	DrawGraph(m_shakeOffsetX, m_shakeOffsetY, m_workScreenHandle, FALSE);

	if (m_isTransition)
	{
		DrawTransitionEffects();
	}
}

void SceneManager::Finalize()
{
}

void SceneManager::ChangeSceneIfNeeded()
{
	if (m_sceneType == m_nextSceneType)
	{
		return;
	}

	if (m_currentScene != nullptr)
	{
		m_currentScene->Finalize();
		delete m_currentScene;
		m_currentScene = nullptr;
	}

	m_sceneType = m_nextSceneType;

	switch (m_sceneType)
	{
	case SceneType::SceneTitle:
		m_currentScene = new TitleScene();
		break;
	case SceneType::SceneGameStage1:
		m_currentScene = new GameScene();
		break;
	case SceneType::SceneGameStage2:
		m_currentScene = new GameScene();
		break;
	case SceneType::SceneResult:
		m_currentScene = new ResultScene();
		break;
	case SceneType::SceneGameRule:
		m_currentScene = new GameRuleScene();
		break;
	default:
		break;
	}

	if (m_currentScene != nullptr)
	{
		m_currentScene->Initialize();
	}
}

void SceneManager::SetNextScene(SceneType next)
{
	if (m_sceneType == next) return;

	m_nextSceneType = next;

	// シーンの演出意図に応じてトランジション種別を分岐
	if (next == SceneType::SceneGameStage1 || next == SceneType::SceneGameStage2)
	{
		m_transitionType = TransitionType::WhiteFlash;
	}
	else if (next == SceneType::SceneResult && m_resultType == ResultType::ResultGameOver)
	{
		m_transitionType = TransitionType::RedFlashShake;
	}
	else
	{
		m_transitionType = TransitionType::NormalBlack;
	}

	m_isTransition = true;
	m_phase = TransitionPhase::TransFadeOut;
	m_transitionTimer = 0;
	m_shakeOffsetX = 0;
	m_shakeOffsetY = 0;
}

void SceneManager::SetResultType(ResultType result)
{
	if (m_resultType == result) return;

	m_resultType = result;
}


void SceneManager::RequestQuit()
{
	m_quitRequest = true;
}
