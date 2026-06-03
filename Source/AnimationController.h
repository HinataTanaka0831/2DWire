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


    // 時間経過に基づくアニメーションコマの進捗更新
    // 副作用：アクティブなアニメーションのカウンタが進む
    void Update();
    void Draw(float x, float y, float cameraX, float cameraY);
    

    void ChangeState(CharacterState newState);

    // カメラのスクロールを考慮してアニメーションを画面上の相対位置に描画
    // 入力：x, y（オブジェクトのワールド座標）, cameraX, cameraY（カメラ座標）
    void SetEnemyReverse(bool rev);

    CharacterState GetCurrentState() const { return mCurrentState; }

private:
    std::map<CharacterState, TextureAnimation*> mAnimations;
    CharacterState mCurrentState;
    CharacterState mPrevState;
    bool mIsInitialized;
};
