#include "TransitionEffectManager.h"
#include "DxLib.h"

TransitionEffectManager::TransitionEffectManager()
   : m_transitionTimer(0)
   , TransitionTime(15)
   , m_phase(TransitionPhase::TransNone)
   , m_transitionType(TransitionType::NormalBlack)
   , m_shakeOffsetX(0)
   , m_shakeOffsetY(0)
   , m_workScreenHandle(-1)

{
	// 画面シェイクや全画面ポストエフェクト合成用の中間スクリーンバッファを生成
	m_workScreenHandle = MakeScreen(Utility::ScreenWidth, Utility::ScreenHeight, TRUE);

}

TransitionEffectManager::~TransitionEffectManager()
{
	if (m_workScreenHandle != -1)
	{
		DeleteGraph(m_workScreenHandle);
		m_workScreenHandle = -1;
	}
}

void TransitionEffectManager::UpdateTransition()
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

void TransitionEffectManager::DrawTransitionEffects()
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
