#pragma once

class TransitionEffectManager
{
public:
	// 遷移演出の種類
	enum class TransitionType
	{
		NormalBlack,      // 通常の黒フェードイン/アウト
		WhiteFlash,       // 白フラッシュ（ゲーム開始時）
		RedFlashShake,   // 赤フラッシュ＋画面シェイク（ゲームオーバー時）
	};

	// 遷移の進行フェーズ
	enum class TransitionPhase
	{
		TransNone,       // 遷移演出なし
		TransFadeOut,    // フェードアウト中
		TransFadeIn,     // フェードイン中
	};

public:
	static TransitionEffectManager& GetInstance();

	// 遷移演出のフェード・シェイク進行計算
    // 入力: なし / 出力: なし / 副作用: mShakeOffsetX/Yおよびアルファ値の更新
	void UpdateTransition();

	// トランジション用マスク・フラッシュの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへのアルファ描画
	void DrawTransitionEffects();


	int GetWorlScreenHandle() { return m_workScreenHandle; }
	int GetShakeOffsetX() { return m_shakeOffsetX; }
	int GetShakeOffsetY() { return m_shakeOffsetY; }

private:
	TransitionEffectManager();

	~TransitionEffectManager();

	TransitionEffectManager(const TransitionEffectManager&) = delete;
	TransitionEffectManager& operator=(const TransitionEffectManager&) = delete;

	TransitionEffectManager(TransitionEffectManager&&) = delete;
	TransitionEffectManager& operator=(TransitionEffectManager&&) = delete;


private:
	int m_transitionTimer;            // トランジションを行う時間
	const int TransitionTime;         // 遷移にかけるフレーム数（約0.25秒）
	TransitionPhase m_phase;          // 遷移演出の進行フェーズ
	TransitionType m_transitionType;  // 遷移演出の種類（黒フェード/白フラッシュ/赤フラッシュ＋シェイク）
	int m_shakeOffsetX;               // シェイクオフセットX
	int m_shakeOffsetY;               // シェイクオフセットY
	int m_workScreenHandle;           // ワークスクリーンハンドル

};