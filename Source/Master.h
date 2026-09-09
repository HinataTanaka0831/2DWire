#pragma once

#include "SceneManager.h"
#include "SoundManager.h"

// シーンやサウンドなど各所から参照される共有マネージャーへのアクセスを提供するクラス
// 副作用: アプリケーション全体で共有される静的ポインタを保持
class Master
{
public:
	static SceneManager* m_sceneManager;        // シーン遷移および現在シーンを管理するポインタ
	static SoundManager* m_soundManager;        // BGM/SE再生を統括するポインタ
};