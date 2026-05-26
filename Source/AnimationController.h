#pragma once
#include <map>
#include "Object2D.h"

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

    void RegisterAnimation(CharacterState state, Object2D* anim);
    void ChangeState(CharacterState newState);
    void Update();
    void Draw(float x, float y);

    CharacterState GetCurrentState() const { return mCurrentState; }

private:
    std::map<CharacterState, Object2D*> mAnimations;
    CharacterState mCurrentState;
    bool mIsInitialized;
};
