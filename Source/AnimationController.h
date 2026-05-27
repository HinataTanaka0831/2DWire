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

    // Register a TextureAnimation for a given state
    void RegisterAnimation(CharacterState state, TextureAnimation* anim);

    // Change state (only resets animation if state actually changed)
    void ChangeState(CharacterState newState);

    // Update the current animation
    void Update();

    // Draw the current animation at the given world position
    void Draw(float x, float y, float cameraX, float cameraY);

    CharacterState GetCurrentState() const { return mCurrentState; }

private:
    std::map<CharacterState, TextureAnimation*> mAnimations;
    CharacterState mCurrentState;
    CharacterState mPrevState;
    bool mIsInitialized;
};
