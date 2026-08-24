#include "Loading.h"
#include "DxLib.h"
#include <string>
#include <cstdio>
#include "Master.h"
#include "GameScene.h"
#include "Scene.h"
#include "ObjectManager.h"

// ========================
// LoadSoundTask 実装
// ========================

LoadSoundTask::LoadSoundTask(const char* path)
    : m_path(path), m_handle(-1)
{}

// サウンドファイルをメモリ上にロード
// 入力: なし / 出力: サウンドハンドル(-1は失敗) / 副作用: m_handleにハンドルを保持
int LoadSoundTask::Execute() {
    m_handle = LoadSoundMem(m_path);
    if (m_handle == -1) {
        char buf[256];
        OutputDebugStringA(buf);
    }
    return m_handle;
}

const char* LoadSoundTask::GetTaskName() const {
    return m_path;
}

int LoadSoundTask::GetHandle() const {
    return m_handle;
}

// ========================
// InitializeSoundManagerTask 実装
// ========================

InitializeSoundManagerTask::InitializeSoundManagerTask() {}

// サウンドマネージャーに必要な全BGM/SEのプリロードを実行
// 入力: なし / 出力: 0 / 副作用: サウンドリソースの読み込み
int InitializeSoundManagerTask::Execute() {
    Master::mpSoundManager->Initialize();
    return 0;
}

const char* InitializeSoundManagerTask::GetTaskName() const {
    return "Initialize Sound Manager";
}

// ========================
// InitializeSceneManagerTask 実装
// ========================

InitializeSceneManagerTask::InitializeSceneManagerTask() {}

// シーンマネージャーの初期化および初期シーンの構築
// 入力: なし / 出力: 0 / 副作用: シーン生成とリソース初期化
int InitializeSceneManagerTask::Execute() {
    Master::mpSceneManager->Initialize();
    return 0;
}

const char* InitializeSceneManagerTask::GetTaskName() const {
    return "Initialize Scene Manager";
}

// ========================
// InitializeLoadStageData 実装
// ========================
InitializeLoadStageData::InitializeLoadStageData() {}

// 現在のゲームシーンに対してステージ地形・オブジェクトデータの構築を要求
// 入力: なし / 出力: 0 / 副作用: GameSceneのステージデータロード実行
int InitializeLoadStageData::Execute() {
    GameScene* pGameScene = dynamic_cast<GameScene*>(Master::mpSceneManager->GetCurrentScene());
    if (pGameScene != nullptr)
    {
        pGameScene->LoadStageData();
    }
    return 0;
}

const char* InitializeLoadStageData::GetTaskName() const {
    return "Initialize LoadStageData";
}

// ========================
// LoadingManager 実装
// ========================

// 実行キューにロードタスクを追加
// 入力: task(ロードタスク) / 出力: なし / 副作用: m_tasksにタスクを格納
void LoadingManager::AddTask(std::unique_ptr<ILoadTask> task) {
    m_tasks.push_back(std::move(task));
}

// 全タスクを順次実行しながら進捗プログレスバーを描画
// 入力: なし / 出力: なし / 副作用: 画面描画、各タスクのExecute呼び出し
void LoadingManager::ExecuteAll() {
    const int total = static_cast<int>(m_tasks.size());

    for (int i = 0; i < total; ++i) {
        ClearDrawScreen();

        float progress = static_cast<float>(i) / total;

        const int barX = 1280, barY = 950, barWidth = 580, barHeight = 40;
        DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(255, 255, 255), FALSE);

        int filledWidth = static_cast<int>(barWidth * progress);
        DrawBox(barX, barY, barX + filledWidth, barY + barHeight, GetColor(100, 200, 255), TRUE);

        std::string loadingText = "Loading: ";
        loadingText += m_tasks[i]->GetTaskName();

        DrawStringToHandle(1280, 900, loadingText.c_str(), GetColor(255, 255, 255), FontSize);

        ScreenFlip();

        m_tasks[i]->Execute();

        // ユーザーに進捗の推移を視認させるための最小待機時間
        WaitTimer(100);
    }

    ClearDrawScreen();
    DrawBox(1280, 950, 1280 + 580, 990, GetColor(255, 255, 255), FALSE);
    DrawBox(1280, 950, 1280 + 580, 990, GetColor(100, 200, 255), TRUE);
    DrawStringToHandle(1280, 900, "Loading Complete!", GetColor(255, 255, 255), FontSize);
    ScreenFlip();

    // 完了表示を視認させるための完了後ウェイト
    WaitTimer(300);
}

// ゲームシーン用のステージ画像付きローディングを実行
// 入力: なし / 出力: なし / 副作用: 画面描画、各タスクのExecute呼び出し
void LoadingManager::ExecuteGameScene()
{
    const int total = static_cast<int>(m_tasks.size());

    for (int i = 0; i < total; ++i)
    {
        ClearDrawScreen();

        Scene3D_GameRuleHandle = LoadGraph("Resource/3D_UI/GameRulePicture.png");
        DrawGraph(0, 0, Scene3D_GameRuleHandle, false);

        float progress = static_cast<float>(i) / total;

        const int barX = 1280, barY = 950, barWidth = 580, barHeight = 40;
        DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(255, 255, 255), FALSE);

        int filledWidth = static_cast<int>(barWidth * progress);
        DrawBox(barX, barY, barX + filledWidth, barY + barHeight, GetColor(100, 200, 255), TRUE);

        ScreenFlip();

        m_tasks[i]->Execute();

        // ロード進行を視認させるための待機時間
        WaitTimer(100);
    }

    ClearDrawScreen();
    DrawBox(1280, 950, 1280 + 580, 990, GetColor(255, 255, 255), FALSE);
    DrawBox(1280, 950, 1280 + 580, 990, GetColor(100, 200, 255), TRUE);

    // ロード画面用テクスチャのメモリリークを防止するため破棄
    DeleteGraph(Scene3D_GameRuleHandle);

    ScreenFlip();

    WaitTimer(300);
}