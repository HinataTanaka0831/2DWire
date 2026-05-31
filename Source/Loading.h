#pragma once
#include <string>
#include <vector>
#include <memory>
#include "DxLib.h"


// ========== ILoadTaskインタフェース ==========
// ILoadTask はタスクの基底クラス（例）
class ILoadTask {
public:
    virtual ~ILoadTask() = default;
    virtual int Execute() = 0;
    virtual const char* GetTaskName() const = 0;
    virtual int GetHandle() const { return -1; }
};



// ========== サウンド読み込みタスク ==========
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


// ========== 初期化タスク ==========
class InitializeSoundManagerTask : public ILoadTask {
public:
    InitializeSoundManagerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

class InitializeSceneManagerTask : public ILoadTask {
public:
    InitializeSceneManagerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};



// ========== 初期化GameSceneタスク ==========
class InitializePlayerTask : public ILoadTask {
public:
    InitializePlayerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};


class InitializeEnemyTask : public ILoadTask {
public:
    InitializeEnemyTask();
    int Execute() override;
    const char* GetTaskName() const override;
};







// ========== ローディングマネージャー ==========
class LoadingManager {
private:
    std::vector<std::unique_ptr<ILoadTask>> tasks;
    std::vector<std::unique_ptr<ILoadTask>> m_tasks;
    int Scene3D_GameRuleHandle;
    int FontSize = CreateFontToHandle(NULL, 30, -1, -1);

public:
    void AddTask(std::unique_ptr<ILoadTask> task);

    void ExecuteAll();

    void ExecuteGameScene();

    // ここを追加
    const std::vector<std::unique_ptr<ILoadTask>>& GetTasks() const {
        return tasks;
    }
};
