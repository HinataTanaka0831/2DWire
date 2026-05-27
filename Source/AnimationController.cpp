#include "AnimationController.h"

AnimationController::AnimationController()
    : mCurrentState(CharacterState::Idle)
    , mPrevState(CharacterState::Idle)
    , mIsInitialized(false)
{
}

AnimationController::~AnimationController()
{
    for (auto& pair : mAnimations) {
        delete pair.second;
    }
    mAnimations.clear();
}

void AnimationController::RegisterAnimation(CharacterState state, TextureAnimation* anim)
{
    mAnimations[state] = anim;
}

void AnimationController::ChangeState(CharacterState newState)
{
    // Same state: do nothing (don't reset animation)
    if (mIsInitialized && mCurrentState == newState) return;

    // Save previous state
    mPrevState = mCurrentState;
    mCurrentState = newState;
    mIsInitialized = true;

    // Reset animation to frame 0
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->Reset();
    }
}

void AnimationController::Update()
{
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->Update();
    }
}

void AnimationController::Draw(float x, float y, float cameraX, float cameraY)
{
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->SetPosition(VGet(x, y, 0.0f));
        mAnimations[mCurrentState]->Draw(cameraX, cameraY);
    }
}
