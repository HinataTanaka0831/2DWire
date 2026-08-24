#pragma once
#include <string>
#include <vector>
#include <memory>
#include "DxLib.h"

// ローディング処理を分割実行するためのタスク基底インターフェース
class ILoadTask {
public:
    virtual ~ILoadTask() = default;
    virtual int Execute() = 0;
    virtual const char* GetTaskName() const = 0;
    virtual int GetHandle() const { return -1; }
};

// サウンドリソースをロードするタスク
class LoadSoundTask : public ILoadTask {
public:
    explicit LoadSoundTask(const char* path);
    int Execute() override;
    const char* GetTaskName() const override;
    int GetHandle() const;
private:
    const char* m_path;
    int m_handle;
};

// サウンドマネージャー初期化タスク
class InitializeSoundManagerTask : public ILoadTask {
public:
    InitializeSoundManagerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// シーンマネージャー初期化タスク
class InitializeSceneManagerTask : public ILoadTask {
public:
    InitializeSceneManagerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// ゲームステージデータ読み込みタスク
class InitializeLoadStageData : public ILoadTask {
public:
    InitializeLoadStageData();
    int Execute() override;
    const char* GetTaskName() const override;
};

// 分割ロードタスクを順次実行し進捗UIを表示する管理クラス
class LoadingManager {
private:
    std::vector<std::unique_ptr<ILoadTask>> tasks;
    std::vector<std::unique_ptr<ILoadTask>> m_tasks;
    int Scene3D_GameRuleHandle;
    int FontSize = CreateFontToHandle(NULL, 30, -1, -1);

public:
    // 実行キューにロードタスクを追加
    // 入力: task(ロードタスク) / 出力: なし / 副作用: 内部タスクリストに追加
    void AddTask(std::unique_ptr<ILoadTask> task);

    // 全タスクを順次実行しながら進捗プログレスバーを描画
    // 入力: なし / 出力: なし / 副作用: 画面描画、各タスクのExecute呼び出し
    void ExecuteAll();

    // ゲームシーン用のステージ画像付きローディングを実行
    // 入力: なし / 出力: なし / 副作用: 画面描画、各タスクのExecute呼び出し
    void ExecuteGameScene();

    const std::vector<std::unique_ptr<ILoadTask>>& GetTasks() const {
        return tasks;
    }
};