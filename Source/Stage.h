#pragma once
#include <vector>

// 各ステージの初期座標・ゴール位置・カメラ移動限界を保持する構造体
struct StageInfo
{
    float playerStartX;  // プレイヤー初期出現X座標
    float playerStartY;  // プレイヤー初期出現Y座標
    float goalX;         // ゴール中心X座標
    float goalY;         // ゴール中心Y座標
    float cameraMinX;    // カメラ移動下限X座標
    float cameraMaxX;    // カメラ移動上限X座標
    float playerMinX;    // プレイヤー移動下限X座標
    float playerMaxX;    // プレイヤー移動上限X座標
};

// ステージ番号に応じた地形・ターゲット・敵の配置を構築するクラス
class Stage
{
public:
    Stage();
    ~Stage();

    void Initialize();

    // 指定ステージ番号に応じたオブジェクト群の一括インスタンス化
    // 入力: stageNum(ステージ番号) / 出力: なし / 副作用: WireTarget, Enemy, Goalの生成
    void LoadStage(int stageNum);

    StageInfo GetStageInfo() const { return mStageInfo; }

private:
    StageInfo mStageInfo;
};