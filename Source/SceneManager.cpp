#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "GameRuleScene.h"
#include "Utility.h"
#include <cstdlib>

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	// 画面シェイクや全画面ポストエフェクト合成用の中間スクリーンバッファを生成
	m_workScreenHandle = MakeScreen(Utility::ScreenWidth, Utility::ScreenHeight, TRUE);
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
	if (m_currentScene == nullptr) return;

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
	if (m_workScreenHandle != -1)
	{
		DeleteGraph(m_workScreenHandle);
		m_workScreenHandle = -1;
	}
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

void SceneManager::UpdateTransition()
{
	m_transitionTimer++;

	if (m_transitionType == TransitionType::RedFlashShake)
	{
		if (m_phase == TransitionPhase::TransFadeOut)
		{
			// 被弾・死亡の衝撃を表現するため時間経過とともに減衰するランダム振動を生成
			int currentTime = m_transitionTimer;
			int limitTime = TransitionTime;
			int maxShakeAmount = 15;

			int shakeRange = maxShakeAmount * (limitTime - currentTime) / limitTime;
			if (shakeRange > 0)
			{
				m_shakeOffsetX = (rand() % (shakeRange * 2 + 1)) - shakeRange;
				m_shakeOffsetY = (rand() % (shakeRange * 2 + 1)) - shakeRange;
			}
			else
			{
				m_shakeOffsetX = 0;
				m_shakeOffsetY = 0;
			}
		}
		else
		{
			m_shakeOffsetX = 0;
			m_shakeOffsetY = 0;
		}
	}

	switch (m_phase)
	{
	case TransitionPhase::TransFadeOut:
		if (m_transitionTimer >= TransitionTime)
		{
			ChangeSceneIfNeeded();
			m_phase = TransitionPhase::TransFadeIn;
			m_transitionTimer = 0;
		}
		break;

	case TransitionPhase::TransFadeIn:
		if (m_transitionTimer >= TransitionTime)
		{
			m_isTransition = false;
			m_phase = TransitionPhase::TransNone;
			m_shakeOffsetX = 0;
			m_shakeOffsetY = 0;
		}
		break;
	}
}

void SceneManager::DrawTransitionEffects()
{
	int alpha = 0;
	if (m_phase == TransitionPhase::TransFadeOut)
	{
		alpha = (m_transitionTimer * 255) / TransitionTime;
	}
	else if (m_phase == TransitionPhase::TransFadeIn)
	{
		alpha = ((TransitionTime - m_transitionTimer) * 255) / TransitionTime;
	}

	unsigned int color = GetColor(0, 0, 0);

	switch (m_transitionType)
	{
	case TransitionType::WhiteFlash:
		color = GetColor(255, 255, 255);
		break;
	case TransitionType::RedFlashShake:
		color = GetColor(255, 0, 0);
		break;
	case TransitionType::NormalBlack:
	default:
		color = GetColor(0, 0, 0);
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Utility::ScreenWidth, Utility::ScreenHeight, color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneManager::RequestQuit()
{
	m_quitRequest = true;
}
