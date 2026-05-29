#pragma once
#include <map>
#include "TextureAnimation.h"

enum class CharacterState {
    Idle,
    Moving,
    Attacking,
    Damaged,
    Dead
};

class AnimationController {
public:
    AnimationController();
    ~AnimationController();

    // 状態に対応するアニメーションを登録。同じ状態に複数登録した場合は後勝ち
    // 入力：state（登録先状態）, anim（アニメーションへのポインタ、所有権は当クラスが持つ）/ 副作用：既存の登録は上書き
    void RegisterAnimation(CharacterState state, TextureAnimation* anim);

    // 状態遷移。同一状態への遷移時は再初期化によるアニメーションのちらつきを防止
    // 入力：newState（遷移先状態）/ 副作用：状態変更時のみmCurrentStateを更新し再生位置をリセット
    void ChangeState(CharacterState newState);

    // 時間経過に基づくアニメーションコマの進捗更新
    // 副作用：アクティブなアニメーションのカウンタが進む
    void Update();

    // カメラのスクロールを考慮してアニメーションを画面上の相対位置に描画
    // 入力：x, y（オブジェクトのワールド座標）, cameraX, cameraY（カメラ座標）
    void Draw(float x, float y, float cameraX = 0.0f, float cameraY = 0.0f);

    CharacterState GetCurrentState() const { return mCurrentState; }

private:
    std::map<CharacterState, TextureAnimation*> mAnimations;
    CharacterState mCurrentState;
    CharacterState mPrevState;
    bool mIsInitialized;
};
