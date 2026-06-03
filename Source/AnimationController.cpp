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
    // 同一状態でのアニメーション再初期化によるちらつきを防止
    if (mIsInitialized && mCurrentState == newState) return;

    mPrevState = mCurrentState;
    mCurrentState = newState;
    mIsInitialized = true;

    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->Reset();
    }
}

void AnimationController::SetEnemyReverse(bool rev)
{
    // Apply reverse flag to the currently active animation (if any)
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->SetReverse(rev);
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
